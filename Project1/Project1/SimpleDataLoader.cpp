// SimpleDataLoader.cpp
#include "SimpleDataLoader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "Inventory.h"
#include "ItemFactory.h"
#include "Types.h"
#include "MapAsciiArtRepository.h"
#include "RewardRepository.h"
#include "Item.h" // Item* 사용 위해 (프로젝트 구조에 맞게 조정)

static std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> out;
    std::stringstream ss(s);
    std::string tok;
    while (std::getline(ss, tok, delim)) out.push_back(tok);
    return out;
}
static inline void trimCR(std::string& s) {
    if (!s.empty() && s.back() == '\r') s.pop_back();
}
static inline void stripUTF8BOM(std::string& s) {
    // UTF-8 BOM: EF BB BF
    if (s.size() >= 3 &&
        (unsigned char)s[0] == 0xEF &&
        (unsigned char)s[1] == 0xBB &&
        (unsigned char)s[2] == 0xBF) {
        s.erase(0, 3);
    }
}

// ✅ 기존 load는 그대로, 내부 공통 구현으로 연결
bool SimpleDataLoader::load(const std::string& path,
    Inventory& inv,
    MapAsciiArtRepository& artRepo) const {
    return loadImpl(path,
        [&](Item* item) {
            inv.add(item); // Inventory가 소유
        },
        artRepo
    );
}

// ✅ 추가: vector로 받기
bool SimpleDataLoader::loadToVector(const std::string& path,
    std::vector<Item*>& outItems,
    MapAsciiArtRepository& artRepo) const {
    outItems.clear();
    return loadImpl(path,
        [&](Item* item) {
            outItems.push_back(item); // 호출자가 소유(Shop/DropTable이 보통 소유)
        },
        artRepo
    );
}

// ✅ 공통 로더 구현(파싱 로직 한 번만 유지)
template <typename ItemConsumer>
bool SimpleDataLoader::loadImpl(const std::string& path,
    ItemConsumer&& consumeItem,
    MapAsciiArtRepository& artRepo) const {
    std::ifstream in(path);
    if (!in.is_open()) {
        std::cerr << "파일 열기 실패: " << path << "\n";
        return false;
    }

    std::string line;
    bool firstLine = true;

    std::string currentItemName;
    int currentPrice = 0;
    std::vector<Effect> currentEffects;

    auto flushItem = [&]() {
        if (currentItemName.empty() || currentEffects.empty()) return;

        const Effect& e = currentEffects.front();
        Item* item = nullptr;

        // 네 기존 규칙 그대로 유지
        if (e.type == EffectType::HealFlat) {
            item = ItemFactory::createHeal(currentItemName, currentPrice, e.value);
        }
        else if (e.duration > 0) {
            item = ItemFactory::createBuff(currentItemName, currentPrice, currentEffects);
        }
        else if (e.stat == StatType::Attack) {
            item = ItemFactory::createWeapon(currentItemName, currentPrice, e.value);
        }
        else if (e.stat == StatType::MaxHealth) {
            item = ItemFactory::createArmor(currentItemName, currentPrice, e.value);
        }

        if (item) {
            consumeItem(item); // ✅ 목적지로 전달
        }

        // 다음 ITEM을 위해 정리(ITEM 만날 때 clear하지만, 안전하게 한 번 더)
        // currentEffects.clear();  // 여기서 지우면 안 됨(ITEM 처리 타이밍과 겹칠 수 있음)
        };

    while (std::getline(in, line)) {
        trimCR(line);

        if (firstLine) {
            stripUTF8BOM(line);
            firstLine = false;
        }

        if (line.empty() || line[0] == '#') continue;

        if (line.rfind("ITEM|", 0) == 0) {
            // 이전 아이템 flush
            flushItem();

            currentEffects.clear();
            auto v = split(line, '|');
            if (v.size() < 3) continue; // 방어

            currentItemName = v[1];
            currentPrice = std::stoi(v[2]);
        }
        else if (line.rfind("EFFECT|", 0) == 0) {
            auto v = split(line, '|');
            if (v.size() < 5) continue; // 방어

            Effect e;
            e.type = (v[1] == "HealFlat") ? EffectType::HealFlat : EffectType::AddStatFlat;
            e.stat = (v[2] == "Attack") ? StatType::Attack :
                (v[2] == "MaxHealth") ? StatType::MaxHealth :
                StatType::Health;
            e.value = std::stoi(v[3]);
            e.duration = std::stoi(v[4]);

            currentEffects.push_back(e);
        }
        else if (line.rfind("ASCII|", 0) == 0) {
            auto name = line.substr(6);
            AsciiLines lines;
            while (std::getline(in, line)) {
                trimCR(line);
                if (line == "END") break;
                lines.push_back(line);
            }
            artRepo.upsert(name, lines);
        }
    }

    // 마지막 아이템 flush
    flushItem();
    return true;
}

bool SimpleDataLoader::loadRewards(
    const std::string& path,
    RewardRepository& rewards
) const {
    std::ifstream fin(path);
    if (!fin) return false;

    rewards.clear();

    std::string line;
    Rule currentRule;
    std::string currentMonster;

    while (std::getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;

        auto tokens = split(line, '|');

        if (tokens[0] == "REWARD") {
            currentMonster = tokens[1];
            currentRule = Rule{};
        }
        else if (tokens[0] == "PICK") {
            currentRule.picks.push_back({
                tokens[1],
                std::stoi(tokens[2])
                });
        }
        else if (tokens[0] == "GUARANTEE") {
            currentRule.guarantees.push_back(tokens[1]);
        }
        else if (tokens[0] == "ENDREWARD") {
            rewards.upsert(currentMonster, currentRule);
            currentMonster.clear();
        }
    }

    return true;
}
