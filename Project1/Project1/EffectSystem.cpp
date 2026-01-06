#include "EffectSystem.h"
static bool isRevertible(const Effect & e) {
	return e.type == EffectType::AddStatFlat /* || 추후 지속형 버프 타입들 */;
}
//아이템 duration(효과 지속 턴 수)에 따라서 활성화 필요한 경우
//active 벡터로 활성화 지속 시간 관리
void EffectSystem::applyFromItem(Character& c, const std::vector<Effect>& effects, EffectManager& manager) {
	for (const auto& e : effects) {
		manager.apply(c, e, +1);

		if (e.duration > 0 && isRevertible(e)) {
			active.push_back(Active{ e, e.duration });
		}
	}
}

//매 턴 아이템 효과(아이템 Effect의 duration)에 따라 지속 업데이트
void EffectSystem::updateActiveEffects(Character& c, EffectManager& manager) {
	for (int i = (int)active.size() - 1; i >= 0; --i) {
		auto& tickActive = active[i];
		tickActive.remaining--;
		if (tickActive.remaining <= 0) {
			manager.apply(c, tickActive.effect, -1);
			active.erase(active.begin() + i);
		}
	}
}

//전투 종료 시 모든 버프/디버프 해제용
void EffectSystem::clearAll(Character& c, EffectManager& manager) {
	for (auto& a : active) {
		manager.apply(c, a.effect, -1); // 지속 효과만 원복
	}
	active.clear();
}