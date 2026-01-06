#pragma once
#include <string>

struct ItemContext;

struct ItemBootstrap {
    static bool build(
        ItemContext& out,
        const std::string& shopDat,
        const std::string& invDat,
        const std::string& dropDat
    );
};