#pragma once
#include <string>
#include <vector>
#include <map>
#include "UIGrid.h"
#include "Types.h"

class MapAsciiArtRepository;
class Shop;
class Inventory;
class Item;
class Character;
class Monster;

class UIManager {
public:
    UIManager(MapAsciiArtRepository& artRepo);
    // GameManager
    // 화면 제어
    void clearScreen();
    void waitForKeyPress();

    // 메인 화면
    void showCharacterCreationScreen();

    // 페이즈 입장 화면
    void showPhase1Entry();
    void showPhase2Entry();
    void showPhase3Entry();

    // 전투 관련
    void showMonsterEncounter(const std::string& monsterName);
    void showBossEncounter(const std::string& bossName);

    // 상점
    void showShopMenu(int playerGold);

    // 선택 화면
    int askShopVisit();  // 1: 상점 방문, 2: 즉시 전투
    char askRetryOrQuit();  // '1': 재도전, '2': 종료

    // 사망/재시작
    void showDeathScreen();
    void showRetryScreen(const std::string& phaseName, int currentRound, int totalRounds);

    // 엔딩
    void showEndingScreen(
        const std::string& playerName,
        int level,
        int gold,
        int health,
        int maxHealth,
        int attack,
        const std::map<std::string, int>& mobKillCounts
    );

    // 입력 처리
    std::string getPlayerName();
    void showPlayerStats(std::string name, int health, int maxHealth, int attack, int level, int gold);

    // BattleService
    // 전투 UI
    void showBattleStart(bool isBoss);
    void showMonsterInfo(const std::string& name, const std::string& info,
        int hp, int maxHp, int attack);
    void showTurnNumber(int turnNumber);
    void showBattleStatus(const std::string& playerName, int playerHp, int playerMaxHp, int playerAtk,
        const std::string& monsterName, int monsterHp, int monsterMaxHp, int monsterAtk);

    // 플레이어 턴
    char showPlayerTurnMenuAndGetChoice();
    void showPlayerAttackResult(const std::string& playerName, const std::string& monsterName,
        int damage, bool isMonsterDead, bool isCritical);
    void showItemActionScreen(
        const std::string& title,
        const std::string& itemName,
        const std::string& effectText
    );
    void showInvalidInput();

    // 몬스터 턴
    void showMonsterTurnHeader(const std::string& monsterName);
    void showMonsterAttackResult(const std::string& monsterName, const std::string& playerName,
        int damage, bool isPlayerDead, int remainingHp, int maxHp);

    // 전투 결과
    void showVictoryScreen(bool isBoss,
        int gold,
        int exp,
        const std::vector<std::string>& droppedItems);
    void showDefeatScreen();

    // 전투 로그
    void showBattleLog(const std::vector<std::string>& battleLog);

    void runShop(Shop& shop, Character& player, Inventory& inv);
    InventoryAction askInventoryAction(Inventory& inv);

    void showQuizRound(int current, int total);
    std::string askQuizAnswer(const std::string& question);
    void showQuizResult(bool correct, const std::string& message, int playerScore, int bossScore);

    void showMessage(const std::string& message);
    //레벨업 
    void showLevelUp(int currentLevel, int levelUps);
    //전투 모드
    BattleMode askBattleMode();
private:
    void drawBoxedTitle(const std::string& title, const std::string& subtitle);
    void drawSeparator();
       // 상점의 실제 “행동 처리”는 UIManager에 남겨두는게 깔끔함(통솔)
    void doBuy(Shop& shop, Character& player, Inventory& inv);
    void doSell(Shop& shop, Character& player, Inventory& inv);
    MapAsciiArtRepository& artRepo;

    // 그리드 UI/입력 유틸 담당 객체
    UIGrid grid;



};