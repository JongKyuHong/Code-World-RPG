#pragma once
#include "Scene.h"
#include "Character.h"
#include "Monster.h"
#include "Renderer.h"
#include "BattleService.h"
#include <vector>
#include <string>
#include <utility>
#include <windows.h>
#include <fstream>
#include <iostream>

class BattlePhaseScene : public Scene {
private:
    Character* player;
    Monster* monster;
    BattleService* battleService;

    // 애니메이션
    int monsterFrame = 0;
    float animTimer = 0.0f;
    const float FRAME_DURATION = 0.2f;
    float appearTime = 0.0f;
    // 전투
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
        : player(p), monster(m), battleService(bs) {
        InitializeQuestions();
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
        animTimer += dt;
        int frameIndex = (int)(animTimer / (FRAME_DURATION)) % 4; 
        monsterFrame = frameIndex;

        // 전투 로직
        if (!questionAnswered && !battleFinished) {
            if (GetAsyncKeyState('W') & 0x8000) {
                if (selectedOption > 0) selectedOption--;
                Sleep(150);
            }
            if (GetAsyncKeyState('S') & 0x8000) {
                if (selectedOption < 2) selectedOption++;
                Sleep(150);
            }
            if ((GetAsyncKeyState(VK_RETURN) & 0x8000) || (GetAsyncKeyState('F') & 0x8000)) {
                AnswerQuestion();
                questionAnswered = true;
                Sleep(300);
            }
        } else if (questionAnswered && !battleFinished) {
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
            if (GetAsyncKeyState(VK_RETURN) & 0x8000 || GetAsyncKeyState('F') & 0x8000) {
                isRunning = false;
                Sleep(300);
            }
        }
    }

    void AnswerQuestion() {
        if (currentQuestionIndex >= quizzes.size()) return;

        auto& quiz = quizzes[currentQuestionIndex];
        std::string correctAnswer = quiz.second;
        std::string selectedAnswer = std::to_string(selectedOption + 1);

        if (selectedAnswer == correctAnswer) {
            int damage = 20 + (player->getAttack() / 2);
            battleResult = "✓ 정답!";
            correctCount++;
        } else {
            battleResult = "✗ 오답!";
            player->takeDamage(player->getHealth()); 
            battleFinished = true;
        }

        if (!monster->isAlive() || !player->isAlive()) {
            FinishBattle();
            questionAnswered = true;
        }
    }

    void FinishBattle() {
        battleFinished = true;
    }

    void Render(Renderer& r) override {
        r.Clear();

        int w = r.GetWidth();
        int h = r.GetHeight();
        int centerX = w / 2;

        // ═══════════════════════════════════════════════════════════════
        // [ 섹션 1: 상단 헤더 + 몬스터 ASCII 아트 ]
        // ═══════════════════════════════════════════════════════════════
        std::string headerLine(w, '=');
        r.PutString(0, 0, headerLine, Renderer::LIGHT_GRAY);

        std::string monsterName = monster->getName();
        int nameX = centerX - (int)monsterName.size() / 2;
        r.PutString(nameX, 1, "[ 🧟 " + monsterName + " 🧟 ]", Renderer::LIGHT_RED);  // 🔴 빨간색

        r.PutString(0, 2, headerLine, Renderer::LIGHT_GRAY);

        // ASCII 아트 출력 (Y: 3~12) - 파란색
        std::string artPath = "Texts/" + monster->getName() + std::to_string(monsterFrame + 1) + ".txt";
        std::ifstream file(artPath);
        if (file.is_open()) {
            std::string line;
            int lineY = 3;

            while (std::getline(file, line) && lineY < 12) {
                int lineW = (int)line.length();
                int lineX = centerX - lineW / 2;
                lineX = Clamp(lineX, 0, w - lineW);
                r.PutString(lineX, lineY, line, Renderer::LIGHT_CYAN);  // 🔵 파란색
                lineY++;
            }
            file.close();
        }

        r.PutString(0, 12, headerLine, Renderer::LIGHT_GRAY);

        // ═══════════════════════════════════════════════════════════════
        // [ 섹션 2: 전투 통계 (체력, 턴, 정답 정보) ]
        // ═══════════════════════════════════════════════════════════════
        int statsY = 13;

        // 상자 테두리
        r.PutString(0, statsY, "┌────────────────────────────────────────────────────────────┐", Renderer::LIGHT_GRAY);

        // 플레이어 정보 - 녹색
        r.PutString(2, statsY + 1, "👤 " + player->getName(), Renderer::LIGHT_GREEN);
        r.PutString(2, statsY + 2, "❤️  HP: " + std::to_string(player->getHealth()) + " / " +
            std::to_string(player->getMaxHealth()), Renderer::LIGHT_GREEN);

        // 턴 정보 - 노란색
        std::string turnInfo = "Turn: " + std::to_string(turn) + "/3";
        std::string correctInfo = "정답: " + std::to_string(correctCount) + "/3";
        int midX = centerX - 8;
        r.PutString(midX, statsY + 1, turnInfo, Renderer::LIGHT_YELLOW);
        r.PutString(midX, statsY + 2, correctInfo, Renderer::LIGHT_YELLOW);

        // 몬스터 정보 - 빨간색
        int rightX = w - 38;
        r.PutString(rightX, statsY + 1, "👹 " + monster->getName(), Renderer::LIGHT_RED);
        r.PutString(rightX, statsY + 2, "❤️  HP: " + std::to_string(monster->getHealth()) + " / " +
            std::to_string(monster->getMaxHealth()), Renderer::LIGHT_RED);

        r.PutString(0, statsY + 3, "└────────────────────────────────────────────────────────────┘", Renderer::LIGHT_GRAY);

        // ═══════════════════════════════════════════════════════════════
        // [ 섹션 3: 문제 & 선택지 ]
        // ═══════════════════════════════════════════════════════════════
        int battleY = statsY + 5;

        if (!battleFinished && currentQuestionIndex < quizzes.size()) {
            auto& quiz = quizzes[currentQuestionIndex];
            std::string fullText = quiz.first;

            // 문제와 선택지 파싱
            std::vector<std::string> lines;
            std::string line;
            for (char c : fullText) {
                if (c == '\n') {
                    lines.push_back(line);
                    line.clear();
                } else {
                    line += c;
                }
            }
            if (!line.empty()) {
                lines.push_back(line);
            }

            // 문제 헤더 - 밝은 노란색
            r.PutString(0, battleY++, "╔════════════════════════════════════════════════════════════╗", Renderer::LIGHT_GRAY);
            r.PutString(2, battleY++, "🧠  [ 문 제 ]", Renderer::LIGHT_YELLOW);
            r.PutString(0, battleY++, "╠════════════════════════════════════════════════════════════╣", Renderer::LIGHT_GRAY);

            // 문제 텍스트 - 흰색
            if (!lines.empty()) {
                std::string problemText = "   " + lines[0];
                r.PutString(0, battleY++, problemText, Renderer::WHITE);
            }
            battleY++;

            // 선택지 헤더 - 파란색
            r.PutString(2, battleY++, "[ 선 택 지 ]", Renderer::LIGHT_CYAN);
            r.PutString(0, battleY++, "┌────────────────────────────────────────────────────────────┐", Renderer::LIGHT_GRAY);

            // 선택지 출력 - 선택된 것은 노란색, 아닌 것은 흰색
            for (int i = 1; i < lines.size() && i < 4; i++) {
                std::string marker = (i - 1 == selectedOption) ? ">>  " : "    ";
                std::string selectedMark = (i - 1 == selectedOption) ? "  <<" : "";
                std::string choiceText = std::to_string(i) + ") " + lines[i] + selectedMark;

                Renderer::Color choiceColor = (i - 1 == selectedOption) ? Renderer::LIGHT_YELLOW : Renderer::WHITE;
                r.PutString(2, battleY++, marker + choiceText, choiceColor);
            }

            r.PutString(0, battleY++, "└────────────────────────────────────────────────────────────┘", Renderer::LIGHT_GRAY);
            battleY++;

            // 정답/오답 결과 또는 안내
            if (questionAnswered) {
                r.PutString(0, battleY, "╔════════════════════════════════════════════════════════════╗", Renderer::LIGHT_GRAY);
                battleY++;

                if (battleResult.find("정답") != std::string::npos) {
                    // 정답 - 녹색
                    r.PutString(2, battleY++, "✅ " + battleResult + "  ✅", Renderer::LIGHT_GREEN);
                } else {
                    // 오답 - 빨간색
                    r.PutString(2, battleY++, "❌ " + battleResult + "  ❌", Renderer::LIGHT_RED);
                }

                battleY++;
                r.PutString(0, battleY, "╚════════════════════════════════════════════════════════════╝", Renderer::LIGHT_GRAY);
                battleY++;

                std::string continueMsg = "[ 아무 키나 눌러 계속... ]";
                int continueX = centerX - (int)continueMsg.size() / 2;
                r.PutString(continueX, battleY, continueMsg, Renderer::LIGHT_YELLOW);
            } else {
                r.PutString(0, battleY, "─────────────────────────────────────────────────────────────", Renderer::LIGHT_GRAY);
                battleY++;
                std::string guideMsg = "< W/S: 선택 | Enter/F: 확정 >";
                int guideX = centerX - (int)guideMsg.size() / 2;
                r.PutString(guideX, battleY, guideMsg, Renderer::LIGHT_CYAN);
            }
        }
        // ═══════════════════════════════════════════════════════════════
        // [ 섹션 4: 전투 종료 (승리/패배) ]
        // ═══════════════════════════════════════════════════════════════
        else if (battleFinished) {
            r.PutString(0, battleY, "╔════════════════════════════════════════════════════════════╗", Renderer::LIGHT_GRAY);
            battleY++;
            r.PutString(0, battleY, "║                                                            ║", Renderer::LIGHT_GRAY);
            battleY++;

            if (player->isAlive()) {
                // 승리 - 녹색
                std::string victoryMsg = "🎉  승 리!  🎉";
                int victoryX = centerX - (int)victoryMsg.size() / 2;
                r.PutString(0, battleY, "║" + std::string(60, ' ') + "║", Renderer::LIGHT_GRAY);
                r.PutString(victoryX, battleY, victoryMsg, Renderer::LIGHT_GREEN);
                battleY++;

                r.PutString(0, battleY, "║                                                            ║", Renderer::LIGHT_GRAY);
                battleY++;
                r.PutString(2, battleY, "║  당신은 모든 문제를 해결했습니다!                           ║", Renderer::LIGHT_GREEN);
                battleY++;
            } else {
                // 패배 - 빨간색
                std::string defeatMsg = "💀  패 배...  💀";
                int defeatX = centerX - (int)defeatMsg.size() / 2;
                r.PutString(0, battleY, "║" + std::string(60, ' ') + "║", Renderer::LIGHT_GRAY);
                r.PutString(defeatX, battleY, defeatMsg, Renderer::LIGHT_RED);
                battleY++;

                r.PutString(0, battleY, "║                                                            ║", Renderer::LIGHT_GRAY);
                battleY++;
                r.PutString(2, battleY, "║  문제를 풀지 못했습니다...                                  ║", Renderer::LIGHT_RED);
                battleY++;
            }

            r.PutString(0, battleY, "║                                                            ║", Renderer::LIGHT_GRAY);
            battleY++;
            r.PutString(0, battleY, "╠════════════════════════════════════════════════════════════╣", Renderer::LIGHT_GRAY);
            battleY++;

            // 스코어 - 노란색
            std::string scoreMsg = "스코어: " + std::to_string(playerScore);
            int scoreX = centerX - (int)scoreMsg.size() / 2;
            r.PutString(scoreX, battleY++, scoreMsg, Renderer::LIGHT_YELLOW);

            // 정답률 - 노란색
            std::string rateMsg = "정답률: " + std::to_string(correctCount) + "/" + std::to_string(quizzes.size());
            int rateX = centerX - (int)rateMsg.size() / 2;
            r.PutString(rateX, battleY++, rateMsg, Renderer::LIGHT_YELLOW);

            battleY++;
            r.PutString(0, battleY, "╚════════════════════════════════════════════════════════════╝", Renderer::LIGHT_GRAY);
            battleY += 2;

            std::string exitMsg = "[ 아무 키나 눌러 계속... ]";
            int exitX = centerX - (int)exitMsg.size() / 2;
            r.PutString(exitX, battleY, exitMsg, Renderer::LIGHT_YELLOW);
        }
    }

    bool IsFinished() const {
        return !isRunning;
    }

private:
    int Clamp(int value, int min, int max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
};
