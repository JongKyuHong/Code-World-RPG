
#pragma once
#include "Scene.h"
#include "Character.h"
#include "Monster.h"
#include "Renderer.h"
#include "BattleService.h"
#include <vector>
#include <windows.h>
#include <fstream>      // ← ifstream 추가
#include <algorithm>    // ← std::min 추가
#include <chrono>       // ← 시간 관련 추가
#include <iostream>

class BattlePhaseScene : public Scene {
private:
    enum Phase {
        ENCOUNTER,   // 등장 애니메이션
        BATTLE       // 전투
    };

    Character* player;
    Monster* monster;
    Renderer renderer;
    BattleService* battleService;

    Phase currentPhase = ENCOUNTER;

    // 등장 애니메이션 관련
    int encounterFrame = 0;
    float encounterTimer = 0.0f;
    const float ENCOUNTER_DURATION = 0.5f;  // 프레임당 0.5초
    std::string monsterArtPath;
    float appearTime = 0.0f;

    // 전투 관련
    std::vector<std::pair<std::string, std::string>> quizzes;
    int currentQuestionIndex = 0;
    int selectedOption = 0;
    bool questionAnswered = false;
    std::string battleResult;
    bool battleFinished = false;
    int playerScore = 0;
    int turn = 1;
    int correctCount = 0;

public:
    BattlePhaseScene(Character* p, Monster* m, BattleService* bs = nullptr)
        : player(p), monster(m), renderer(160, 50), battleService(bs) {
        InitializeQuestions();
        // 몬스터 이름으로 ASCII 아트 경로 설정
        monsterArtPath = "Texts/" + monster->getName();
    }

    void InitializeQuestions() {
        if (battleService != nullptr) {
            quizzes = battleService->getBossQuizzes(monster->getName());
        }

        if (quizzes.empty()) {
            quizzes = {
                {"이 몬스터의 이름은?\n   1) PointerLich\n   2) PolyDragon\n   3) TeamProjectDevil", "1"},
                {"전투를 계속하시겠습니까?\n   1) 예\n   2) 아니오\n   3) 모르겠습니다", "1"}
            };
        }
    }

    void Update(float dt) override {
        if (currentPhase == ENCOUNTER) {
            UpdateEncounter(dt);
        } else if (currentPhase == BATTLE) {
            UpdateBattle(dt);
        }
    }

    void UpdateEncounter(float dt) {
        encounterTimer += dt;

        // 타이핑 효과
        appearTime = (encounterTimer / 2.0f > 1.0f) ? 1.0f : encounterTimer / 2.0f;

        // 애니메이션 프레임 변경 (4프레임 반복)
        int frameIndex = (int)(encounterTimer / ENCOUNTER_DURATION) % 4;
        encounterFrame = frameIndex;

        // Enter 또는 F 키로 전투 시작
        if ((GetAsyncKeyState(VK_RETURN) & 0x8000) || (GetAsyncKeyState('F') & 0x8000)) {
            currentPhase = BATTLE;
            Sleep(300);
        }
    }

    void UpdateBattle(float dt) {
        if (!questionAnswered && !battleFinished) {
            // W 키: 위로
            if (GetAsyncKeyState('W') & 0x8000) {
                if (selectedOption > 0) selectedOption--;
                Sleep(150);
            }
            // S 키: 아래로
            if (GetAsyncKeyState('S') & 0x8000) {
                if (selectedOption < 2) selectedOption++;
                Sleep(150);
            }
            // Enter 또는 F 키: 선택
            if ((GetAsyncKeyState(VK_RETURN) & 0x8000) || (GetAsyncKeyState('F') & 0x8000)) {
                AnswerQuestion();
                questionAnswered = true;
                Sleep(300);
            }
        } else if (questionAnswered && !battleFinished) {
            // 다음 질문으로 진행
            if (GetAsyncKeyState(VK_RETURN) & 0x8000 || GetAsyncKeyState('F') & 0x8000) {
                if (currentQuestionIndex < quizzes.size() - 1) {
                    currentQuestionIndex++;
                    selectedOption = 0;
                    questionAnswered = false;
                    turn++;
                    Sleep(300);
                } else {
                    FinishBattle();
                }
            }
        } else if (battleFinished) {
            // 전투 종료 후 종료
            if (GetAsyncKeyState(VK_RETURN) & 0x8000 || GetAsyncKeyState('F') & 0x8000) {
                isRunning = false;
                Sleep(300);
            }
        }
    }

    void AnswerQuestion() {
        if (currentQuestionIndex >= quizzes.size()) {
            return;
        }

        auto& quiz = quizzes[currentQuestionIndex];
        std::string correctAnswer = quiz.second;
        std::string selectedAnswer = std::to_string(selectedOption + 1);

        if (selectedAnswer == correctAnswer) {
            int damage = 20 + (player->getAttack() / 2);
            monster->takeDamage(damage);
            playerScore += 100;
            correctCount++;
            battleResult = "✓ 정답! " + std::to_string(damage) + " 피해를 입혔다!";
        } else {
            int damage = 10;
            player->takeDamage(damage);
            battleResult = "✗ 오답! 정답은 " + correctAnswer + "번입니다. " + std::to_string(damage) + " 피해를 입었다!";
        }

        if (!monster->isAlive()) {
            FinishBattle();
            questionAnswered = true;
        } else if (!player->isAlive()) {
            FinishBattle();
            questionAnswered = true;
        }
    }

    void FinishBattle() {
        battleFinished = true;
    }

    void Render(Renderer& r) override {
        if (currentPhase == ENCOUNTER) {
            RenderEncounter(r);
        } else if (currentPhase == BATTLE) {
            RenderBattle(r);
        }
    }

    void RenderEncounter(Renderer& r) {
        r.Clear();

        int w = r.GetWidth();
        int h = r.GetHeight();
        int centerX = w / 2;

        // ASCII 아트 렌더링
        std::string artPath = monsterArtPath + std::to_string(encounterFrame + 1) + ".txt";
        std::ifstream file(artPath);
        if (file.is_open()) {
            std::string line;
            int lineY = 2;

            while (std::getline(file, line) && lineY < h - 20) {
                int lineW = (int)line.length();
                int lineX = centerX - lineW / 2;
                lineX = Clamp(lineX, 0, w - lineW);

                r.PutString(lineX, lineY, line);
                lineY++;
            }
            file.close();
        }

        int y = 16;
        r.PutString(0, y++, std::string(w, '='));

        // 몬스터 이름
        std::string monsterName = monster->getName();
        int nameX = centerX - (int)monsterName.size() / 2;
        nameX = Clamp(nameX, 0, w - (int)monsterName.size());
        r.PutString(nameX, y++, monsterName);

        r.PutString(0, y++, std::string(w, '='));
        y++;

        // 몬스터 정보 (타이핑)
        int infoLineY = y;
        std::string fullInfo = monster->getMobInfo();
        int visibleLen = (int)(fullInfo.size() * appearTime);
        std::string visibleInfo = fullInfo.substr(0, visibleLen);
        int infoX = centerX - (int)visibleInfo.size() / 2;
        infoX = Clamp(infoX, 0, w - (int)visibleInfo.size());
        r.PutString(infoX, infoLineY, visibleInfo);

        y = infoLineY + 2;
        r.PutString(0, y++, std::string(w, '='));
        y++;

        // 플레이어 정보
        int px = 2;
        r.PutString(px, y++, "플레이어 정보");
        r.PutString(px, y++, "이름: " + player->getName());
        r.PutString(px, y++, "체력: " + std::to_string(player->getHealth()) + "/" + std::to_string(player->getMaxHealth()));
        r.PutString(px, y++, "공격: " + std::to_string(player->getAttack()));

        // 안내
        std::string guide = "Enter 또는 F 키를 눌러 전투 시작";
        int guideX = centerX - (int)guide.size() / 2;
        guideX = Clamp(guideX, 0, w - (int)guide.size());
        r.PutString(guideX, h - 3, guide);
    }

    void RenderBattle(Renderer& r) {
        r.Clear();

        int w = r.GetWidth();
        int h = r.GetHeight();
        int centerX = w / 2;

        // 1. 제목
        std::string title = "퀴즈 전투";
        int titleX = centerX - (int)title.size() / 2;
        r.PutString(titleX, 1, title);
        r.PutString(0, 2, std::string(w, '='));

        // 2. 플레이어 vs 몬스터 정보
        int y = 4;
        r.PutString(2, y, "[ " + player->getName() + " ]");
        r.PutString(2, y + 1, "체력: " + std::to_string(player->getHealth()) + "/" + std::to_string(player->getMaxHealth()));

        int monsterInfoX = w - 35;
        r.PutString(monsterInfoX, y, "[ " + monster->getName() + " ]");
        r.PutString(monsterInfoX, y + 1, "체력: " + std::to_string(monster->getHealth()) + "/" + std::to_string(monster->getMaxHealth()));

        y = 8;
        r.PutString(0, y, std::string(w, '-'));

        // 3. 진행 상황
        y = 10;
        std::string progress = "Turn " + std::to_string(turn) + " / " + std::to_string(quizzes.size())
            + " | 정답: " + std::to_string(correctCount) + "/" + std::to_string(turn - 1);
        int progX = centerX - (int)progress.size() / 2;
        r.PutString(progX, y, progress);

        // 4. 퀴즈
        if (!battleFinished) {
            if (currentQuestionIndex < quizzes.size()) {
                auto& quiz = quizzes[currentQuestionIndex];
                std::string question = quiz.first;

                y = 13;
                int qX = 5;
                int lineCount = 0;
                std::string line;
                for (char c : question) {
                    if (c == '\n') {
                        r.PutString(qX, y + lineCount, line);
                        lineCount++;
                        line.clear();
                    } else {
                        line += c;
                    }
                }
                if (!line.empty()) {
                    r.PutString(qX, y + lineCount, line);
                }

                y += 6;

                // 선택지
                std::vector<std::string> options = { "1", "2", "3" };
                for (int i = 0; i < 3; i++) {
                    std::string option = options[i] + ")";

                    if (i == selectedOption) {
                        r.PutString(qX, y + i, ">> " + option + " <<");
                    } else {
                        r.PutString(qX, y + i, "   " + option);
                    }
                }

                y += 5;
                r.PutString(0, y, std::string(w, '-'));
                y++;

                if (questionAnswered) {
                    int resultX = 5;
                    r.PutString(resultX, y, battleResult);
                    y += 2;

                    std::string continueMsg = "아무 키나 눌러 계속...";
                    int continueX = centerX - (int)continueMsg.size() / 2;
                    r.PutString(continueX, y, continueMsg);
                } else {
                    std::string guide = "W/S: 선택, Enter/F: 확정";
                    int guideX = centerX - (int)guide.size() / 2;
                    r.PutString(guideX, y, guide);
                }
            }
        } else {
            y = 15;
            r.PutString(0, y, std::string(w, '='));
            y += 2;

            if (player->isAlive()) {
                std::string victoryMsg = "승 리!";
                int victoryX = centerX - (int)victoryMsg.size() / 2;
                r.PutString(victoryX, y, victoryMsg);
                y += 2;
            } else {
                std::string defeatMsg = "패 배!";
                int defeatX = centerX - (int)defeatMsg.size() / 2;
                r.PutString(defeatX, y, defeatMsg);
                y += 2;
            }

            std::string scoreMsg = "최종 스코어: " + std::to_string(playerScore) + " | 정답률: " + std::to_string(correctCount) + "/" + std::to_string(quizzes.size());
            int scoreX = centerX - (int)scoreMsg.size() / 2;
            r.PutString(scoreX, y, scoreMsg);

            y = h - 3;
            std::string exitMsg = "아무 키나 눌러 계속...";
            int exitX = centerX - (int)exitMsg.size() / 2;
            r.PutString(exitX, y, exitMsg);
        }
    }

    bool IsPhaseFinished() const {
        return !isRunning;
    }

private:
    int Clamp(int value, int min, int max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
};
