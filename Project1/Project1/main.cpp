#include "GameManager.h"
#include <windows.h>
#include <iostream>

#include "ItemContext.h"
#include "ItemBootstrap.h"
#include "BattleRewardService.h"
static bool fileExists(const char* path) {
    DWORD attr = GetFileAttributesA(path);
    return (attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    if (!fileExists("data/shop_items.dat") ||
        !fileExists("data/inventory_items.dat") ||
        !fileExists("data/monster_drops.dat")) {
        std::cout << "[오류] data 폴더 또는 dat 파일을 찾을 수 없습니다.\n";
        std::cout << "실행 경로 기준으로 data/... 가 존재해야 합니다.\n";
        std::cout << "현재 실행 파일(.exe)과 같은 폴더에 data 폴더를 두거나,\n";
        std::cout << "VS 디버깅 '작업 디렉터리'를 프로젝트 루트로 설정하세요.\n";
        return 1;
    }

    ItemContext ctx;
    ItemBootstrap::build(
        ctx,
        "data/shop_items.dat",
        "data/inventory_items.dat",
        "data/monster_drops.dat"
    );
    std::cout << "[BOOT] loadRewards OK. rules.size=" << ctx.rewards.size()
        << " &ctx.rewards=" << &ctx.rewards << "\n" << std::flush;
    // ctx를 기반으로 보상 서비스 생성 (참조 주입)
    BattleRewardService rewardService(ctx.rewards, ctx.dropTable, ctx.inventory);

    // GameManager가 rewardService를 받아서 BattleService 생성 시 넘겨주도록 변경
    GameManager game(ctx, rewardService);
    game.play();

    return 0;
}
