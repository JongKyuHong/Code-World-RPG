#pragma once

#include <string>
#include <map>
#include "Monster.h"
#include "Item.h"
#include "Character.h"
#include "UIManager.h"
#include "Types.h"        // PhaseType, EquipSlot 등은 여기서 가져온다고 가정
#include "UIManager.h"    // InventoryAction, UIManager
#include "ItemContext.h"  // ctx
#include "BattleRewardService.h" 
#include "BattleService.h" // BattleMode

// forward declarations (헤더에서는 가볍게)
class Character;
class Monster;

class EffectSystem;
class EffectManager;

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
    INVENTORY,
    BATTLE,
    BOSS_BATTLE,
    PHASE_CLEAR,
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
    ItemContext& ctx;
    UIManager uiManager;
    Character* player;
    BattleMode battleMode_ = BattleMode::Manual;
    GameState currentState;
    PhaseType currentPhase = PhaseType::NONE;

    bool isRunning;
    int currentRound;
    int totalRoundsInPhase;
    BattleRewardService& rewardService; // 오류 해결: BattleRewardService 선언 필요
    std::vector<MonsterData> phase1Monsters;
    std::vector<MonsterData> phase2Monsters;
    std::vector<MonsterData> phase3Monsters;
    std::map<std::string, int> mobKillCounts;

    //Effect는 incomplete type 문제 때문에 포인터로 보유
    EffectSystem* effectSystem = nullptr;
    EffectManager* effectManager = nullptr;

    // 상태별 메서드들
    void showMainMenu();
    void createCharacter();
    void startPhase(PhaseType phase);
    void runShop();
    void runInventory();
    void runBattle();
    void runBossBattle();
    void showPhaseClearScreen();
    void showEndingScreen();
    void retryCurrentBattle();

    // 유틸리티 메서드들
    Monster* generateMonster();
    Monster* generateBoss();
    void handlePlayerDeath();
    void clearScreen();

public:
    GameManager(ItemContext& ctx, BattleRewardService& rewardService);
    ~GameManager() = default; 

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


