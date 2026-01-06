#include "BattleService.h"
#include "RewardManager.h"
#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <random> 
#include <algorithm>

#include "Inventory.h"   // inv_->useItem 호출 때문에 필요
#include <chrono>     // 자동 모드에서 딜레이 주고 싶으면 사용
#include <thread>
#include <string>

namespace { //자동 전투 시 아이템 사용 효과 적용 메세지 처리를 위해서(스탯 비교)
    struct StatSnapshot {
        int hp;
        int maxHp;
        int atk;
    };

    StatSnapshot snap(Character* c) {
        return {
            c->getHealth(),
            c->getMaxHealth(),
            c->getAttack()
        };
    }

    std::string diffToText(const StatSnapshot& before, const StatSnapshot& after) {
        std::string out;

        auto append = [&](const std::string& name, int b, int a) {
            int d = a - b;
            if (d == 0) return;
            if (!out.empty()) out += " | ";
            out += name + (d > 0 ? " +" : " ") + std::to_string(d);
            };

        append("ATK", before.atk, after.atk);
        append("MAXHP", before.maxHp, after.maxHp);
        append("HP", before.hp, after.hp);

        if (out.empty()) out = "변화 없음";
        return out;
    }
}

void BattleService::tickEndOfTurn()
{
    if (!player) return;
    effectSystem.updateActiveEffects(*player, effectManager);
}

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

    // 자동전투 컨트롤러 초기화 (자동 모드에서만 의미 있게 사용됨)
    autoCtrl.onBattleStart(monster->isBossMonster());
    
    RewardManager* rewardManager = RewardManager::getInstance();
    BattleResult result = rewardManager->generateRewards(monster);

    // ✅ 모드에 따라 wait 동작 통합
    auto waitIfManual = [&]() {
        if (mode_ == BattleMode::Manual) {
            uiManager.waitForKeyPress();
        }
        // 자동 모드에서 너무 빨리 넘어가면 아래 주석 해제해서 딜레이 줄 수 있음
        
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(2500));
        }
        
    };

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
    waitIfManual();

    if (monster->isBossMonster()) {
        return bossBattle();
    } else {
        return normalBattle();
    }

    return result;
}
void BattleService::waitNextBeat() {
    if (mode_ == BattleMode::Manual) {
        uiManager.waitForKeyPress();
    }
    else {
        // 자동 모드 딜레이 정책은 여기 한 곳에서만 관리
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    }
}
BattleResult BattleService::normalBattle() {
    int turnCount = 0;

    // 턴제 전투
    while (player->isAlive() && monster->isAlive()) {
        turnCount++;

        // ✅ 자동 컨트롤러 턴 훅 (수동이어도 호출해도 무방)
        autoCtrl.onTurnStart(turnCount);

        uiManager.showTurnNumber(turnCount);

        playerTurn();

        if (!monster->isAlive()) {
            tickEndOfTurn();
            break;
        }

        monsterTurn();

        tickEndOfTurn();

        if (!player->isAlive()) break;

        waitNextBeat();
        uiManager.clearScreen();
    }
   // 전투 종료 시 지속 효과 정리
    effectSystem.clearAll(*player, effectManager);

    BattleResult result = RewardManager::getInstance()->generateRewards(monster);
    result.turnCount = turnCount;
    result.playerWon = player->isAlive();
    result.monsterName = monster->getName();
    rewardService.applyDrops(monster, result);

    if (result.playerWon) {
        uiManager.showVictoryScreen(
            monster->isBossMonster(),
            result.goldEarned,
            result.expEarned,
            result.droppedItemNames
        );
        RewardManager::getInstance()->applyRewards(player, result);
        if (result.levelUps > 0) {
            uiManager.showLevelUp(
                player->getLevel(),
                result.levelUps
            );
        };
        waitNextBeat();
        monster->onDeath();
    } else {
        uiManager.showDefeatScreen();
    }

    waitNextBeat();
    return result;
}


void BattleService::playerTurn()
{
    if (mode_ == BattleMode::Auto) playerTurnAuto();
    else playerTurnManual();
}

void BattleService::playerTurnManual()
{
    bool endTurn = false;

    while (!endTurn) {
        displayBattleStatus();

        char choice = uiManager.showPlayerTurnMenuAndGetChoice();

        switch (choice) {
        case '1': {
            displayBattleStatus();

            int damage = player->getAttack();
            // 1. 크리티컬 여부 결정 (30% 확률)
            bool isCritical = rollCritical();
            if (isCritical) {
                // 3. 크리티컬 배수 적용 (성공 시 2배)
                int finalDamage = applyCriticalMultiplier(damage, isCritical);

                // 4. 몬스터에게 최종 데미지 적용
                monster->takeDamage(finalDamage);
                // 5. 콘솔 UI에 결과 출력 (마지막 인자로 isCritical 전달)
                uiManager.showPlayerAttackResult(
                    player->getName(),
                    monster->getName(),
                    finalDamage,
                    !monster->isAlive(),
                    isCritical // UIManager에서 크리티컬 문구를 띄우기 위해 추가
                );
                std::string logMsg = player->getName() + "의 공격: " + std::to_string(finalDamage) + " 데미지";
                if (isCritical) {
                    logMsg += " (★크리티컬 히트!★)";
                }
                addLog(logMsg);
            }
            else {
                monster->takeDamage(damage);
                uiManager.showPlayerAttackResult(
                    player->getName(),
                    monster->getName(),
                    damage,
                    !monster->isAlive(),
                    isCritical // UIManager에서 크리티컬 문구를 띄우기 위해 추가
                );
                addLog(player->getName() + "의 공격: " + std::to_string(damage) + " 데미지");
            }

            endTurn = true;
            break;
        }
        case '2': {
            // 인벤토리 사용(수동 모드용 콜백)
            if (openInventoryCb) openInventoryCb();
            else uiManager.showInvalidInput();

            endTurn = true;
            break;
        }
        default:
            uiManager.showInvalidInput();
            break;
        }
    }
}

void BattleService::playerTurnAuto()
{
    displayBattleStatus();
    if (!inv_) {
        uiManager.showInvalidInput();
        return;
    }
    PlayerAction act = autoCtrl.decide(*player, *monster, *inv_, monster->isBossMonster());

    if (act.type == PlayerActionType::UseItem) {
        // 1) 스탯 스냅샷
        StatSnapshot before = snap(player);

        std::string usedName;
        bool ok = inv_->useItem(
            player,
            act.inventoryIndex,
            effectSystem,
            effectManager,
            &usedName
        );

        StatSnapshot after = snap(player);

        if (ok) {
            autoCtrl.onItemUsed();

            std::string effectText = diffToText(before, after);

            // ✅ 여기서 UI 출력
            uiManager.showItemActionScreen(
                "🧍 " + player->getName() + "의 아이템 사용",
                usedName,
                effectText
            );

            addLog("[AUTO] 아이템 사용: " + usedName + " (" + effectText + ")");
        }
        else {
            addLog("[AUTO] 아이템 사용 실패");
        }
        return;
    }

    int damage = player->getAttack();
    // 1. 크리티컬 여부 결정 (30% 확률)
    bool isCritical = rollCritical();
    if (isCritical) {
        // 3. 크리티컬 배수 적용 (성공 시 2배)
        int finalDamage = applyCriticalMultiplier(damage, isCritical);

        // 4. 몬스터에게 최종 데미지 적용
        monster->takeDamage(finalDamage);
        // 5. 콘솔 UI에 결과 출력 (마지막 인자로 isCritical 전달)
        uiManager.showPlayerAttackResult(
            player->getName(),
            monster->getName(),
            finalDamage,
            !monster->isAlive(),
            isCritical // UIManager에서 크리티컬 문구를 띄우기 위해 추가
        );
        std::string logMsg = player->getName() + "의 공격: " + std::to_string(finalDamage) + " 데미지";
        if (isCritical) {
            logMsg += " (★크리티컬 히트!★)";
        }
        addLog("[AUTO] " + logMsg);
    }
    else {
        monster->takeDamage(damage);
        uiManager.showPlayerAttackResult(
            player->getName(),
            monster->getName(),
            damage,
            !monster->isAlive(),
            isCritical // UIManager에서 크리티컬 문구를 띄우기 위해 추가
        );
        addLog("[AUTO] " + player->getName() + "의 공격: " + std::to_string(damage) + " 데미지");
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
    RewardManager* rewardManager = RewardManager::getInstance();
    BattleResult result = rewardManager->generateRewards(monster);
    result.playerWon = true;
    result.turnCount = maxRounds;
    rewardService.applyDrops(monster, result);
    uiManager.showVictoryScreen(
            monster->isBossMonster(),
            result.goldEarned,
            result.expEarned,
            result.droppedItemNames
    );
    RewardManager::getInstance()->applyRewards(player, result);
    if (result.levelUps > 0) {
        uiManager.showLevelUp(
            player->getLevel(),
            result.levelUps
        );
    };
    waitNextBeat();
    monster->onDeath();
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