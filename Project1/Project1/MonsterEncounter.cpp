#include "MonsterEncounter.h"
#include "Input.h"
#include <string>

MonsterEncounter::MonsterEncounter(Monster* m, Character* p)
    : monster(m), player(p) {
}

void MonsterEncounter::OnEnter() {
    // 초기화
    monsterFrame = 0;
    animTime = 0.0f;
    appearTime = 0.0f;
    finished = false;
}

void MonsterEncounter::Update(float deltaTime) {
    Scene::Update(deltaTime);

    // 애니메이션 타이머
    animTime += deltaTime;
    if (animTime > 0.15f) {  // 0.15초마다 프레임 전환
        animTime = 0.0f;
        monsterFrame = (monsterFrame + 1) % 4;  // 4프레임 순환
    }

    // 등장 연출 시간
    appearTime += deltaTime;

    // Enter 키로 전투 시작
    if (appearTime > 1.0f && Input::GetKeyDown(KeyCode::Enter)) {
        finished = true;
    }
}

void MonsterEncounter::Render(Renderer& renderer) {
    Scene::Render(renderer);
    renderer.PutBox(0, 0, renderer.GetWidth(), renderer.GetHeight());

    std::string monsterName = monster->getName();
    std::string monsterInfo = monster->getMobInfo();
    int phase = monster->GetPhase();

    // 1단계: 몬스터 이름 타이핑 (0.5초부터)
    if (appearTime > 0.5f) {
        int visibleChars = static_cast<int>((appearTime - 0.5f) * 12);
        if (visibleChars > monsterName.size()) visibleChars = monsterName.size();

        int centerX = renderer.GetWidth() / 2 - monsterName.size() / 2;
        renderer.PutString(centerX, 5, monsterName.substr(0, visibleChars));
    }

    // 2단계: 몬스터 애니메이션 등장 (1초부터)
    if (appearTime > 1.0f) {
        std::string frameFile = "Phase" + std::to_string(phase) + "/" +
            monsterName + "/" +
            std::to_string(monsterFrame + 1) + ".txt";
        renderer.PutTextFile(65, 12, frameFile);
    }

    // 3단계: 몬스터 설명 페이드인 (1.3초부터)
    if (appearTime > 1.3f) {
        renderer.PutString(55, 25, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

        // 설명 타이핑 효과
        int descChars = static_cast<int>((appearTime - 1.3f) * 20);
        if (descChars > monsterInfo.size()) descChars = monsterInfo.size();
        renderer.PutString(58, 26, monsterInfo.substr(0, descChars));

        renderer.PutString(55, 27, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    }

    // 4단계: 등장 선언 메시지 (2초부터)
    if (appearTime > 2.0f) {
        renderer.PutString(58, 30, "╔═══════════════════════════════╗");
        renderer.PutString(58, 31, "║ " + monsterName + " 이(가) 나타났다! ║");
        renderer.PutString(58, 32, "╚═══════════════════════════════╝");
    }

    // 5단계: Enter 안내 (2.5초부터)
    if (appearTime > 2.5f) {
        // 깜빡이는 효과
        if (static_cast<int>(appearTime * 2) % 2 == 0) {
            renderer.PutString(60, 36, "▶ Enter 키를 눌러 전투 시작 ◀");
        }
    }

    // 플레이어 정보 (항상 표시)
    renderer.PutBox(3, 38, 40, 6);
    renderer.PutString(5, 39, "━━━━━━━ 플레이어 정보 ━━━━━━━");
    renderer.PutString(5, 40, "👤 이름: " + player->getName());
    renderer.PutString(5, 41, "❤️  체력: " + std::to_string(player->getHealth()) +
        " / " + std::to_string(player->getMaxHealth()));
    renderer.PutString(5, 42, "⚔️  공격: " + std::to_string(player->getAttack()));
}
