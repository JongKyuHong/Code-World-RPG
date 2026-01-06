#include "NormalBattleScene.h"
#include "Input.h"
#include <windows.h>
#include <sstream>
#include <iostream>

NormalBattleScene::NormalBattleScene(Monster* m, Character* p)
    : monster(m), player(p), state(PLAYER_SELECT), battleEnded(false), result() {
}

void NormalBattleScene::OnEnter() {
    state = PLAYER_SELECT;
    selectedAction = 0;
}

void NormalBattleScene::Update(float deltaTime) {
    static bool wPrev = false, sPrev = false, enterPrev = false, fPrev = false;
    static int turnNumber = 0;

    bool wNow = (GetAsyncKeyState('W') & 0x8000);
    bool sNow = (GetAsyncKeyState('S') & 0x8000);
    bool enterNow = (GetAsyncKeyState(VK_RETURN) & 0x8000);
    bool fNow = (GetAsyncKeyState('F') & 0x8000);

    // WS 이동
    if (wNow && !wPrev && selectedAction > 0) selectedAction--;
    if (sNow && !sPrev && selectedAction < 2) selectedAction++;

    // Enter/F = 턴 진행 (즉시 종료 X)
    if ((enterNow && !enterPrev) || (fNow && !fPrev)) {
        turnNumber++;
        std::cout << "\n=== TURN " << turnNumber << " ===" << std::endl;
        std::cout << "Action: " << (selectedAction + 1) << std::endl;

        switch (selectedAction) {
        case 0: {  // 공격
            int damage = player->getAttack();
            monster->takeDamage(damage);
            std::cout << "Player attacks for " << damage << "!" << std::endl;

            if (!monster->isAlive()) {
                std::cout << "Monster defeated!" << std::endl;
                battleEnded = true;
                result.playerWon = true;
                result.turnCount = turnNumber;
                result.goldEarned = 50;
                result.expEarned = 30;
                return;
            }

            // 몬스터 반격
            int mDamage = monster->getAttack();
            player->takeDamage(mDamage);
            std::cout << "Monster attacks for " << mDamage << "!" << std::endl;

            if (!player->isAlive()) {
                std::cout << "Player defeated!" << std::endl;
                battleEnded = true;
                result.playerWon = false;
                return;
            }
            break;
        }
        case 1: case 2:  // 상점/인벤 = 턴 스킵
            std::cout << "Turn skipped" << std::endl;
            break;
        }
        std::cout << "Player HP: " << player->getHealth() << "/" << player->getMaxHealth() << std::endl;
        std::cout << "Monster HP: " << monster->getHealth() << "/" << monster->getMaxHealth() << std::endl;
    }

    wPrev = wNow; sPrev = sNow; enterPrev = enterNow; fPrev = fNow;
}

void NormalBattleScene::Render(Renderer& renderer) {
    renderer.Clear();
    int w = renderer.GetWidth();
    int centerX = w / 2;

    // 몬스터 아스키
    std::string monsterArt = "Monsters/" + monster->getName() + "/1.txt";
    renderer.PutTextFile(centerX - 20, 2, monsterArt);

    // 상태 바
    renderer.PutBox(5, 25, w - 10, 8);

    // HP 바
    std::string playerHP = "Player HP: " + std::to_string(player->getHealth()) + "/" +
        std::to_string(player->getMaxHealth());
    renderer.PutString(10, 26, playerHP.c_str());

    std::string monsterHP = monster->getName() + " HP: " + std::to_string(monster->getHealth()) + "/" +
        std::to_string(monster->getMaxHealth());
    renderer.PutString(10, 28, monsterHP.c_str());

    // 선택지 (MainMenu 스타일)
    std::string actions[3] = { "1) 공격", "2) 상점", "3) 인벤토리" };
    for (int i = 0; i < 3; i++) {
        int actionY = 34 + i * 2;
        if (i == selectedAction) {
            renderer.PutString(15, actionY, ">");
            renderer.PutBox(15, actionY - 1, 30, 3);
        }
        renderer.PutString(25, actionY, actions[i].c_str());
    }

    // 상태 메시지
    switch (state) {
    case PLAYER_SELECT:
        renderer.PutString(centerX - 15, 45, "W/S: 선택 F: 결정");
        break;
    case PLAYER_ATTACK:
        renderer.PutString(centerX - 10, 45, "공격!");
        break;
    case MONSTER_ATTACK:
        renderer.PutString(centerX - 10, 45, "몬스터 공격!");
        break;
    }
}
