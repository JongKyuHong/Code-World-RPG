#include "RewardRepository.h"
#include <iostream>
#include <random>

// =======================================================
// 기본 API
// =======================================================

void RewardRepository::clear() {
    rules.clear();
}

void RewardRepository::upsert(const std::string& monsterName, const Rule& rule) {
    rules[monsterName] = rule;
}

const Rule* RewardRepository::find(const std::string& monsterName) const {
    auto it = rules.find(monsterName);
    return (it == rules.end()) ? nullptr : &it->second;
}

// =======================================================
// RNG helpers (thread-safe)
// =======================================================

static std::mt19937& globalRng() {
    static thread_local std::mt19937 rng{ std::random_device{}() };
    return rng;
}

static int randInt(int lo, int hi) {
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(globalRng());
}

// =======================================================
// 드랍 규칙
// - GUARANTEE : 항상 지급
// - PICK      : 30% 확률로 가중치 기반 1개 지급
// =======================================================

std::vector<std::string> RewardRepository::rollNames(const std::string& monsterName) const {
    std::vector<std::string> result;

    const Rule* rule = find(monsterName);
    if (!rule) return result;

    // 1) GUARANTEE: 항상 지급
    for (const auto& name : rule->guarantees) {
        if (!name.empty()) {
            result.push_back(name);
        }
    }

    // 2) PICK: 30% 확률로 1개 추가 드랍
    const int chancePercent = 30;
    const int gate = randInt(1, 100);

    // 70%는 여기서 종료(guarantee만)
    if (gate > chancePercent) {
        return result;
    }

    // 3) PICK 목록 가중치 기반 1개 선택
    int totalWeight = 0;
    for (const auto& p : rule->picks) {
        if (p.second > 0) totalWeight += p.second;
    }
    if (totalWeight <= 0) {
        return result; // picks가 없거나 전부 0이면 guarantee만
    }

    int roll = randInt(1, totalWeight);

    for (const auto& p : rule->picks) {
        if (p.second <= 0) continue;

        roll -= p.second;
        if (roll <= 0) {
            if (!p.first.empty()) result.push_back(p.first);
            break;
        }
    }

    return result;
}