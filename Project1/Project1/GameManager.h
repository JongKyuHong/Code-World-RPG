#pragma once

#include <string>
#include <map>
#include "Monster.h"
#include "Item.h"
#include "Character.h"

class GameManager
{
private:
    std::map<std::string, int> mobKillCounts;
    Character* player;

public:
    Monster* generateMonster(int level) {};
    Character* generateCharacter() {};

    // call Main Menu에서 게임시작을했을때
    // 여기로 불러와서 게임진행
    // 처음오면 캐릭터생성
    void play() {}

    // 전투
    void battle() {};

    // 상점 방문
    // uiManager에서 상점 품목보여주고
    // 실제입력 여기서 받고
    // 해당하는 아이템의 인덱스로 상점의 buyItem불러옴
    void EnterShop() {}

    Item* generateDroItems(const Monster& monster) {}

    // 게임매니저에서 이런거 출력하는게 나은지
    void diplayKillCount() {};
    void displayCharacterStatus() {};
};

