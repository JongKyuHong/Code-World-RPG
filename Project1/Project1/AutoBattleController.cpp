#include "AutoBattleController.h"

#include "Character.h"
#include "Monster.h"
#include "Inventory.h"
#include "Item.h"

AutoBattleController::AutoBattleController(AutoBattleConfig cfg)
    : cfg_(cfg)
{
}

void AutoBattleController::onBattleStart(bool /*isBoss*/)
{
    usedItemsThisBattle_ = 0;
    usedItemsThisTurn_ = 0;
    turnCount_ = 0;
}

void AutoBattleController::onTurnStart(int turnCount)
{
    turnCount_ = turnCount;
    usedItemsThisTurn_ = 0;
}

void AutoBattleController::onItemUsed()
{
    usedItemsThisTurn_++;
    usedItemsThisBattle_++;
}

bool AutoBattleController::canUseItemNow() const
{
    if (usedItemsThisTurn_ >= cfg_.maxItemsPerTurn) return false;
    if (usedItemsThisBattle_ >= cfg_.maxItemsPerBattle) return false;
    return true;
}

bool AutoBattleController::hpBelow(Character& player, double ratio) const
{
    const int hp = player.getHealth();
    const int maxHp = player.getMaxHealth();
    if (maxHp <= 0) return false;
    return (double)hp / (double)maxHp <= ratio;
}

PlayerAction AutoBattleController::decide(Character& player, Monster& /*monster*/, Inventory& inv, bool isBoss)
{
    if (canUseItemNow()) {
        const bool emergency = hpBelow(player, cfg_.emergencyHpRatio);
        const bool needHeal = hpBelow(player, cfg_.healHpRatio);

        // ✅ 1) 응급이면 무조건 힐 최우선
        if (emergency) {
            int idx = findBestHealItemIndex(inv);
            if (idx >= 0) return { PlayerActionType::UseItem, idx };
        }

        // ✅ 2) (완화) 전투 초반/보스전이면 버프도 적극 사용
        if (cfg_.allowStatBuffAutoUse && !emergency) {
            const bool earlyTurn = (turnCount_ <= 2);
            if (earlyTurn || isBoss) {
                int idx = findBestBuffItemIndex(inv);
                if (idx >= 0) return { PlayerActionType::UseItem, idx };
            }
        }

        // ✅ 3) 그 다음에 힐(needHeal이면)
        if (needHeal) {
            int idx = findBestHealItemIndex(inv);
            if (idx >= 0) return { PlayerActionType::UseItem, idx };
        }
    }

    return { PlayerActionType::Attack, -1 };
}


static int buffPriority(StatType s) {
    // 우선순위: 공격 > 최대체력 > 체력
    switch (s) {
    case StatType::Attack:    return 3;
    case StatType::MaxHealth: return 2;
    case StatType::Health:    return 1;
    default: return 0;
    }
}

int AutoBattleController::findBestBuffItemIndex(Inventory& inv) const
{
    const auto& items = inv.getItems();

    int bestIdx = -1;
    int bestScore = -1;

    // 예) 전투 초반에만 버프 쓰고 싶으면:
    // if (turnCount_ > 2) return -1;

    for (int i = 0; i < (int)items.size(); ++i) {
        const Item* it = items[i];
        if (!isCandidateConsumable(it)) continue;

        // 아이템이 여러 효과를 갖는 구조이므로, 그 중 AddStatFlat만 점수화
        for (const Effect& e : getEffects(it)) {
            if (e.type != EffectType::AddStatFlat) continue;
            if (e.value <= 0) continue; // 음수 버프면 제외

            int score = 0;

            // 스탯 우선순위 반영
            score += buffPriority(e.stat) * 1000;

            // value가 클수록 선호
            score += e.value * 10;

            // duration이 있으면 조금 선호(지속버프)
            score += (e.duration > 0 ? std::min(e.duration, 50) : 0);

            if (score > bestScore) {
                bestScore = score;
                bestIdx = i;
            }
        }
    }

    return bestIdx;
}


bool AutoBattleController::isCandidateConsumable(const Item* it) const
{
    if (!it) return false;

    // Inventory::useItem 정책과 동일: 장비는 사용 대상 아님
    if (it->isEquipItem()) return false;

    // 자동 사용 금지 정책 훅
    if (!isAutoUsableAllowed(it)) return false;

    return true;
}

const std::vector<Effect>& AutoBattleController::getEffects(const Item* it) const
{
    return it->getEffect(); // vector<Effect>
}

bool AutoBattleController::isAutoUsableAllowed(const Item* /*it*/) const
{
    // TODO: GUARANTEE / 희귀 아이템 자동 사용 금지 로직
    return true;
}
