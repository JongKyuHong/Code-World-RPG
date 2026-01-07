#define NOMINMAX
#include <Windows.h>
#include "UIManager.h"
#include "UIGrid.h"
#include "Shop.h"
#include "Character.h"
#include "Inventory.h"
#include "Item.h"
#include "MapAsciiArtRepository.h"
#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
#include <map>
#include <limits>

UIManager::UIManager(MapAsciiArtRepository& artRepo)
    : artRepo(artRepo)
    , grid(artRepo)   // ✅ 핵심
{
}

void UIManager::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void UIManager::waitForKeyPress() {
    std::cout << "\n계속하려면 아무 키나 누르세요...\n";
    _getch();
}

void UIManager::showCharacterCreationScreen() {
    clearScreen();
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║                  ⚔️  캐 릭 터  생 성  ⚔️                   ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║                  Character Creation                        ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    std::cout << "  당신의 이름을 입력하여 모험을 시작하세요.\n";
    std::cout << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "  📝 캐릭터 이름: ";
}

std::string UIManager::getPlayerName() {
    // 버퍼 클리어
    while (_kbhit()) {
        _getch();
    }
    std::cin.clear();
    std::cin.sync();
    while (std::cin.rdbuf()->in_avail() > 0) {
        std::cin.ignore();
    }

    std::string playerName;
    if (std::cin.peek() == '\n') {
        std::cin.ignore();
    }
    std::getline(std::cin, playerName);

    if (playerName.empty()) {
        std::cout << "\n이름을 입력하지 않았습니다. 기본 이름 'Hero'로 설정합니다.\n";
        return "Hero";
    }

    return playerName;
}

void UIManager::showPlayerStats(std::string name, int health, int maxHealth, int attack, int level, int gold) {
    std::cout << "\n이름 : " << name << "\n";
    std::cout << "체력: " << health << " / " << maxHealth << "\n";
    std::cout << "공격력: " << attack << "\n";
    std::cout << "레벨: " << level << "\n";
    std::cout << "골드: " << gold << " G\n";
    waitForKeyPress();
}

void UIManager::showPhase1Entry() {
    clearScreen();
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║        🌲    변 수 의  숲    🌲                           ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║              The Forest of Variables                       ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    std::cout << "    🌳                  🌳                  🌳\n";
    std::cout << "         🌲      🌲           🌲      🌲\n";
    std::cout << "\n";
    std::cout << "  \"이곳은 모든 데이터가 시작되는 곳...\"\n";
    std::cout << "  \"변하는 것과 변하지 않는 것을 구분하라.\"\n";
    std::cout << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "  📊 총 라운드: 5 | 난이도: ★☆☆☆☆\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    waitForKeyPress();
}

void UIManager::showPhase2Entry() {
    clearScreen();
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║        🏰    클 래 스 의  성    🏰                        ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║              The Castle of Classes                         ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    std::cout << "          🏰\n";
    std::cout << "         /||\\\n";
    std::cout << "        /_||_\\\n";
    std::cout << "       /  ||  \\\n";
    std::cout << "      |___||___|\n";
    std::cout << "\n";
    std::cout << "  \"객체들이 태어나고 소멸하는 신비로운 성...\"\n";
    std::cout << "  \"클래스는 설계도, 객체는 실체. 그 경계를 넘어라.\"\n";
    std::cout << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "  📊 총 라운드: 6 | 난이도: ★★★☆☆\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    waitForKeyPress();
}

void UIManager::showPhase3Entry() {
    clearScreen();
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║      ⚡  디 자 인  패 턴 의  탑  ⚡                       ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║           The Tower of Design Patterns                     ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    std::cout << "            ⚡\n";
    std::cout << "           /^\\\n";
    std::cout << "          /   \\\n";
    std::cout << "         /  ⚡  \\\n";
    std::cout << "        /_______\\\n";
    std::cout << "        |   |   |\n";
    std::cout << "        |   |   |\n";
    std::cout << "\n";
    std::cout << "  \"고대 마법사들이 남긴 설계의 정수...\"\n";
    std::cout << "  \"패턴을 이해하는 자만이 탑의 정상에 오를 수 있다.\"\n";
    std::cout << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "  📊 총 라운드: 4 | 난이도: ★★★★★\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    waitForKeyPress();
}

void UIManager::showMonsterEncounter(const std::string& monsterName) {
    clearScreen();
    std::cout << monsterName << "가 나타났다." << std::endl;
}

void UIManager::showBossEncounter(const std::string& bossName) {
    std::cout << "보스 " << bossName << "가 나타났다." << std::endl;
    std::cout << "⚔️ " << bossName << "와 전투 중 ⚔️\n";
}

void UIManager::showShopMenu(int playerGold) {
    clearScreen();
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║                    🏪  상 점  🏪                           ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║                    Item Shop                               ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "  💰 보유 골드: " << playerGold << " G\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "\n";
    std::cout << "  [메뉴]\n";
    std::cout << "  ┌────────────────────────────────────────────────────────┐\n";
    std::cout << "  │  1. 🛒 아이템 구매                                     │\n";
    std::cout << "  │  2. 💸 아이템 판매                                     │\n";
    std::cout << "  │  3. 🚪 상점 나가기                                     │\n";
    std::cout << "  └────────────────────────────────────────────────────────┘\n";
    std::cout << "\n  선택: ";
}

int UIManager::askShopVisit() {
    clearScreen();
    while (_kbhit()) {
        _getch();
    }
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║                  🛤️  갈 림 길  🛤️                          ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    std::cout << "       🏪  ←  [당신]  →  ⚔️\n";
    std::cout << "\n";
    std::cout << "  전투를 계속하기 전에 상점을 방문하시겠습니까?\n";
    std::cout << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "  [선택지]\n";
    std::cout << "  1. ⚔️  즉시 전투\n";
    std::cout << "  2. 🏪 상점 방문 (아이템 구매/판매)\n";   
    std::cout << "  3. 🎒  인 벤 토 리\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "\n  선택: ";

    int choice;
    std::cin >> choice;
    return choice;
}

void UIManager::showDeathScreen() {
    clearScreen();
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║                    💀  사 망  💀                          ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║                      You Died                              ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    std::cout << "                    ___\n";
    std::cout << "                   /   \\\n";
    std::cout << "                  | x x |\n";
    std::cout << "                  |  ^  |\n";
    std::cout << "                   \\___/\n";
    std::cout << "\n";
    std::cout << "  당신은 쓰러졌습니다...\n";
    std::cout << "  하지만 모험은 여기서 끝나지 않습니다.\n";
    std::cout << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "  [선택지]\n";
    std::cout << "  1. ♻️  재도전 (현재 전투 다시 시작)\n";
    std::cout << "  2. 🚪 게임 종료\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "\n  선택: ";
}

char UIManager::askRetryOrQuit() {
    char choice;
    while (true) {
        std::cin >> choice;
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

        if (choice == '1' || choice == '2') {
            return choice;
        } else {
            std::cout << "잘못된 입력입니다. 1 또는 2를 입력하세요: ";
        }
    }
}

void UIManager::showRetryScreen(const std::string& phaseName, int currentRound, int totalRounds) {
    clearScreen();
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║                ♻️  전 투  재 시 작  ♻️                    ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    std::cout << "        ✨ 체력이 완전히 회복되었습니다! ✨\n";
    std::cout << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "  📊 현재 상태\n";
    std::cout << "  ├─ 라운드: " << currentRound << " / " << totalRounds << "\n";
    std::cout << "  └─ 페이즈: " << phaseName << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "\n";

    if (currentRound >= totalRounds) {
        std::cout << "  🔥 보스전으로 복귀합니다...\n";
    } else {
        std::cout << "  ⚔️  일반 전투로 복귀합니다...\n";
    }
}

void UIManager::showEndingScreen(
    const std::string& playerName,
    int level,
    int gold,
    int health,
    int maxHealth,
    int attack,
    const std::map<std::string, int>& mobKillCounts
) {
    clearScreen();
    std::cout << "\n\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║              🎉  게 임  클 리 어!  🎉                     ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║                   Congratulations!                         ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    std::cout << "                    ⭐️\n";
    std::cout << "                   ⭐️⭐️\n";
    std::cout << "                  ⭐️⭐️⭐️\n";
    std::cout << "                 ⭐️ 🏆 ⭐️\n";
    std::cout << "                ⭐️⭐️⭐️⭐️⭐️\n";
    std::cout << "\n";
    std::cout << "  당신은 디자인 패턴의 탑을 정복했습니다!\n";
    std::cout << "  모든 페이즈를 클리어하고 진정한 C++ 마스터가 되었습니다.\n";
    std::cout << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "  📊 최종 통계\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "  🎮 플레이어: " << playerName << "\n";
    std::cout << "  ⭐ 최종 레벨: " << level << "\n";
    std::cout << "  💰 보유 골드: " << gold << " G\n";
    std::cout << "  ❤️  최종 체력: " << health << " / " << maxHealth << "\n";
    std::cout << "  ⚔️  공격력: " << attack << "\n";
    std::cout << "\n";
    std::cout << "  🏆 처치한 몬스터\n";

    for (const auto& mobKillCount : mobKillCounts) {
        std::cout << "     - " << mobKillCount.first << ": " << mobKillCount.second << "마리\n";
    }

    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "\n";
    std::cout << "  \"변수의 숲에서 시작한 여정이 디자인 패턴의 정상에서 끝났다.\"\n";
    std::cout << "  \"이제 당신은 C++의 세계를 자유롭게 탐험할 수 있다.\"\n";
    std::cout << "\n";
    std::cout << "  \"하지만 진정한 모험은 이제부터 시작이다...\"\n";
    std::cout << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "\n";
    std::cout << "              제작: No. 1 우리가 최고다 조\n";
    std::cout << "              감사합니다!\n";
    std::cout << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    waitForKeyPress();
}


void UIManager::showBattleStart(bool isBoss) {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                            ║\n";
    if (isBoss) {
        std::cout << "║              ⚔️  보 스  전 투  시 작!  ⚔️                  ║\n";
    } else {
        std::cout << "║                ⚔️  전 투  시 작!  ⚔️                       ║\n";
    }
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
}

void UIManager::showMonsterInfo(const std::string& name, const std::string& info,
    int hp, int maxHp, int attack) {
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "  👹 적: " << name << "\n";
    std::cout << "  📝 설명: " << info << "\n";
    std::cout << "  ❤️  체력: " << hp << " / " << maxHp << "\n";
    std::cout << "  ⚔️  공격력: " << attack << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

}

void UIManager::showTurnNumber(int turnNumber) {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                     Turn " << turnNumber << "                                 ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
}

void UIManager::showBattleStatus(const std::string& playerName, int playerHp, int playerMaxHp, int playerAtk,
    const std::string& monsterName, int monsterHp, int monsterMaxHp, int monsterAtk) {
    std::cout << "\n";
    std::cout << "┌────────────────────────────────────────────────────────────┐\n";
    std::cout << "│  👤 " << playerName << "\n";
    std::cout << "│  ❤️  HP: " << playerHp << " / " << playerMaxHp
        << " | ⚔️  공격력: " << playerAtk << "\n";
    std::cout << "├────────────────────────────────────────────────────────────┤\n";
    std::cout << "│  👹 " << monsterName << "\n";
    std::cout << "│  ❤️  HP: " << monsterHp << " / " << monsterMaxHp
        << " | ⚔️  공격력: " << monsterAtk << "\n";
    std::cout << "└────────────────────────────────────────────────────────────┘\n";
}

char UIManager::showPlayerTurnMenuAndGetChoice() {
    std::cout << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "  [플레이어 턴]\n";
    std::cout << "  1. ⚔️  공격\n";
    std::cout << "  2. 💊 아이템 사용\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "  선택: ";

    char choice;
    std::cin >> choice;

    // 입력 버퍼 비우기 (Enter 키 제거)
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    return choice;
}

void UIManager::showPlayerAttackResult(const std::string& playerName, const std::string& monsterName,
    int damage, bool isMonsterDead , bool isCritical) {
    std::string turnTitle = playerName + "의 턴";

    const int BOX_WIDTH = 60;

    int displayWidth = 0;

    // 플레이어 이름의 표시 너비 계산
    for (size_t i = 0; i < playerName.length(); ) {
        unsigned char c = playerName[i];

        if ((c & 0x80) == 0) {
            // ASCII (영문/숫자) - 1칸
            displayWidth += 1;
            i += 1;
        } else {
            // 한글 등 멀티바이트 - 2칸
            displayWidth += 2;
            // UTF-8에서 한글은 3바이트
            i += 3;
        }
    }

    // "의 턴" 추가: 의(2칸) + 공백(1칸) + 턴(2칸) = 5칸
    displayWidth += 5;

    int totalPadding = BOX_WIDTH - displayWidth;
    int leftPadding = totalPadding / 2;
    int rightPadding = totalPadding - leftPadding;

    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║" << std::string(leftPadding, ' ') << turnTitle << std::string(rightPadding, ' ') << "║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";

    // 2. 크리티컬 여부에 따른 출력 추가
    if (isCritical) {
        std::cout << "\n\t💥 [ CRITICAL HIT! ] 💥\n";
    }


    std::cout << "\n⚔️  " << playerName << "의 공격!\n";
    std::cout << "→ " << monsterName << "에게 " << damage << " 데미지!\n";

    if (isMonsterDead) {
        std::cout << "→ " << monsterName << "을(를) 쓰러뜨렸다!\n";
    }
}

void UIManager::showItemActionScreen(
    const std::string& title,
    const std::string& itemName,
    const std::string& effectText
) {
    const int BOX_WIDTH = 60;

    // ── 타이틀 표시 너비 계산 (한글 대응)
    int displayWidth = 0;
    for (size_t i = 0; i < title.length();) {
        unsigned char c = title[i];
        if ((c & 0x80) == 0) {
            displayWidth += 1;
            i += 1;
        }
        else {
            displayWidth += 2;
            i += 3;
        }
    }

    int totalPadding = BOX_WIDTH - displayWidth;
    int leftPadding = totalPadding / 2;
    int rightPadding = totalPadding - leftPadding;

    std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║" << std::string(leftPadding, ' ')
        << title
        << std::string(rightPadding, ' ') << "║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";

    std::cout << "\n→ 대상 아이템: " << itemName << "\n";
    std::cout << "→ 효과: " << effectText << "\n";
}


void UIManager::showInvalidInput() {
    std::cout << "❌ 잘못된 입력입니다.다시 선택해주세요.\n";
}

void UIManager::showMonsterTurnHeader(const std::string& monsterName) {
    std::string turnTitle = monsterName + "의 턴";

    const int BOX_WIDTH = 60;

    // 몬스터 이름은 영문만 = 1글자당 1칸
    // "의 턴" = 의(2칸) + 공백(1칸) + 턴(2칸) = 5칸
    int displayWidth = monsterName.length() + 5;

    int totalPadding = BOX_WIDTH - displayWidth;
    int leftPadding = totalPadding / 2;
    int rightPadding = totalPadding - leftPadding;

    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║" << std::string(leftPadding, ' ') << turnTitle << std::string(rightPadding, ' ') << "║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
}

void UIManager::showMonsterAttackResult(const std::string& monsterName, const std::string& playerName,
    int damage, bool isPlayerDead, int remainingHp, int maxHp) {
    std::cout << "💥 " << monsterName << "의 공격!\n";
    std::cout << "→ " << playerName << "이(가) " << damage << " 데미지를 받았다!\n";

    if (isPlayerDead) {
        std::cout << "💀 체력이 모두 소진되었다...\n";
    } else {
        std::cout << "  → 남은 체력: " << remainingHp << " / " << maxHp << "\n";
    }
}

void UIManager::showVictoryScreen(bool isBoss,
    int gold,
    int exp,
    const std::vector<std::string>& droppedItems)
{
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║                    🎉  승 리!  🎉                          ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";

    if (isBoss) {
        std::cout << "\n🏆 보스를 처치했습니다! 🏆\n";
    }

    std::cout << "\n획득 보상:\n";
    std::cout << "💰 골드: " << gold << " G\n";
    std::cout << "⭐ 경험치: " << exp << " EXP\n";

    // ✅ 아이템 출력
    if (!droppedItems.empty()) {
        std::cout << "\n🎁 획득 아이템:\n";
        for (const auto& name : droppedItems) {
            std::cout << "  - " << name << "\n";
        }
    }
    else {
        std::cout << "\n🎁 획득 아이템: 없음\n";
    }

    std::cout << "\n";
}

void UIManager::showDefeatScreen() {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║                    💀  패 배...  💀                       ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n당신은 쓰러졌습니다...\n";
}

void UIManager::showBattleLog(const std::vector<std::string>& battleLog) {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    📜  전 투  로 그  📜                   ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";

    if (battleLog.empty()) {
        std::cout << "  기록된 로그가 없습니다.\n";
    } else {
        for (size_t i = 0; i < battleLog.size(); i++) {
            std::cout << "  [" << (i + 1) << "] " << battleLog[i] << "\n";
        }
    }

    std::cout << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
}

void UIManager::showQuizRound(int current, int total) {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "           🧠  Round " << current << "/" << total << "  🧠\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
}

std::string UIManager::askQuizAnswer(const std::string& question) {
    std::cin.ignore(1000, '\n');
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "   " << question << "\n";  // 질문: 스페이스 3개
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "💭 답변: ";


    std::string answer;
    std::getline(std::cin, answer);

    size_t start = answer.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return "";
    size_t end = answer.find_last_not_of(" \t\n\r\f\v");
    answer = answer.substr(start, end - start + 1);

    return answer.empty() ? "pass" : answer;
}

void UIManager::showQuizResult(bool correct, const std::string& message, int playerScore, int totalRounds) {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    if (correct) {
        std::cout << "  ✅ 정답!  ✅\n";
    } else {
        std::cout << "  ❌ 틀림! ❌\n";
    }
    std::cout << "╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "  📊 정답 수: " << playerScore << "/" << totalRounds << "\n";
    std::cout << "  💬 " << message << "\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
}

void UIManager::showMessage(const std::string& message) {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                            ║\n";
    std::cout << "   " << message << "\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";  // 다음 출력 여백
}

// ===========================
// ✅ Shop 통솔(흐름)
// ===========================
void UIManager::runShop(Shop& shop, Character& player, Inventory& inv) {
    const int cols = 3;
    const int cellW = 0;  // 0이면 자동 폭
    const int artH = 8;

    bool shopping = true;
    while (shopping) {
        clearScreen();

        // ✅ 그리드 폭 계산 (헤더 줄 길이 맞추기)
        int gridW = grid.calcShopGridWidth(shop, player, cols, cellW);

        // ✅ 상단 헤더: 너비를 그리드폭으로 통일
        if (gridW < 10) gridW = 10;
        std::cout << std::string(gridW, '=') << "\n";
        std::cout << "  🏪 상점\n";
        std::cout << std::string(gridW, '=') << "\n";

        // ✅ 골드 표시: 매 루프마다 다시 출력되니 자동 갱신됨
        std::cout << "보유 골드: " << player.getGold() << "G\n\n";

        // ✅ 여기서 그리드는 타이틀을 출력하지 않게(=중복 헤더 방지)
        grid.printShopGrid(shop, player, cols, cellW, artH);
        std::cout << "\n";

        std::cout << "[메뉴]\n";
        std::cout << "  ┌────────────────────────────────────────────────────────┐\n";
        std::cout << "  │  1. 🛒 아이템 구매                                     │\n";
        std::cout << "  │  2. 💸 아이템 판매                                     │\n";
        std::cout << "  │  0. 🚪 상점 나가기                                     │\n";
        std::cout << "  └────────────────────────────────────────────────────────┘\n";

        int choice = grid.askInt("선택: ", 0, 2);
        switch (choice) {
        case 1: doBuy(shop, player, inv); break;
        case 2: doSell(shop, player, inv); break;
        case 0: shopping = false; break;
        }
    }
}
// ===========================
// ✅ Inventory 통솔(흐름)
// ===========================
InventoryAction UIManager::askInventoryAction(Inventory& inv) {
    while (true) {
        clearScreen();
        grid.printInventoryGrid(inv, 3, 0, 8);

        std::cout << "\n";
        std::cout << "╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                    🎒  인 벤 토 리  🎒                     ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n";
        std::cout << "\n";

        std::cout << "  ┌────────────────────────────────────────────────────────┐\n";
        std::cout << "  │  1. 💊 아이템 사용                                     │\n";
        std::cout << "  │  2. ⚔️  장비 장착                                     │\n";
        std::cout << "  │  3. 🛡️  장비 해제                                     │\n";
        std::cout << "  │  0. 🚪 나가기                                         │\n";
        std::cout << "  └────────────────────────────────────────────────────────┘\n";

        int c = grid.askInt("선택: ", 0, 4);

        InventoryAction act{}; // ✅ 안전 초기화

        if (c == 0) {
            act.type = InventoryAction::Exit;
            return act;
        }

        if (c == 3) {
            std::cout << "\n";
            std::cout << "╔════════════════════════════════════════════════════════════╗\n";
            std::cout << "║                    🛡️  장 비  해 제  🛡️                  ║\n";
            std::cout << "╚════════════════════════════════════════════════════════════╝\n";
            std::cout << "\n";

            std::cout << "  ┌────────────────────────────────────────────────────────┐\n";
            std::cout << "  │  1. ⚔️  무기 해제                                     │\n";
            std::cout << "  │  2. 🛡️  방어구 해제                                   │\n";
            std::cout << "  │  0. ↩️  취소                                         │\n";
            std::cout << "  └────────────────────────────────────────────────────────┘\n";

            int s = grid.askInt("선택: ", 0, 2);
            if (s == 0) continue;

            act.type = InventoryAction::Unequip;
            act.slot = (s == 1) ? EquipSlot::Weapon : EquipSlot::Armor;
            return act;
        }

        int idx = grid.askInventoryIndexOrCancel(inv, "\n아이템을 선택하세요");
        if (idx == -1) continue;

        act.index = idx;
        if (c == 1) act.type = InventoryAction::Use;
        else if (c == 2) act.type = InventoryAction::Equip;

        return act;
    }
}

// ===========================
// ✅ 상점 행동 처리(구매/판매)
// ===========================
void UIManager::doBuy(Shop& shop, Character& player, Inventory& inv) {
    int max = shop.size() - 1;
    if (max < 0) { grid.waitAnyKey("상점에 아이템이 없습니다. "); return; }

    int idx = grid.askInt("구매할 아이템 번호: ", 0, max);

    Item* peek = shop.peek(idx);
    if (peek && player.getGold() < peek->getPrice()) {
        grid.waitAnyKey("골드가 부족합니다. ");
        return;
    }

    Item* bought = shop.buyItem(&player, idx);
    if (!bought) { grid.waitAnyKey("구매 실패. "); return; }

    inv.add(bought);
    std::cout << "인벤토리에 추가됨: " << bought->getName() << "\n";
    grid.showItemArt(bought->getName());
    grid.waitAnyKey();
}

void UIManager::doSell(Shop& shop, Character& player, Inventory& inv) {
    // 판매 전용 라벨(판매가 표기)
    auto labelInvSell = [](int idx, Item* it) {
        if (!it) return "[" + std::to_string(idx) + "] (EMPTY)";

        int basePrice = it->getPrice();
        int sellPrice = (basePrice * 60) / 100;

        std::string label = "[" + std::to_string(idx) + "] " + it->getName();
        if (basePrice <= 0 || sellPrice <= 0) label += " (판매불가)";
        else label += " (판매가 " + std::to_string(sellPrice) + "G)";
        return label;
        };

    clearScreen();
    grid.printItemGrid(inv.getItems(), "💰 판매할 아이템 선택", labelInvSell, 3, 0, 8);
    std::cout << "----------------------------------------\n";
    std::cout << "장착 상태:\n";
    inv.listEquipped();

    int idx = grid.askInventoryIndexOrCancel(inv, "\n판매할 아이템을 선택하세요");
    if (idx == -1) return;

    const auto& items = inv.getItems();
    Item* peek = (idx >= 0 && idx < (int)items.size()) ? items[idx] : nullptr;

    if (!peek) { grid.waitAnyKey("판매 실패. "); return; }

    int basePrice = peek->getPrice();
    int sellPrice = (basePrice * 60) / 100;

    if (basePrice <= 0 || sellPrice <= 0) {
        grid.waitAnyKey("이 아이템은 판매할 수 없습니다. ");
        return;
    }

    std::cout << "선택한 아이템: " << peek->getName() << "\n";
    std::cout << "판매가: " << sellPrice << "G\n";

    Item* sold = inv.extractAt(idx);
    if (!sold) { grid.waitAnyKey("판매 실패. "); return; }

    shop.sellItem(&player, sold);
    delete sold;

    std::cout << "판매 완료! +" << sellPrice << "G\n";
    grid.waitAnyKey();
}

void UIManager::showLevelUp(int currentLevel, int levelUps) {
    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "\n";
    std::cout << "                     🎉  LEVEL UP!  🎉\n";
    std::cout << "\n";
    std::cout << "            레벨이 상승했습니다!  ( +" << levelUps << " )\n";
    std::cout << "                 현재 레벨 : Lv." << currentLevel << "\n";
    std::cout << "\n";
    std::cout << "============================================================\n";
}

BattleMode UIManager::askBattleMode() {
    clearScreen();
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║                  ⚔️  전투 모드 선택  ⚔️                     ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "║   1) 수동 전투 (직접 조작)                                  ║\n";
    std::cout << "║   2) 자동 전투 (AI 자동 진행)                               ║\n";
    std::cout << "║                                                            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";

    int choice = 0;

    while (true) {
        std::cout << "선택 (1 또는 2): ";
        std::cin >> choice;

        // 입력 실패(문자 등)
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "잘못된 입력입니다. 숫자를 입력해주세요.\n";
            continue;
        }

        if (choice == 1) return BattleMode::Manual;
        if (choice == 2) return BattleMode::Auto;

        std::cout << "1 또는 2만 선택 가능합니다.\n";
    }
}