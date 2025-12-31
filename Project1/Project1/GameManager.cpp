#include "GameManager.h"
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
    std::cout << "메인메뉴" << std::endl;
}

