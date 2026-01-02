#pragma once

#include <string>
#include <map>
#include "Monster.h"
#include "Item.h"
#include "Character.h"

struct MonsterData {
    std::string name;
    std::string info;

    MonsterData(const std::string& n, const std::string& i)
        : name(n), info(i) {
    }
};

enum class GameState {
    MAIN_MENU,
    CHARACTER_CREATION,
    PHASE_1,
    PHASE_2,
    PHASE_3,
    SHOP,
    BATTLE,
    BOSS_BATTLE,
    PHASE_CLEAR,
    GAME_OVER,
    ENDING
};

enum class PhaseType {
    NONE,
    PHASE_1,
    PHASE_2,
    PHASE_3
};

class GameManager
{
private:
    Character* player;

    GameState currentState;
    PhaseType currentPhase = PhaseType::NONE;

    bool isRunning;
    int currentRound;
    int totalRoundsInPhase;

    std::vector<MonsterData> phase1Monsters;
    std::vector<MonsterData> phase2Monsters;
    std::vector<MonsterData> phase3Monsters;
    std::map<std::string, int> mobKillCounts;

    // 상태별 메서드들
    void showMainMenu();
    void createCharacter();
    void startPhase(PhaseType phase);
    void runShop();
    void runBattle();
    void runBossBattle();
    void showPhaseClearScreen();
    void showGameOverScreen();
    void showEndingScreen();
    void retryCurrentBattle();

    // 유틸리티 메서드들
    Monster* generateMonster();
    Monster* generateBoss();
    void handlePlayerDeath();
    void applyBuffItems();
    bool askShopVisit();
    void clearScreen();
    void waitForInput();

public:
    // call Main Menu에서 게임시작을했을때
    // 여기로 불러와서 게임진행
    // 처음오면 캐릭터생성
    void play();

    // 전투
    void battle() {};

    // 상점 방문
    // uiManager에서 상점 품목보여주고
    // 실제입력 여기서 받고
    // 해당하는 아이템의 인덱스로 상점의 buyItem불러옴
    void EnterShop() {
    
    }

    Item* generateDroItems(const Monster& monster) {
    
    }

    // 게임매니저에서 이런거 출력하는게 나은지
    void diplayKillCount() {};
    void displayCharacterStatus() {};
};


