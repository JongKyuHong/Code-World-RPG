#include "ItemBootstrap.h"
#include "ItemContext.h"
#include "SimpleDataLoader.h"
#include "Shop.h"
#include "Item.h"

#include <vector>
#include <iostream>

bool ItemBootstrap::build(
    ItemContext& ctx,
    const std::string& shopDat,
    const std::string& invDat,
    const std::string& dropDat
) {
    // build가 진짜 호출되는지 무조건 확인
    std::cout << "[BOOT] build ENTER ctx=" << &ctx
        << " &ctx.rewards=" << &ctx.rewards << "\n" << std::flush;

    ctx.shop = Shop::getInstance();
    if (!ctx.shop) {
        std::cout << "[BOOT][ERR] Shop::getInstance() returned nullptr\n" << std::flush;
        return false;
    }

    SimpleDataLoader loader;

    // 1) 인벤토리 초기 아이템 (Inventory가 소유)
    if (!loader.load(invDat, ctx.inventory, ctx.artRepo)) {
        std::cout << "[BOOT][ERR] loader.load(invDat) failed: " << invDat << "\n" << std::flush;
        return false;
    }
    std::cout << "[BOOT] inv loaded OK\n" << std::flush;

    // 2) 상점 아이템 (Shop이 소유)
    {
        std::vector<Item*> items;
        if (!loader.loadToVector(shopDat, items, ctx.artRepo)) {
            std::cout << "[BOOT][ERR] loader.loadToVector(shopDat) failed: " << shopDat << "\n" << std::flush;
            for (auto* p : items) delete p;
            return false;
        }

        std::cout << "[BOOT] shop items loaded: " << items.size() << "\n" << std::flush;

        ctx.shop->clearItems();
        for (auto* it : items) {
            ctx.shop->addItem(it);
        }
    }

    // 3) 드랍 아이템 프로토타입 + ASCII + REWARD 규칙
    {
        std::vector<Item*> items;
        if (!loader.loadToVector(dropDat, items, ctx.artRepo)) {
            std::cout << "[BOOT][ERR] loader.loadToVector(dropDat prototypes) failed: " << dropDat << "\n" << std::flush;
            for (auto* p : items) delete p;
            return false;
        }

        std::cout << "[BOOT] drop prototypes loaded: " << items.size() << "\n" << std::flush;

        ctx.dropTable.clear();
        for (auto* it : items) {
            ctx.dropTable.addPrototype(it);
        }

        // REWARD 규칙
        ctx.rewards.clear();
        std::cout << "[BOOT] before loadRewards &ctx.rewards=" << &ctx.rewards << "\n" << std::flush;

        if (!loader.loadRewards(dropDat, ctx.rewards)) {
            std::cout << "[BOOT][ERR] loader.loadRewards failed: " << dropDat << "\n" << std::flush;
            return false;
        }

        //여기서 rewards가 진짜 채워졌는지 확인하려면 RewardRepository::size() 필요
        // (아직 size()가 없다면 우선 주소라도 찍혀서 "같은 객체"인지 확인 가능)
        std::cout << "[BOOT] loadRewards OK. &ctx.rewards=" << &ctx.rewards
            << " (add RewardRepository::size() to print count)\n" << std::flush;
    }

    std::cout << "[BOOT] build SUCCESS\n" << std::flush;
    return true;
}
