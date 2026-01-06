#pragma once
#include "Scene.h"
#include "Monster.h"
#include "Character.h"
#include <vector>
#include <string>
#include "BattleResult.h"

class BossQuizScene : public Scene {
public:
    BossQuizScene(Monster* m, Character* p);
    void OnEnter() override;
    void Update(float deltaTime) override;
    void Render(Renderer& renderer) override;

    bool IsFinished() const { return sceneFinished; }
    const BattleResult& GetResult() const { return result; }

private:
    enum QuizState {
        SHOW_QUESTION,
        WAIT_INPUT,
        SHOW_RESULT
    };

    Monster* monster;
    Character* player;
    QuizState quizState = SHOW_QUESTION;

    std::vector<std::pair<std::string, std::string>> quizzes;
    int currentRound = 0;
    int maxRounds = 0;
    int playerCorrect = 0;
    int selectedOption = 0;  // 0,1,2

    bool sceneFinished = false;
    BattleResult result;

    void loadQuizzes();
    void nextRound();
};
