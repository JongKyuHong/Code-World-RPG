#pragma once
#include <string>
#include <vector>

class Monster;
class DropTable;
class RewardRepository;
class Inventory;
struct BattleResult;

class BattleRewardService {
public:
    BattleRewardService(RewardRepository& rewards, DropTable& dropTable, Inventory& inventory)
        : rewards(rewards)
        , dropTable(dropTable)
        , inventory(inventory)
    {
    }

    // ✅ 승리 시 드랍만 처리 (RewardManager의 gold/exp 로직은 그대로 유지)
    void applyDrops(const Monster* monster, BattleResult& result);

private:
    RewardRepository& rewards;
    DropTable& dropTable;
    Inventory& inventory;
};
