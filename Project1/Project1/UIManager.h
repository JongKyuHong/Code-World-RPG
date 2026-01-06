#pragma once
#include <string>
#include <vector>
#include <map>

class Character;
class Monster;

class UIManager {
public:
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
        int damage, bool isMonsterDead);
    void showItemUseScreen();
    void showInvalidInput();

    // 몬스터 턴
    void showMonsterTurnHeader(const std::string& monsterName);
    void showMonsterAttackResult(const std::string& monsterName, const std::string& playerName,
        int damage, bool isPlayerDead, int remainingHp, int maxHp);

    // 전투 결과
    void showVictoryScreen(bool isBoss, int gold, int exp);
    void showDefeatScreen();

    // 전투 로그
    void showBattleLog(const std::vector<std::string>& battleLog);


    void showQuizRound(int current, int total);
    std::string askQuizAnswer(const std::string& question);
    void showQuizResult(bool correct, const std::string& message, int playerScore, int bossScore);

    void showMessage(const std::string& message);
private:
    void drawBoxedTitle(const std::string& title, const std::string& subtitle);
    void drawSeparator();
};