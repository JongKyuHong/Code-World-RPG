#include "GameManager.h"
#include "Monster.h"
#include <string>
#include <vector>
#include <iostream>

void GameManager::play() {
    isRunning = true;
    currentState = GameState::MAIN_MENU;

    while (isRunning) {
        switch (currentState) {
        case GameState::MAIN_MENU:
            showMainMenu();
            break;

        case GameState::CHARACTER_CREATION:
            createCharacter();
            currentState = GameState::PHASE_1;
            break;

        case GameState::PHASE_1:
            startPhase(PhaseType::PHASE_1);
            break;

        case GameState::PHASE_2:
            startPhase(PhaseType::PHASE_2);
            break;

        case GameState::PHASE_3:
            startPhase(PhaseType::PHASE_3);
            break;

        case GameState::SHOP:
            runShop();
            currentState = GameState::BATTLE;
            break;

        case GameState::BATTLE:
            runBattle();
            break;

        case GameState::BOSS_BATTLE:
            runBossBattle();
            break;

        case GameState::PHASE_CLEAR:
            showPhaseClearScreen();
            break;

        case GameState::GAME_OVER:
            showGameOverScreen();
            break;

        case GameState::ENDING:
            showEndingScreen();
            isRunning = false;
            break;
        }
    }
}

void GameManager::showMainMenu() {
    // std::cout << "메인메뉴" << std::endl;
}

void GameManager::createCharacter() {
    std::string InpName;
    std::cin >> InpName;
    player = Character::getInstance(InpName);
    // std::cout << "환영합니다 용사 ... 님" << std::endl;
}

void GameManager::startPhase(PhaseType phase) {
    if (currentPhase != phase) {
        currentPhase = phase;
        currentRound = 0;

        switch (phase) {
        case PhaseType::PHASE_1:
            // std::cout << "변수의 숲" << std::endl;
            phase1Monsters = { "VariableSlime", "ConstantGolem", "TypeSpirit"
            ,"InitWisp", "DeclareShadow" };
            totalRoundsInPhase = 5;
            break;
        case PhaseType::PHASE_2:
            // std::cout << "클래스의 성" << std::endl;
            phase1Monsters = { "ClassMimic", "ObjectOrc", "MemberSpider"
            ,"FunctionMage", "ConstructGargoyle", "DestructReaper" };
            totalRoundsInPhase = 6;
            break;
        case PhaseType::PHASE_3:
            // std::cout << "디자인 패턴의 탑" << std::endl;
            phase1Monsters = { "SingletonKing", "DecoratorKnight", "ObserverBeholder"
            ,"StrategyGeneral" };
            totalRoundsInPhase = 4;
            break;
        }
    }
    // 상점 방문 여부
    if (askShopVisit()) {
        currentState = GameState::SHOP;
    } else {
        currentState = GameState::BATTLE;
    }
}

void GameManager::runBattle() {
    Monster* monster = generateMonster();

    // std::cout << 전투관련 내용들 << std::endl;

    // 전투 전 버프적용, 자동전투한다면 구현
    //applyBuffItems();

    // 실제 전투
    // battleService->battle();

    // 전투정보 받아와서 처리
    //if (player->isAlive()) {
    //    // mob킬수저장
    //    // mobKillCount[monster->getName()]++;
    //    
    //    // reward 받음
    //    // battleService->battle()에서 처리?

    //    delete monster;

    //    // 다음라운드 실행
    //    currentRound++;
    //    if (currentRound >= totalRoundsInPhase) {
    //        currentState = GameState::BOSS_BATTLE;
    //    } else {
    //        // 상점에 방문하는가?
    //        if (askShopVisit()) {
    //            currentState = GameState::SHOP;
    //        } else {
    //            currentState = GameState::BATTLE;

    //        }
    //    }
    //} else {
    //    delete monster;
    //    handlePlayerDeath();
    //}
}

//Monster* GameManager::generateMonster() {
//    std::vector<std::string>* monsterNames;
//
//    switch (currentPhase) {
//    case PhaseType::PHASE_1:
//        monsterNames = &phase1Monsters;
//        break;
//    case PhaseType::PHASE_2:
//        monsterNames = &phase2Monsters;
//        break;
//    case PhaseType::PHASE_3:
//        monsterNames = &phase3Monsters;
//        break;
//    default:
//        break;
//    }
//
//    // 현재 라운드에 맞는 몬스터
//    std::string name = (*monsterNames)[currentRound % monsterNames->size()];
//    
//
//    // 스텟은 추후에 변경
//    int baseHealth = (currentRound + 1) * 20;
//    int baseAttack = (currentRound + 1) * 5;
//    
//    // 추후에 Monster 추상클래스를 상속하는 NormalMonster클래스 추가 
//    return new NormalMonster();
//}


// 추후에 수정
//Monster* GameManager::generateBoss() {
//    std::string bossName;
//    float multiplier = 0.0f;
//    bool isFinalBoss = false;
//
//    switch (currentPhase) {
//    case PhaseType::PHASE_1:
//        bossName = "PointerLich";
//        multiplier = 1.2f;
//        break;
//    case PhaseType::PHASE_2:
//        bossName = "PolyDragon";
//        multiplier = 1.2f;
//        break;
//    case PhaseType::PHASE_3:
//        bossName = "TeamProjectDevil";
//        multiplier = 1.5f;
//        isFinalBoss = true;
//        break;
//    }
//
//    // 스텟은 우선 요구사항 따라서
//    int baseHealth = (currentRound + 1) * 20;
//    int baseAttack = (currentRound + 1) * 5;
//
//    int health = static_cast<int>(baseHealth * multiplier);
//    int attack = static_cast<int>(baseAttack * multiplier);
//
//    return new Monster();
//}

void GameManager::runShop() {}

void GameManager::runBossBattle() {}

void GameManager::showPhaseClearScreen() {}

void GameManager::showGameOverScreen() {}

void GameManager::showEndingScreen() {}

bool GameManager::askShopVisit() { return false; }