#pragma once
 enum class EquipSlot {
    Weapon = 0,
    Armor = 1
};

enum class StatType {
    Attack,
    Health,
    MaxHealth
};

enum class EffectType {
    HealFlat,        // 즉시 회복(+HP)
    AddStatFlat,     // 스탯 증감(장착/버프에서 사용)
    InvincibleHits,  // 1회 무적 같은 카운트형(적용 시스템이 있으면)
};

struct Effect {
    EffectType type;
    StatType stat;       //스탯 증감 효과에 사용
    int value = 0;       // 회복량/증감량/무적횟수 등
    int duration = 0;    // 턴/초 단위 지속(사용하는 시스템이 있을 때만 의미)
};

struct InventoryAction {
    enum Type {
        Exit,
        Use,
        Equip,
        Unequip
    } type = Exit;

    int index = -1;              // Use/Equip/ShowArt 할 때 인벤 인덱스
    EquipSlot slot = EquipSlot::Weapon;  // Unequip 할 때 슬롯
};

enum class BattleMode {
    Manual,
    Auto
};