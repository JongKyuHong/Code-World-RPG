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
    const float FRAME_DURATION = 0.5f;
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
            monster->takeDamage(damage);
            playerScore += 100;
            correctCount++;
            battleResult = "✓ 정답! " + std::to_string(damage) + " 피해를 입혔다!";
        } else {
            int damage = 10;
            player->takeDamage(damage);
            battleResult = "✗ 오답! 정답은 " + correctAnswer + "번입니다. " + std::to_string(damage) + " 피해를 입었다!";
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

        // === 중앙에 큰 ASCII 아트 표시 (MonsterEncounter처럼) ===
        std::string artPath = "Texts/" + monster->getName() + std::to_string(monsterFrame + 1) + ".txt";
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

        // === 구분선 ===
        r.PutString(0, y++, std::string(w, '='));

        // === 몬스터 이름 ===
        std::string monsterName = monster->getName();
        int nameX = centerX - (int)monsterName.size() / 2;
        nameX = Clamp(nameX, 0, w - (int)monsterName.size());
        r.PutString(nameX, y++, monsterName);

        r.PutString(0, y++, std::string(w, '='));
        y++;

        // === 몬스터 정보 (타이핑 효과) ===
        if (!battleFinished) {
            //animTimer += 0.016f;  // 약 60fps 기준
            appearTime = (animTimer / 2.0f > 1.0f) ? 1.0f : animTimer / 2.0f;

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

            // === 플레이어 정보 ===
            int px = 2;
            r.PutString(px, y++, "플레이어 정보");
            r.PutString(px, y++, "이름: " + player->getName());
            r.PutString(px, y++, "체력: " + std::to_string(player->getHealth()) + "/" + std::to_string(player->getMaxHealth()));
            r.PutString(px, y++, "공격: " + std::to_string(player->getAttack()));

            // === 가이드 (등장 애니메이션 중) ===
            if (appearTime < 1.0f) {
                std::string guide = "Enter 또는 F 키를 눌러 전투 시작";
                int guideX = centerX - (int)guide.size() / 2;
                guideX = Clamp(guideX, 0, w - (int)guide.size());
                r.PutString(guideX, h - 3, guide);
            }
            // === 전투 시작 (등장 완료 후) ===
            else {
                y = 18;
                r.PutString(0, y++, std::string(w, '='));
                y++;

                // 진행도
                r.PutString(2, y++, "Turn " + std::to_string(turn) + " / " + std::to_string(quizzes.size()));
                r.PutString(2, y++, "정답: " + std::to_string(correctCount) + "/" + std::to_string(turn - 1));
                y += 2;

                if (currentQuestionIndex < quizzes.size()) {
                    auto& quiz = quizzes[currentQuestionIndex];
                    std::string fullText = quiz.first;  // "문제?\n1) 선택지1\n2) 선택지2\n3) 선택지3"

                    // === 문제와 선택지 분리 ===
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

                    // 문제는 첫 줄
                    r.PutString(2, y++, "[ 문제 ]");
                    r.PutString(2, y++, std::string(w - 4, '-'));
                    r.PutString(4, y++, lines[0]);  // 문제 텍스트
                    y++;

                    // 선택지는 나머지
                    r.PutString(2, y++, "[ 선택지 ]");

                    for (int i = 1; i < lines.size() && i < 4; i++) {
                        if (i - 1 == selectedOption) {
                            r.PutString(4, y++, ">> " + lines[i] + " <<");
                        } else {
                            r.PutString(4, y++, "   " + lines[i]);
                        }
                    }

                    y += 2;

                    // 결과
                    if (questionAnswered) {
                        r.PutString(2, y++, battleResult);
                        y++;
                        r.PutString(2, y++, "아무 키나 눌러 계속...");
                    } else {
                        r.PutString(2, y++, "W/S: 선택, Enter/F: 확정");
                    }
                }
            }
        }
        // === 전투 종료 ===
        else {
            y = 18;
            r.PutString(0, y++, std::string(w, '='));
            y += 2;

            if (player->isAlive()) {
                std::string victoryMsg = "승 리!";
                int victoryX = centerX - (int)victoryMsg.size() / 2;
                r.PutString(victoryX, y++, victoryMsg);
                y += 2;
            } else {
                std::string defeatMsg = "패 배!";
                int defeatX = centerX - (int)defeatMsg.size() / 2;
                r.PutString(defeatX, y++, defeatMsg);
                y += 2;
            }

            std::string scoreMsg = "최종 스코어: " + std::to_string(playerScore);
            int scoreX = centerX - (int)scoreMsg.size() / 2;
            r.PutString(scoreX, y++, scoreMsg);

            std::string rateMsg = "정답률: " + std::to_string(correctCount) + "/" + std::to_string(quizzes.size());
            int rateX = centerX - (int)rateMsg.size() / 2;
            r.PutString(rateX, y++, rateMsg);

            y = h - 3;
            std::string exitMsg = "아무 키나 눌러 계속...";
            int exitX = centerX - (int)exitMsg.size() / 2;
            r.PutString(exitX, y, exitMsg);
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
