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

void SetupConsole(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // 1. 창 크기 조절 막기 (가장 먼저 수행해서 유저 개입 차단)
    HWND consoleWindow = GetConsoleWindow();
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style = style & ~(WS_MAXIMIZEBOX | WS_SIZEBOX);
    SetWindowLong(consoleWindow, GWL_STYLE, style);

    // 2. 창 크기를 일단 최소로 줄임 (버퍼 줄일 때 에러 방지)
    SMALL_RECT rect = { 0, 0, 1, 1 };
    SetConsoleWindowInfo(hConsole, TRUE, &rect);

    // 3. 버퍼 크기 설정
    COORD coord = { (SHORT)width, (SHORT)height };
    SetConsoleScreenBufferSize(hConsole, coord);

    // 4. 창 크기를 버퍼에 맞춰서 확장
    rect = { 0, 0, (SHORT)(width - 1), (SHORT)(height - 1) };
    SetConsoleWindowInfo(hConsole, TRUE, &rect);

    DWORD mode;
    GetConsoleMode(hConsole, &mode);
    mode &= ~ENABLE_WRAP_AT_EOL_OUTPUT; // 줄 끝에서 자동 줄바꿈 끄기
    SetConsoleMode(hConsole, mode);
}

int main() {
    SetupConsole(160, 50);
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("cls");
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
    BattleRewardService rewardService(ctx.rewards, ctx.dropTable, ctx.inventory);

    

    GameManager game(ctx, rewardService);
    game.play();

    return 0;
}
