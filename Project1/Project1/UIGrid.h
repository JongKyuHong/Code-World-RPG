#pragma once
#include <string>
#include <vector>
#include <functional>

class Item;
class Inventory;
class Shop;
class Character;
class MapAsciiArtRepository;

class UIGrid {
public:
    UIGrid(MapAsciiArtRepository& artRepo);

    void printHeader(const std::string& title);
    void waitAnyKey(const std::string& msg = "");
    int askInt(const std::string& prompt, int min, int max);

    void showItemArt(const std::string& itemName);

    // 핵심: 폭 계산 API
    int calcItemGridWidth(
        const std::vector<Item*>& list,
        const std::function<std::string(int, Item*)>& makeLabelFn,
        int cols,
        int cellW,
        int padding = 2
    ) const;

    int calcShopGridWidth(Shop& shop, Character& player, int cols, int cellW, int padding = 2) const;

    // 그리드 출력
    void printItemGrid(
        const std::vector<Item*>& list,
        const std::string& title,
        std::function<std::string(int, Item*)> makeLabelFn,
        int cols,
        int cellW,
        int artH,
        bool printTitle = true
    );

    void printInventoryGrid(Inventory& inv, int cols, int cellW, int artH);
    void printShopGrid(Shop& shop, Character& player, int cols, int cellW, int artH);

    int askInventoryIndexOrCancel(Inventory& inv, const std::string& prompt);

private:
    MapAsciiArtRepository& artRepo;

private:
    // ===== UTF-8 표시폭/자 tells (UIGrid 내부 전용) =====
    static uint32_t utf8NextCodepoint(const std::string& s, size_t& i);
    static int codepointDisplayWidth(uint32_t cp);
    static int displayWidthUTF8(const std::string& s);

    static std::string cutUTF8ByWidth(const std::string& s, int targetWidth);
    static std::string fitWidthUTF8(const std::string& s, int cellW);

    static int clampInt(int v, int lo, int hi);

    static int autoCellWidthUTF8(
        const std::vector<Item*>& list,
        const std::function<std::string(int, Item*)>& makeLabelFn,
        int minW = 22,
        int maxW = 44,
        int padding = 2
    );

    static void printGridHeader(const std::string& title, int gridWidth);
};