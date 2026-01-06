#pragma once
#include <string>
#include <unordered_map>
#include <vector>

struct Rule {
    std::vector<std::string> guarantees;
    std::vector<std::pair<std::string, int>> picks;
};

class RewardRepository {
public:
    void clear();
    void upsert(const std::string& monsterName, const Rule& rule);
    const Rule* find(const std::string& monsterName) const;
    size_t size() const { return rules.size(); }
    // GUARANTEE + 30% PICK 1개
    std::vector<std::string> rollNames(const std::string& monsterName) const;

private:
    std::unordered_map<std::string, Rule> rules;
};
