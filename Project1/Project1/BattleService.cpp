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

void BattleService::playerTurn() {
    bool endTurn = false;
    
    while (!endTurn) {
        std::cout << "\n[플레이어 턴]\n";
        std::cout << "1. 공격\n";
        std::cout << "2. 아이템 사용\n";
        std::cout << "선택 : ";

        char choice = _getch();
        std::cout << choice << std::endl;

        switch (choice) {
        case '1': {
            // 공격
            int damage = player->getAttack();
            monster->takeDamage(damage);

            std::cout << "\n⚔️  " << player->getName() << "의 공격!\n";
            std::cout << "→ " << monster->getName() << "에게 " << damage << " 데미지!\n";

            if (!monster->isAlive()) {
                std::cout << "→ " << monster->getName() << "을(를) 쓰러뜨렸다!\n";
            }

            addLog(player->getName() + "의 공격: " + std::to_string(damage) + " 데미지");
            endTurn = true;
            break;
        }
        case '2': {
            // 아이템 사용
            endTurn = true;
            break;
        }
        default:
            std::cout << "입력이 잘못되었습니다!\n";
            break;
        }
    }
}

void BattleService::monsterTurn() {
    std::cout << "\n[" << monster->getName() << " 턴]\n";

    int damage = monster->getAttack();
    player->takeDamage(damage);

    std::cout << "💥 " << monster->getName() << "의 공격!\n";
    std::cout << "→ " << player->getName() << "이(가) " << damage << " 데미지를 받았다!\n";

    if (!player->isAlive()) {
        std::cout << "→ 체력이 모두 소진되었다...\n";
    }

    addLog(monster->getName() + "의 공격: " + std::to_string(damage) + " 데미지");
}

void BattleService::displayBattleStatus() {
    std::cout << "\n┌────────────────────────────────────────┐\n";
    std::cout << "│ 플레이어: " << player->getName() << "\n";
    std::cout << "│ HP: " << player->getHealth() << " / " << player->getMaxHealth()
        << " | 공격력: " << player->getAttack() << "\n";
    std::cout << "├────────────────────────────────────────┤\n";
    std::cout << "│ 적: " << monster->getName() << "\n";
    std::cout << "│ HP: " << monster->getHealth() << " / " << monster->getMaxHealth()
        << " | 공격력: " << monster->getAttack() << "\n";
    std::cout << "└────────────────────────────────────────┘\n";
}

void BattleService::addLog(const std::string& message) {
    battleLog.push_back(message);
}

void BattleService::displayLog() {
    std::cout << "\n=== 전투 로그 ===\n";
    for (const auto& log : battleLog) {
        std::cout << log << "\n";
    }
}