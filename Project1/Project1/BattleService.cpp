#include "BattleService.h"
#include <iostream>
#include <conio.h>

BattleResult BattleService::battle(Character* p, Monster* m) {
    player = p;
    monster = m;
    
    BattleResult result;
    result.monsterName = monster->getName();
    result.isBossKill = monster->isBossMonster();

    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════╗\n";
    if (monster->isBossMonster()) {
        std::cout << "║          ⚔️  보스 전투 시작! ⚔️         ║\n";
    } else {
        std::cout << "║          ⚔️  전투 시작! ⚔️             ║\n";
    }
    std::cout << "╚════════════════════════════════════════╝\n";


    std::cout << "\n적: " << monster->getName() << " (" << monster->getMobInfo() << ")\n";
    std::cout << "HP: " << monster->getHealth() << " / 공격력: " << monster->getAttack() << "\n";

    std::cout << "\n계속하려면 아무 키나 누르세요...";
    _getch();

    int turnCount = 0;

    // 전투
    while (player->isAlive() && monster->isAlive()) {
        turnCount++;

        std::cout << "\n";
        std::cout << "━━━━━━━━━━━━━ Turn " << turnCount << " ━━━━━━━━━━━━━\n";

        playerTurn();

        if (!monster->isAlive()) {
            break;
        }

        monsterTurn();

        if (!player->isAlive()) {
            break;
        }

        std::cout << "\n계속하려면 아무 키나 누르세요...";
        _getch();
    }

    result.turnCount = turnCount;

    if (player->isAlive()) {
        result.playerWon = true;

        std::cout << "\n";
        std::cout << "╔════════════════════════════════════════╗\n";
        std::cout << "║            🎉 승리! 🎉                ║\n";
        std::cout << "╚════════════════════════════════════════╝\n";

        if (monster->isBossMonster()) {
            std::cout << "\n🏆 보스를 처치했습니다! 🏆\n";
        }

        std::cout << "\n획득 보상:\n";
        std::cout << "💰 골드: " << result.goldEarned << " G\n";
        std::cout << "⭐ 경험치: " << result.expEarned << " EXP\n";

        // 보상 지급
        player->addGold(result.goldEarned);
        player->addExperience(result.expEarned);

        // 몬스터 onDeath 콜백
        monster->onDeath();
    } else {
        result.playerWon = false;

        std::cout << "\n";
        std::cout << "╔════════════════════════════════════════╗\n";
        std::cout << "║            💀 패배... 💀              ║\n";
        std::cout << "╚════════════════════════════════════════╝\n";

        std::cout << "\n당신은 쓰러졌습니다...\n";
    }

    std::cout << "\n계속하려면 아무 키나 누르세요...";
    _getch();

    return result;
}