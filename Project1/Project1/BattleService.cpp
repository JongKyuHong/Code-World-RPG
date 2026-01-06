#include "BattleService.h"
#include "RewardManager.h"
#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <random> 
#include <algorithm>

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

    if (monster->isBossMonster()) {
        return bossBattle();
    } else {
        return normalBattle();
    }

    return result;
}

BattleResult BattleService::normalBattle() {
    int turnCount = 0;

    // 턴제 전투
    while (player->isAlive() && monster->isAlive()) {
        turnCount++;

        uiManager.showTurnNumber(turnCount);

        playerTurn();

        if (!monster->isAlive()) break;

        monsterTurn();

        if (!player->isAlive()) break;

        uiManager.waitForKeyPress();
        uiManager.clearScreen();
    }

    BattleResult result = RewardManager::getInstance()->generateRewards(monster);
    result.turnCount = turnCount;
    result.playerWon = player->isAlive();
    result.monsterName = monster->getName();

    if (result.playerWon) {
        uiManager.showVictoryScreen(false, result.goldEarned, result.expEarned);
        player->addGold(result.goldEarned);
        player->addExperience(result.expEarned);
        monster->onDeath();
    } else {
        uiManager.showDefeatScreen();
    }

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

std::string toLower(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        if (c >= 'A' && c <= 'Z') {
            c += 32;
        }
    }
    return result;
}

BattleResult BattleService::bossBattle() {
    uiManager.showBossEncounter(monster->getName());

    std::string bossName = monster->getName();
    std::vector<std::pair<std::string, std::string>> quizzes = getBossQuizzes(bossName);

    int maxRounds = 0;
    if (bossName.find("PointerLich") != std::string::npos) {
        maxRounds = 3;
    } else if (bossName.find("PolymorphicDragon") != std::string::npos) {
        maxRounds = 4;
    } else if (bossName.find("TeamProjectDevil") != std::string::npos) {
        maxRounds = 5;
    } else {
        maxRounds = 3;
    }

    if ((int)quizzes.size() < maxRounds) {
        maxRounds = quizzes.size();
    }

    int playerCorrect = 0;

    for (int i = 0; i < maxRounds; ++i) {
        uiManager.showQuizRound(i + 1, maxRounds);

        std::string question = quizzes[i].first;
        std::string correct = quizzes[i].second;

        std::string answer = uiManager.askQuizAnswer(question);

        // 단순 비교
        bool isCorrect = (answer == correct);

        if (isCorrect) {
            playerCorrect++;
            uiManager.showQuizResult(true, "정답!", playerCorrect, maxRounds);
            if (i < maxRounds - 1) Sleep(500);
        } else {
            uiManager.showQuizResult(false, "틀렸습니다! 정답: " + correct, playerCorrect, maxRounds);
            uiManager.showMessage("💀 퀴즈 실패! 보스의 저주에 당했습니다... 💀\n😵 캐릭터 사망! GAME OVER");
            player->takeDamage(player->getMaxHealth());
            BattleResult failResult{ false, i + 1 };
            Sleep(500);
            return failResult;
        }
    }

    BattleResult result = RewardManager::getInstance()->generateRewards(monster);
    result.playerWon = true;
    result.turnCount = maxRounds;
    uiManager.showVictoryScreen(monster, result.goldEarned, result.expEarned);
    return result;
}


std::vector<std::pair<std::string, std::string>> BattleService::getBossQuizzes(const std::string& bossName) {
    std::vector<std::pair<std::string, std::string>> quizzes;

    if (bossName.find("PointerLich") != std::string::npos) {
        quizzes = {
            {"int* ptr;의 * 기호 의미는?\n   1) 곱셈연산자\n   2) 포인터선언\n   3) 참조연산자", "2"},
            {"int x=5; int* p=&x; *p=10; 실행 후 x값은?\n   1) 5\n   2) 10\n   3) 15", "2"},
            {"int x=10; int* p=&x; cout << *p; 출력값은?\n   1) 10\n   2) 0\n   3) 주소값", "1"}
        };
    } else if (bossName.find("PolyDragon") != std::string::npos) {
        quizzes = {
            {"class A { virtual void f(); }; 'virtual' 키워드 역할은?\n   1) 상속\n   2) 템플릿\n   3) 가상함수", "3"},
            {"class Parent { void show(); }; class Child : public Parent { void show(); }; 관계는?\n   1) 오버로드\n   2) 오버라이드\n   3) 다중상속", "2"},
            {"Base* obj = new Derived(); 이때 obj의 실제 타입은?\n   1) Base\n   2) Derived\n   3) void*", "2"},
            {"class Animal { virtual ~Animal() {} }; 소멸자에 virtual을 쓰는 이유는?\n   1) 메모리누수방지\n   2) 성능향상 \n   3) 컴파일오류방지", "1"}
        };
    } else if (bossName.find("TeamProjectDevil") != std::string::npos) {
        quizzes = {
            {"GitHub에서 코드 병합을 요청하는 기능은?\n   1) Commit\n   2) PR\n   3) Issue", "2"},
            {"class Singleton { static Singleton* instance; }; instance는 몇 개?\n   1) 0\n   2) 1개\n   3) 무한대", "2"},
            {"Observer 패턴에서 상태 변경 시 누가 통보?\n   1) Observer\n   2) Context\n   3) Subject", "3"},
            {"Git에서 브랜치를 합치는 명령어는?\n   1) merge\n   2) push\n   3) commit", "1"},
            {"데코레이터 패턴의 핵심 비유로 가장 적절한 것은?\n   1) 붕어빵 틀에서 붕어빵 찍어내기\n   2) 마트료시카 인형처럼 객체 안에 객체를 계속 감싸기\n   3) 로봇 조립처럼 부품을 조합하기", "2"}
        };
    }

    return quizzes;
}