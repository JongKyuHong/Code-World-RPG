#include "BattleRewardService.h"

#include "RewardRepository.h"
#include "DropTable.h"
#include "Inventory.h"
#include "BattleResult.h"
#include "Monster.h"
#include "Item.h"

void BattleRewardService::applyDrops(const Monster* monster, BattleResult& result) {
    if (!monster) return;

    const std::string& monsterName = monster->getName();

    // 드랍 이름 결정 (GUARANTEE + 확률 PICK)
    const auto names = rewards.rollNames(monsterName);

    for (const auto& name : names) {
        Item* it = dropTable.cloneByName(name);
        if (!it) continue;

        inventory.add(it);
        result.droppedItemNames.push_back(name);
    }
}