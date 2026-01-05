#include "MonsterEncounter.h"
#include "Input.h"
#include "TextLoader.h"
#include <string>
#include <iostream>
#include <algorithm> 
#include <fstream>

#define NOMINMAX

MonsterEncounter::MonsterEncounter(Monster* m, Character* p)
    : monster(m), player(p) {
}

static int GetAsciiFileMaxLineWidth(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) return 0;

    std::string line;
    int mx = 0;
    while (std::getline(in, line)) {
        int len = (int)line.size();
        if (len > mx) mx = len;
    }
    return mx;
}

static int Clamp(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static std::string HLine(int w, char ch = '=') {
    return std::string((w > 0) ? w : 0, ch);
}

void MonsterEncounter::OnEnter() {
    // 초기화
    monsterFrame = 0; 
    animTime = 0.0f;
    finished = false;
}

void MonsterEncounter::Update(float deltaTime) {
    Input::Update();
    Scene::Update(deltaTime);

    // 슬라임
    animTime += deltaTime;
    if (animTime >= 0.2f) {
        monsterFrame = (monsterFrame + 1) % 4;
        animTime = 0.0f;
    }

    // 타이핑 (3배속)
    appearTime += deltaTime * 3.0f;
    if (appearTime > 1.0f) appearTime = 1.0f;

    if (Input::GetKeyDown(KeyCode::Enter) || Input::GetKeyDown(KeyCode::F)) {
        finished = true;
    }
}

void MonsterEncounter::Render(Renderer& renderer) {
    renderer.Clear();

    int w = renderer.GetWidth();
    int h = renderer.GetHeight();
    int centerX = w / 2;

    // 1. 몬스터 아스키 (진짜 중앙)
    std::string artPath = "Phase1/VariableSlime/" + std::to_string(monsterFrame + 1) + ".txt";
    int artW = GetAsciiFileMaxLineWidth(artPath);
    int artX = centerX - artW / 2;
    artX = Clamp(artX, 0, w - artW);
    renderer.PutTextFile(artX, 4, artPath);

    int y = 16;

    // 2. 구분선
    renderer.PutString(0, y++, HLine(w, '='));

    // 3. 몬스터 이름
    std::string monsterName = monster->getName();
    int nameX = centerX - (int)monsterName.size() / 2;
    nameX = Clamp(nameX, 0, w - (int)monsterName.size());
    renderer.PutString(nameX, y++, monsterName);

    renderer.PutString(0, y++, HLine(w, '='));
    y++;

    // 4. 몬스터 정보 (타이핑)
    int infoLineY = y;
    std::string fullInfo = monster->getMobInfo();
    int visibleLen = (int)(fullInfo.size() * appearTime);
    std::string visibleInfo = fullInfo.substr(0, visibleLen);
    int infoX = centerX - (int)visibleInfo.size() / 2;
    infoX = Clamp(infoX, 0, w - (int)visibleInfo.size());
    renderer.PutString(infoX, infoLineY, visibleInfo);

    y = infoLineY + 2;
    renderer.PutString(0, y++, HLine(w, '='));
    y++;

    // 5. 플레이어 정보
    int px = 2;
    renderer.PutString(px, y++, "플레이어 정보");
    renderer.PutString(px, y++, "이름: " + player->getName());
    renderer.PutString(px, y++, "체력: " + std::to_string(player->getHealth()) + "/" + std::to_string(player->getMaxHealth()));
    renderer.PutString(px, y++, "공격: " + std::to_string(player->getAttack()));

    // 6. 안내
    std::string guide = "Enter 또는 F 키를 눌러 전투 시작";
    int guideX = centerX - (int)guide.size() / 2;
    guideX = Clamp(guideX, 0, w - (int)guide.size());
    renderer.PutString(guideX, h - 3, guide);
}
