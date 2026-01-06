// UIGrid.cpp
#include "UIGrid.h"

#include <iostream>
#include <limits>
#include <conio.h>
#include <algorithm>
#include <functional>

#include "Inventory.h"
#include "Shop.h"
#include "Character.h"
#include "Item.h"
#include "MapAsciiArtRepository.h"

UIGrid::UIGrid(MapAsciiArtRepository& artRepo)
    : artRepo(artRepo)
{
}

void UIGrid::printHeader(const std::string& title) {
    std::cout << "========================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "========================================\n";
}

void UIGrid::waitAnyKey(const std::string& msg) {
    std::cout << msg;
    (void)_getch();
    std::cout << "\n";
}

int UIGrid::askInt(const std::string& prompt, int min, int max) {
    while (true) {
        std::cout << prompt;
        int x;
        std::cin >> x;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::cout << "숫자를 입력하세요.\n";
            continue;
        }
        if (x < min || x > max) {
            std::cout << "범위를 벗어났습니다. (" << min << "~" << max << ")\n";
            continue;
        }
        return x;
    }
}

void UIGrid::showItemArt(const std::string& itemName) {
    const AsciiLines* lines = artRepo.findByItemName(itemName);
    if (!lines) return;

    std::cout << "\n";
    for (const auto& l : *lines) std::cout << l << "\n";
    std::cout << "\n";
}

// ========================= UTF-8 표시폭 유틸(UIGrid private static) =========================

uint32_t UIGrid::utf8NextCodepoint(const std::string& s, size_t& i) {
    unsigned char c = (unsigned char)s[i];
    if (c < 0x80) { ++i; return c; }
    if ((c & 0xE0) == 0xC0 && i + 1 < s.size()) {
        uint32_t cp = ((c & 0x1F) << 6) | ((unsigned char)s[i + 1] & 0x3F);
        i += 2; return cp;
    }
    if ((c & 0xF0) == 0xE0 && i + 2 < s.size()) {
        uint32_t cp = ((c & 0x0F) << 12)
            | (((unsigned char)s[i + 1] & 0x3F) << 6)
            | ((unsigned char)s[i + 2] & 0x3F);
        i += 3; return cp;
    }
    if ((c & 0xF8) == 0xF0 && i + 3 < s.size()) {
        uint32_t cp = ((c & 0x07) << 18)
            | (((unsigned char)s[i + 1] & 0x3F) << 12)
            | (((unsigned char)s[i + 2] & 0x3F) << 6)
            | ((unsigned char)s[i + 3] & 0x3F);
        i += 4; return cp;
    }
    ++i;
    return '?';
}

int UIGrid::codepointDisplayWidth(uint32_t cp) {
    if (cp == 0) return 0;
    if (cp < 32 || (cp >= 0x7F && cp < 0xA0)) return 0;

    // Hangul
    if ((cp >= 0x1100 && cp <= 0x11FF) ||
        (cp >= 0x3130 && cp <= 0x318F) ||
        (cp >= 0xAC00 && cp <= 0xD7A3))
        return 2;

    // CJK / Fullwidth blocks
    if ((cp >= 0x2E80 && cp <= 0xA4CF) ||
        (cp >= 0xF900 && cp <= 0xFAFF) ||
        (cp >= 0xFE10 && cp <= 0xFE19) ||
        (cp >= 0xFE30 && cp <= 0xFE6F) ||
        (cp >= 0xFF01 && cp <= 0xFF60) ||
        (cp >= 0xFFE0 && cp <= 0xFFE6))
        return 2;

    return 1;
}

int UIGrid::displayWidthUTF8(const std::string& s) {
    int w = 0;
    for (size_t i = 0; i < s.size();) {
        uint32_t cp = utf8NextCodepoint(s, i);
        w += codepointDisplayWidth(cp);
    }
    return w;
}

std::string UIGrid::cutUTF8ByWidth(const std::string& s, int targetWidth) {
    if (targetWidth <= 0) return "";
    std::string out;
    out.reserve(s.size());

    int w = 0;
    for (size_t i = 0; i < s.size();) {
        size_t before = i;
        uint32_t cp = utf8NextCodepoint(s, i);
        int cw = codepointDisplayWidth(cp);
        if (w + cw > targetWidth) break;
        out.append(s, before, i - before);
        w += cw;
    }
    return out;
}

std::string UIGrid::fitWidthUTF8(const std::string& s, int cellW) {
    std::string cut = cutUTF8ByWidth(s, cellW);
    int w = displayWidthUTF8(cut);
    if (w >= cellW) return cut;
    return cut + std::string(cellW - w, ' ');
}

int UIGrid::clampInt(int v, int lo, int hi) {
    return std::max(lo, std::min(v, hi));
}

int UIGrid::autoCellWidthUTF8(
    const std::vector<Item*>& list,
    const std::function<std::string(int, Item*)>& makeLabelFn,
    int minW,
    int maxW,
    int padding
) {
    int best = minW;
    for (int i = 0; i < (int)list.size(); ++i) {
        Item* it = list[i];
        if (!it) continue;
        std::string label = makeLabelFn(i, it);
        best = std::max(best, displayWidthUTF8(label) + padding);
    }
    return clampInt(best, minW, maxW);
}

void UIGrid::printGridHeader(const std::string& title, int gridWidth) {
    if (gridWidth < 10) gridWidth = 10;
    std::cout << std::string(gridWidth, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(gridWidth, '=') << "\n";
}

// ========================= 폭 계산 =========================

int UIGrid::calcItemGridWidth(
    const std::vector<Item*>& list,
    const std::function<std::string(int, Item*)>& makeLabelFn,
    int cols,
    int cellW,
    int padding
) const {
    cols = std::max(1, cols);

    const std::string SEP = "  ||  ";
    const int sepW = (int)SEP.size();

    int resolvedCellW = cellW;
    if (resolvedCellW <= 0) {
        resolvedCellW = autoCellWidthUTF8(list, makeLabelFn, 22, 44, padding);
    }

    return cols * resolvedCellW + (cols - 1) * sepW;
}

int UIGrid::calcShopGridWidth(Shop& shop, Character& player, int cols, int cellW, int padding) const {
    std::vector<Item*> temp;
    temp.reserve(shop.size());
    for (int i = 0; i < shop.size(); ++i) temp.push_back(shop.peek(i));

    auto labelShop = [&player](int idx, Item* it) {
        int price = it->getPrice();
        std::string label = "[" + std::to_string(idx) + "] " + it->getName()
            + " (" + std::to_string(price) + "G)";
        if (player.getGold() < price) label += " (구매불가)";
        return label;
        };

    return calcItemGridWidth(temp, labelShop, cols, cellW, padding);
}

// ========================= 아이템 그리드 출력 =========================

void UIGrid::printItemGrid(const std::vector<Item*>& list,
    const std::string& title,
    std::function<std::string(int, Item*)> makeLabelFn,
    int cols,
    int cellW,
    int artH,
    bool printTitle)
{
    cols = std::max(1, cols);
    artH = std::max(0, artH);

    // ✅ 비어있을 때도: printTitle일 때만 헤더 출력
    if (list.empty()) {
        if (printTitle && !title.empty()) {
            printGridHeader(title, 40);
        }
        std::cout << "(비어 있음)\n";
        return;
    }

    if (cellW <= 0) {
        cellW = autoCellWidthUTF8(list, makeLabelFn, 22, 44, 2);
    }

    const int cellH = 1 + artH + 1;
    const std::string SEP = "  ||  ";
    const int sepW = (int)SEP.size();

    const int gridWidth = cols * cellW + (cols - 1) * sepW;

    // ✅ 핵심: printTitle일 때만 헤더 출력 (중복 헤더 방지)
    if (printTitle && !title.empty()) {
        printGridHeader(title, gridWidth);
    }

    for (int start = 0; start < (int)list.size(); start += cols) {
        int end = std::min(start + cols, (int)list.size());
        int rowCols = end - start;

        std::vector<std::vector<std::string>> cellLines;
        cellLines.reserve(rowCols);

        for (int i = start; i < end; ++i) {
            std::vector<std::string> lines(cellH, "");

            Item* it = list[i];
            if (!it) {
                lines[0] = fitWidthUTF8("[" + std::to_string(i) + "] (EMPTY)", cellW);
                for (int a = 0; a < artH; ++a) lines[1 + a] = fitWidthUTF8("", cellW);
                lines[cellH - 1] = fitWidthUTF8("", cellW);
                cellLines.push_back(std::move(lines));
                continue;
            }

            lines[0] = fitWidthUTF8(makeLabelFn(i, it), cellW);

            const AsciiLines* art = artRepo.findByItemName(it->getName());
            for (int a = 0; a < artH; ++a) {
                std::string rowStr;
                if (art && a < (int)art->size()) rowStr = (*art)[a];
                else rowStr = "";
                lines[1 + a] = fitWidthUTF8(rowStr, cellW);
            }

            lines[cellH - 1] = fitWidthUTF8("", cellW);
            cellLines.push_back(std::move(lines));
        }

        for (int r = 0; r < cellH; ++r) {
            for (int c = 0; c < (int)cellLines.size(); ++c) {
                std::cout << cellLines[c][r];
                if (c != (int)cellLines.size() - 1) std::cout << SEP;
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}

void UIGrid::printInventoryGrid(Inventory& inv, int cols, int cellW, int artH) {
    auto labelInv = [](int idx, Item* it) {
        return "[" + std::to_string(idx) + "] " + it->getName();
        };

    printItemGrid(inv.getItems(), "🎒 인벤토리", labelInv, cols, cellW, artH, true);

    std::cout << "----------------------------------------\n";
    std::cout << "장착 상태:\n";
    inv.listEquipped();
}

void UIGrid::printShopGrid(Shop& shop, Character& player, int cols, int cellW, int artH) {
    std::vector<Item*> temp;
    temp.reserve(shop.size());
    for (int i = 0; i < shop.size(); ++i) temp.push_back(shop.peek(i));

    auto labelShop = [&player](int idx, Item* it) {
        int price = it->getPrice();
        std::string label = "[" + std::to_string(idx) + "] " + it->getName()
            + " (" + std::to_string(price) + "G)";
        if (player.getGold() < price) label += " (구매불가)";
        return label;
        };

    // ✅ UIManager가 상단 헤더를 찍기 때문에 여기서는 타이틀 출력 끔
    printItemGrid(temp, "🏪 상점", labelShop, cols, cellW, artH, false);
}

int UIGrid::askInventoryIndexOrCancel(Inventory& inv, const std::string& prompt) {
    const auto& items = inv.getItems();
    if (items.empty()) {
        waitAnyKey("인벤이 비었습니다. ");
        return -1;
    }

    std::cout << prompt << " (취소:-1)\n";
    while (true) {
        std::cout << "번호: ";
        int idx;
        std::cin >> idx;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::cout << "숫자를 입력하세요.\n";
            continue;
        }
        if (idx == -1) return -1;
        if (idx < 0 || idx >= (int)items.size() || !items[idx]) {
            std::cout << "잘못된 번호입니다.\n";
            continue;
        }
        return idx;
    }
}
