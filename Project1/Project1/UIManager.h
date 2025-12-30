#pragma once
class UIManager {
public:
    enum class Type { 
        MainMenu, // 메인 메뉴
        Turn, // 라운드 표시
        Status, // 캐릭터 상태 표시
        Attack, // 공격 로그
        Crit, // 치명타 로그
        Damage, // 몇데미지
        Item, // 소비 아이템 사용
        Buff, // 버프 아이템
        Reward, // 보상
        Drop, // 드롭 아이템
        Shop, // 상점
        Level, // 레벨업
        Info, // 몬스터 정보
        Error // 자잘한 에러
    };
};
