#include "BattleService.h"
#include "RewardManager.h"
#include <iostream>
#include <conio.h>
#include <cstdlib>

bool BattleService::rollCritical()
{
    // 크리티컬 (30%)
    if (rand() % 10 < 3)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int BattleService::applyCriticalMultiplier(int baseDamage, bool isCritical)
{
    if (isCritical == true)
    {
        // 크리티컬이면 2배로 뻥튀기
        return baseDamage * 2;
    }
    else
    {
        // 아니면 원래 데미지 그대로
        return baseDamage;
    }
}

BattleResult BattleService::battle(Character* p, Monster* m) {
    player = p;
    monster = m;
    
    RewardManager* rewardManager = RewardManager::getInstance();
    BattleResult result = rewardManager->generateRewards(monster);

    uiManager.showBattleStart(monster->isBossMonster());

    // 적 정보
    uiManager.showMonsterInfo(
        monster->getName(),
        monster->getMobInfo(),
        monster->getHealth(),
        monster->getMaxHealth(),
        monster->getAttack()
    );


    // 애니메이션 구현한다면 들어가는 자리
    uiManager.waitForKeyPress();

    int turnCount = 0;

    // 전투
    while (player->isAlive() && monster->isAlive()) {
        turnCount++;

        uiManager.showTurnNumber(turnCount);

        playerTurn();

        if (!monster->isAlive()) {
            break;
        }

        monsterTurn();

        if (!player->isAlive()) {
            break;
        }

        // 애니메이션 구현한다면 들어가는 자리
        uiManager.waitForKeyPress();
    }

    result.turnCount = turnCount;

    if (player->isAlive()) {
        result.playerWon = true;

        uiManager.showVictoryScreen(monster->isBossMonster(), result.goldEarned, result.expEarned);

        // 보상 지급
        player->addGold(result.goldEarned);
        player->addExperience(result.expEarned);

        // 몬스터 onDeath 콜백
        monster->onDeath();
    } else {
        result.playerWon = false;

        uiManager.showDefeatScreen();
    }

    // 애니메이션 구현한다면 들어가는 자리
    uiManager.waitForKeyPress();

    return result;
}

void BattleService::playerTurn() {
    bool endTurn = false;
    
    while (!endTurn) {
        displayBattleStatus();

        char choice = uiManager.showPlayerTurnMenuAndGetChoice();

        switch (choice) {
        case '1': {
            displayBattleStatus();
            // 공격
            int damage = player->getAttack();
            monster->takeDamage(damage);

            uiManager.showPlayerAttackResult(
                player->getName(),
                monster->getName(),
                damage,
                !monster->isAlive()
            );

            addLog(player->getName() + "의 공격: " + std::to_string(damage) + " 데미지");
            
            endTurn = true;
            break;
        }
        case '2': {
            // 아이템 사용
            uiManager.showItemUseScreen();
            endTurn = true;
            break;
        }
        default:
            uiManager.showInvalidInput();
            break;
        }
    }
}

void BattleService::monsterTurn() {
    displayBattleStatus();

    uiManager.showMonsterTurnHeader(monster->getName());

    int damage = monster->getAttack();
    player->takeDamage(damage);

    uiManager.showMonsterAttackResult(
        monster->getName(),
        player->getName(),
        damage,
        !player->isAlive(),
        player->getHealth(),
        player->getMaxHealth()
    );

    addLog(monster->getName() + "의 공격: " + std::to_string(damage) + " 데미지");
}

void BattleService::displayBattleStatus() {
    uiManager.showBattleStatus(
        player->getName(),
        player->getHealth(),
        player->getMaxHealth(),
        player->getAttack(),
        monster->getName(),
        monster->getHealth(),
        monster->getMaxHealth(),
        monster->getAttack()
    );
}

void BattleService::addLog(const std::string& message) {
    battleLog.push_back(message);
}

void BattleService::displayLog() {
    uiManager.showBattleLog(battleLog);
}

