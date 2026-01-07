#pragma once
#pragma comment(lib, "winmm.lib")
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
#include <conio.h>
#include <mmsystem.h>
#include <thread>

class BattlePhaseScene : public Scene {
private:
    Character* player;
    Monster* monster;
    BattleService* battleService;
    BattleResult BattleResultI{ 0, 0 };

    // 애니메이션
    int monsterFrame = 0;
    float animTimer = 0.0f;
    const float FRAME_DURATION = 0.1f;
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
    int totalQuestions = 0;
    // 오디오 변수
    bool bgmPlaying = false;
    std::string currentBgm = "";
    static constexpr const char* SOUNDS_FOLDER = "Sounds/";

    DWORD bgmStartTime = 0;

    bool needsRestartBGM = false;
    DWORD bgmRestartTime = 0;
    const float BGM_RESTART_DELAY = 0.5f;

    std::string bgmToRestart = "";
public:
    BattlePhaseScene(Character* p, Monster* m, BattleService* bs = nullptr)
        : player(p), monster(m), battleService(bs) {
        InitializeQuestions();
    }

    void OnEnter() override {
        PlayBackgroundMusic("BossBattle.wav");

        //// 입력 버퍼 정리
        //HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        //FlushConsoleInputBuffer(hStdin);
    }

    void PlaySoundAsync(const std::string& soundFileName) {
        std::thread([this, soundFileName]() {
            std::string soundPath = std::string(SOUNDS_FOLDER) + soundFileName;
            PlaySoundA(soundPath.c_str(), NULL, SND_FILENAME | SND_SYNC);  // SND_SYNC 사용
        }).detach();
    }

    // 오디오 함수
    void PlaySound(const std::string& soundFileName, int volume = 100) {
        std::string soundPath = std::string(SOUNDS_FOLDER) + soundFileName;
        PlaySoundA(soundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
    }

    void PlayBackgroundMusic(const std::string& bgmFileName) {
        if (bgmPlaying && currentBgm == bgmFileName) {
            return;
        }

        std::string soundPath = std::string(SOUNDS_FOLDER) + bgmFileName;
        PlaySoundA(soundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

        bgmPlaying = true;
        currentBgm = bgmFileName;
        bgmStartTime = GetTickCount();
    }

    void StopBackgroundMusic() {
        PlaySoundA(NULL, NULL, SND_PURGE);
        bgmPlaying = false;
        currentBgm = "";
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

        totalQuestions = quizzes.size();
    }

    void Update(float dt) override {
        animTimer += dt;
        int frameIndex = (int)(animTimer / FRAME_DURATION) % 4;
        monsterFrame = frameIndex;

        static DWORD lastKeyTime = 0;
        DWORD now = GetTickCount();


        if (battleFinished) {
            if ((GetAsyncKeyState(VK_RETURN) & 0x8000 || GetAsyncKeyState('F') & 0x8000)
                && now - lastKeyTime > 300) {
                isRunning = false;
                lastKeyTime = now;
            }
            return;
        }

        if ((GetAsyncKeyState('W') & 0x8000) && now - lastKeyTime > 200) {
            if (selectedOption > 0) selectedOption--;
            lastKeyTime = now;
        }
        if ((GetAsyncKeyState('S') & 0x8000) && now - lastKeyTime > 200) {
            if (selectedOption < 2) selectedOption++;
            lastKeyTime = now;
        }

        if ((GetAsyncKeyState(VK_RETURN) & 0x8000 || GetAsyncKeyState('F') & 0x8000)
            && now - lastKeyTime > 300) {
            AnswerQuestion();

            if (currentQuestionIndex < (int)quizzes.size() - 1) {
                currentQuestionIndex++;
                selectedOption = 0;
                turn++;
            } else {
                battleFinished = true;
            }

            lastKeyTime = now;
        }
    }

    void RestartBackgroundMusic() {
        // 현재 배경음을 강제로 다시 재생
        std::string soundPath = std::string(SOUNDS_FOLDER) + currentBgm;
        PlaySoundA(NULL, NULL, SND_PURGE);  // 현재 루프 중단
        Sleep(50);  // 짧은 대기
        PlaySoundA(soundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        bgmPlaying = true;
    }

    void AnswerQuestion() {
        auto& quiz = quizzes[currentQuestionIndex];
        int correctAnswer = std::stoi(quiz.second);

        if (selectedOption == correctAnswer - 1) {
            PlaySoundAsync("Correct.wav");

            BattleResultI.turnCount++;
            correctCount++;
            if (correctCount >= totalQuestions) {
                battleFinished = true;
                BattleResultI.playerWon = true;
                BattleResultI.isBossKill = monster->isBossMonster();
                BattleResultI.monsterName = monster->getName();
                BattleResultI.expEarned = 100;      // 보스 전투 고정 경험치
                BattleResultI.goldEarned = 500;     // 보스 전투 고정 골드
                PlaySoundAsync("BossWin.wav");
                StopBackgroundMusic();
            } else {
                RestartBackgroundMusic();
            }
        } else {
            player->takeDamage(player->getMaxHealth());
            // 오답 시 즉시 패배
            BattleResultI.playerWon = false;
            BattleResultI.turnCount++;
            battleFinished = true;
            PlaySoundAsync("BossDefeat.wav");
            StopBackgroundMusic();
        }
    }

    void FinishBattle() {
        battleFinished = true;
    }

    void Render(Renderer& r) override {
        r.Clear();

        // 1. 기본 화면 크기 및 레이아웃 설정
        int w = r.GetWidth();
        int h = r.GetHeight(); // 화면의 전체 높이 한계
        int centerX = w / 2;
        const int BOX_WIDTH = 120;

        std::string headerLine(BOX_WIDTH, '=');
        int headerX = centerX - BOX_WIDTH / 2;

        // =============================================
        // [ 섹션 1: 몬스터 이름 ] (고정 영역)
        // =============================================
        r.PutString(headerX, 0, headerLine, Renderer::LIGHT_GRAY);

        std::string monsterName = monster->getName();
        int nameX = centerX - (int)monsterName.size() / 2;
        r.PutString(nameX, 1, "[ " + monsterName + " ]", Renderer::LIGHT_RED);

        r.PutString(headerX, 2, headerLine, Renderer::LIGHT_GRAY);

        // =============================================
        // [ 섹션 2: 아스키 아트 ] (가변 영역 - 핵심 수정)
        // =============================================

        // [수정 포인트 1] 시작 Y 위치를 변수에 담습니다.
        // 이 변수(currentY)는 출력할 때마다 계속 1씩 증가합니다.
        int currentY = 3;

        std::string artPath = "Texts/" + monster->getName() + std::to_string(monsterFrame + 1) + ".txt";
        std::ifstream file(artPath);

        if (file.is_open()) {
            std::string line;

            // [수정 포인트 2] 반복문 조건에서 'lineY < 숫자' 제한을 제거했습니다.
            // 대신 안전장치로 화면 전체 높이(h)를 넘지 않도록만(currentY < h - 15) 체크합니다.
            // (아래쪽에 문제/선택지 UI가 들어갈 공간 약 15줄은 남겨둬야 짤리지 않기 때문입니다)
            while (std::getline(file, line) && currentY < (h - 15)) {
                int lineW = (int)line.length();

                // 중앙 정렬 계산
                int lineX = centerX - lineW / 2;
                lineX = Clamp(lineX, 0, w - lineW);

                // 현재 위치(currentY)에 출력
                r.PutString(lineX, currentY, line, Renderer::LIGHT_CYAN);

                // [수정 포인트 3] 출력이 끝났으므로 Y축을 한 칸 내립니다.
                currentY++;
            }
            file.close();
        } else {
            // 파일이 없을 경우를 대비해 최소한의 빈 공간을 둡니다 (선택사항)
            currentY += 5;
        }

        // =============================================
        // [ 섹션 3: 전투 통계 ] (위치 자동 계산)
        // =============================================

        // [수정 포인트 4] 아스키 아트가 끝난 지점(currentY)에 바로 구분선을 그립니다.
        r.PutString(headerX, currentY, headerLine, Renderer::LIGHT_GRAY);

        // 통계 정보는 구분선 바로 다음 줄(currentY + 1)부터 시작합니다.
        int statsY = currentY + 1;

        std::string playerName = player->getName();
        std::string monsterNameShort = monster->getName();

        // 플레이어 (좌측)
        r.PutString(headerX + 4, statsY, playerName, Renderer::LIGHT_GREEN);
        r.PutString(headerX + 4, statsY + 1, "HP: " + std::to_string(player->getHealth()) + "/" +
            std::to_string(player->getMaxHealth()), Renderer::LIGHT_GREEN);

        // 턴/정답 (중앙)
        std::string turnInfo = "Turn: " + std::to_string(turn) + "/" + std::to_string(totalQuestions);
        std::string correctInfo = "Correct: " + std::to_string(correctCount) + "/" + std::to_string(totalQuestions);
        int midX = headerX + BOX_WIDTH / 2 - 25;
        r.PutString(midX, statsY, turnInfo, Renderer::LIGHT_YELLOW);
        r.PutString(midX, statsY + 1, correctInfo, Renderer::LIGHT_YELLOW);

        // 몬스터 (우측)
        int monsterX = headerX + BOX_WIDTH - 4 - (int)monsterNameShort.size();
        r.PutString(monsterX, statsY, monsterNameShort, Renderer::LIGHT_RED);
        r.PutString(monsterX, statsY + 1, "HP: " + std::to_string(monster->getHealth()) + "/" +
            std::to_string(monster->getMaxHealth()), Renderer::LIGHT_RED);

        // 통계 섹션 끝 구분선 (2줄 사용했으니 +2 위치에 그림)
        r.PutString(headerX, statsY + 2, headerLine, Renderer::LIGHT_GRAY);

        // =============================================
        // [ 섹션 4: 문제 & 선택지 ] (위치 자동 계산)
        // =============================================

        // [수정 포인트 5] battleY 역시 statsY에 종속되므로 자동으로 위치가 잡힙니다.
        int battleY = statsY + 3;

        if (!battleFinished && currentQuestionIndex < quizzes.size()) {
            auto& quiz = quizzes[currentQuestionIndex];
            std::string fullText = quiz.first;

            // 파싱
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

            // 문제 섹션 (headerX 기준 정렬)
            r.PutString(headerX, battleY, headerLine, Renderer::LIGHT_GRAY);

            // "[ QUESTION ]" (headerX 기준)
            std::string questionLabel = "[ QUESTION ]";
            r.PutString(headerX + 4, battleY + 1, questionLabel, Renderer::LIGHT_YELLOW);

            // 문제 텍스트 (headerX 기준, 안쪽 여백)
            if (!lines.empty()) {
                std::string problemText = lines[0];
                int problemX = headerX + BOX_WIDTH / 2 - (int)problemText.size() / 2;
                r.PutString(problemX, battleY + 2, problemText, Renderer::WHITE);
            }

            r.PutString(headerX, battleY + 3, headerLine, Renderer::LIGHT_GRAY);

            // 선택지 섹션 (headerX 기준)
            std::string choiceLabel = "[ CHOICES ]";
            r.PutString(headerX + 4, battleY + 4, choiceLabel, Renderer::LIGHT_CYAN);

            // 선택지 (headerX 기준, 안쪽 정렬)
            for (int i = 1; i < lines.size() && i < 4; i++) {
                std::string marker = (i - 1 == selectedOption) ? ">> " : "   ";
                std::string selectedMark = (i - 1 == selectedOption) ? " <<" : "";
                std::string choiceText = lines[i] + selectedMark;

                int choiceX = headerX + BOX_WIDTH / 2 - (int)choiceText.size() / 2;
                Renderer::Color choiceColor = (i - 1 == selectedOption) ? Renderer::LIGHT_YELLOW : Renderer::WHITE;
                r.PutString(choiceX, battleY + 5 + (i - 1), marker + choiceText, choiceColor);
            }

            r.PutString(headerX, battleY + 8, headerLine, Renderer::LIGHT_GRAY);

            // 안내 메시지 (headerX 기준)
            if (!questionAnswered) {
                std::string guideMsg = "< W/S: Select | Enter/F: Confirm >";
                int guideX = headerX + BOX_WIDTH / 2 - (int)guideMsg.size() / 2;
                r.PutString(guideX, battleY + 9, guideMsg, Renderer::LIGHT_CYAN);
            }
        } else if (battleFinished) {
            int endY = battleY + 2;
            r.PutString(headerX, endY, headerLine, Renderer::LIGHT_GRAY);

            if (player->isAlive()) {
                std::string victoryMsg = "VICTORY!";
                int victoryX = headerX + BOX_WIDTH / 2 - (int)victoryMsg.size() / 2;
                r.PutString(victoryX, endY + 1, victoryMsg, Renderer::LIGHT_GREEN);
            } else {
                std::string defeatMsg = "DEFEAT...";
                int defeatX = headerX + BOX_WIDTH / 2 - (int)defeatMsg.size() / 2;
                r.PutString(defeatX, endY + 1, defeatMsg, Renderer::LIGHT_RED);
            }

            r.PutString(headerX, endY + 2, headerLine, Renderer::LIGHT_GRAY);

            std::string scoreMsg = "Score: " + std::to_string(playerScore);
            int scoreX = headerX + BOX_WIDTH / 2 - (int)scoreMsg.size() / 2;
            r.PutString(scoreX, endY + 3, scoreMsg, Renderer::LIGHT_YELLOW);

            std::string rateMsg = "Accuracy: " + std::to_string(correctCount) + "/" + std::to_string(quizzes.size());
            int rateX = headerX + BOX_WIDTH / 2 - (int)rateMsg.size() / 2;
            r.PutString(rateX, endY + 4, rateMsg, Renderer::LIGHT_YELLOW);

            r.PutString(headerX, endY + 5, headerLine, Renderer::LIGHT_GRAY);

            std::string exitMsg = "[ Press any key to continue... ]";
            int exitX = headerX + BOX_WIDTH / 2 - (int)exitMsg.size() / 2;
            r.PutString(exitX, endY + 6, exitMsg, Renderer::LIGHT_YELLOW);
        }
    }

    bool IsFinished() const {
        return !isRunning;
    }

    const BattleResult& GetBattleResult() const { return BattleResultI; }

private:
    int Clamp(int value, int min, int max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
};
