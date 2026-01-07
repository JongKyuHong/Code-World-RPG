#pragma once

#include "Character.h"
#include "Monster.h"
#include "BattleResult.h"
#include "UIManager.h"
#include "Types.h"
#include <vector>
#include <string>
#include <functional>
#include "EffectManager.h"
#include "EffectSystem.h"
#include "BattleRewardService.h"
#include "AutoBattleController.h"


class Inventory;

class BattleService
{
private:
    Character* player;
    Monster* monster;
    UIManager uiManager;

    // 드랍/인벤 처리 전담 서비스
    BattleRewardService& rewardService;

    // 전투 중 소비 아이템 지속 효과 관리
    EffectSystem effectSystem;
    EffectManager effectManager;

    // 전투 중 "인벤토리 열기"를 외부(GameManager)에게 맡김 (수동 모드용)
    std::function<void()> openInventoryCb;

      // 전투로그 남기기
    std::vector<std::string> battleLog;

    // 전투에서 쓸 인벤토리 (외부에서 주입)
    Inventory* inv_ = nullptr;

    // 전투 모드
    BattleMode mode_ = BattleMode::Manual;

    // 자동전투 AI
    AutoBattleController autoCtrl;

    // 모드 분기용 엔트리
    void playerTurn();

    // 수동/자동 턴 로직 분리
    void playerTurnManual();
    void playerTurnAuto();

    void monsterTurn();
    void displayBattleStatus();
    void addLog(const std::string& message);

    // 한 턴 종료 시 duration tick 처리(아이템 유효효과 지속 처리)
    void tickEndOfTurn();
      // 보스 퀴즈 전용
    BattleResult bossBattle();
    std::vector<std::pair<std::string, std::string>> getBossQuizzes(const std::string& bossName);

    BattleResult normalBattle();

public:


    BattleService(UIManager& ui, BattleRewardService& rs)
        : uiManager(ui)
        , rewardService(rs)
    {
    }
    BattleResult battle(Character* p, Monster* m);

    void displayLog();

    // 1. 크리티컬 여부 결정 (선언)
    bool rollCritical();

    // 2. 크리티컬 데미지 적용 (선언)
    int applyCriticalMultiplier(int baseDamage, bool isCritical);

    // 전투 중 인벤토리 연결(수동 모드에서만 주로 사용)
    void setOpenInventoryCallback(std::function<void()> cb) {
        openInventoryCb = std::move(cb);
    }

    // 자동전투에서 아이템 사용하려면 반드시 주입해야 함
    void setInventory(Inventory* inv) { inv_ = inv; }

    // 모드 전환
    void setBattleMode(BattleMode m) { mode_ = m; }
    BattleMode getBattleMode() const { return mode_; }

    void waitNextBeat();   // 또는 waitAfterUi() 같은 이름
};


