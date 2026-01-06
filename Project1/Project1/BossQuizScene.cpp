#include "BossQuizScene.h"
#include "Input.h"
#include <windows.h>
#include <sstream>

BossQuizScene::BossQuizScene(Monster* m, Character* p)
    : monster(m), player(p), quizState(SHOW_QUESTION), sceneFinished(false), result() {
    loadQuizzes();
}

void BossQuizScene::OnEnter() {
    currentRound = 0;
    playerCorrect = 0;
    selectedOption = 0;
    quizState = SHOW_QUESTION;
}

void BossQuizScene::loadQuizzes() {
    std::string bossName = monster->getName();
    if (bossName.find("PointerLich") != std::string::npos) {
        quizzes = {
            {"int* ptr;의 * 기호 의미는?\n   1) 곱셈연산자\n   2) 포인터선언\n   3) 참조연산자", "2"},
            {"int x=5; int* p=&x; *p=10; 실행 후 x값은?\n   1) 5\n   2) 10\n   3) 15", "2"},
            {"int x=10; int* p=&x; cout << *p; 출력값은?\n   1) 10\n   2) 0\n   3) 주소값", "1"}
        };
        maxRounds = 3;
    } else if (bossName.find("PolyDragon") != std::string::npos) {
        quizzes = {
            {"class A { virtual void f(); }; 'virtual' 키워드 역할은?\n   1) 상속\n   2) 템플릿\n   3) 가상함수", "3"},
            {"class Parent { void show(); }; class Child : public Parent { void show(); }; 관계는?\n   1) 오버로드\n   2) 오버라이드\n   3) 다중상속", "2"},
            {"Base* obj = new Derived(); 이때 obj의 실제 타입은?\n   1) Base\n   2) Derived\n   3) void*", "2"},
            {"class Animal { virtual ~Animal() {} }; 소멸자에 virtual을 쓰는 이유는?\n   1) 메모리누수방지\n   2) 성능향상 \n   3) 컴파일오류방지", "1"}
        };
        maxRounds = 4;
    } else if (bossName.find("TeamProjectDevil") != std::string::npos) {
        quizzes = {
            {"GitHub에서 코드 병합을 요청하는 기능은?\n   1) Commit\n   2) PR\n   3) Issue", "2"},
            {"class Singleton { static Singleton* instance; }; instance는 몇 개?\n   1) 0\n   2) 1개\n   3) 무한대", "2"},
            {"Observer 패턴에서 상태 변경 시 누가 통보?\n   1) Observer\n   2) Context\n   3) Subject", "3"},
            {"Git에서 브랜치를 합치는 명령어는?\n   1) merge\n   2) push\n   3) commit", "1"},
            {"데코레이터 패턴의 핵심 비유로 가장 적절한 것은?\n   1) 붕어빵 틀에서 붕어빵 찍어내기\n   2) 마트료시카 인형처럼 객체 안에 객체를 계속 감싸기\n   3) 로봇 조립처럼 부품을 조합하기", "2"}
        };
        maxRounds = 5;
    } else {
        maxRounds = 3;
    }
}

void BossQuizScene::Update(float deltaTime) {
    Scene::Update(deltaTime);

    if (quizState == SHOW_QUESTION) {
        static float showTime = 0;
        showTime += deltaTime;
        if (showTime > 2.0f) {
            quizState = WAIT_INPUT;
            showTime = 0;
        }
        return;
    }

    if (quizState == WAIT_INPUT) {
        if (Input::GetKeyDown(KeyCode::W)) {
            if (selectedOption > 0) selectedOption--;
        }
        if (Input::GetKeyDown(KeyCode::S)) {
            if (selectedOption < 2) selectedOption++;
        }
        if ((GetAsyncKeyState(VK_RETURN) & 0x8000) || (GetAsyncKeyState('F') & 0x8000)) {
            // ✅ string으로 통일
            std::string answer = std::to_string(selectedOption + 1);
            std::string correct = quizzes[currentRound].second;  // string 추출
            if (answer == correct) {
                playerCorrect++;
            }
            quizState = SHOW_RESULT;
        }
        return;
    }

    if (quizState == SHOW_RESULT) {
        static float resultTime = 0;
        resultTime += deltaTime;
        if (resultTime > 1.5f) {
            resultTime = 0;
            if (currentRound + 1 < maxRounds) {
                currentRound++;
                quizState = SHOW_QUESTION;
            } else {
                result.playerWon = true;
                result.turnCount = maxRounds;
                result.goldEarned = 200;
                result.expEarned = 100;
                sceneFinished = true;
            }
        }
    }
}

void BossQuizScene::Render(Renderer& renderer) {
    renderer.Clear();
    int w = renderer.GetWidth();
    int centerX = w / 2;
    int y = 8;

    std::string artFile = "Phase1/VariableSlime/" + std::to_string((currentRound % 4) + 1) + ".txt";
    renderer.PutTextFile(centerX - 20, 2, artFile);

    renderer.PutBox(5, y, w - 10, 22);

    switch (quizState) {
    case SHOW_QUESTION: {

        std::string roundText = "Round " + std::to_string(currentRound + 1) + "/" +
            std::to_string(maxRounds);
        renderer.PutString(centerX - 20, y + 2, roundText.c_str());
        renderer.PutString(20, y + 4, quizzes[currentRound].first.c_str());
        break;
    }

    case WAIT_INPUT: {
        std::string options[3] = { "1) 곱셈", "2) 포인터", "3) 참조" };
        for (int i = 0; i < 3; i++) {
            int optY = y + 5 + i * 2;
            if (i == selectedOption) {
                renderer.PutString(8, optY, ">");
                renderer.PutBox(8, optY - 1, 40, 3);
            }
            renderer.PutString(18, optY, options[i].c_str());
        }
        renderer.PutString(centerX - 10, y + 15, "W/S: 이동  F: 선택");
        break;
    }

    case SHOW_RESULT: {
        std::string answer = std::to_string(selectedOption + 1);
        std::string correct = quizzes[currentRound].second;
        bool isCorrect = (answer == correct);

        renderer.PutString(centerX - 10, y + 4, isCorrect ? "O 정답!" : "X 틀림!");
        std::string score = "점수: " + std::to_string(playerCorrect) + "/" + std::to_string(maxRounds);
        renderer.PutString(centerX - 15, y + 7, score.c_str());
        break;
    }
    }
}

