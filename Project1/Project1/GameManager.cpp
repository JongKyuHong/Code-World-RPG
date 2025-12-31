#include "GameManager.h"
#include <string>
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

    // 3페이즈에서는 변주주기
    while (currentRound < totalRoundsInPhase) {
        if (askShopVisit()) {
            currentState = GameState::SHOP;
            return;
        }
        currentState = GameState::BATTLE;
        return;
    }

    currentState = GameState::BOSS_BATTLE;
}

void GameManager::runBattle() {
    Monster* monster = generateMonster();

    // std::cout << 전투관련 내용들 << std::endl;

    // 전투 전 버프적용, 자동전투한다면 구현
    //applyBuffItems();

    // 전투 시작 battleService
    //battleService->startBattle(player, monster);
}