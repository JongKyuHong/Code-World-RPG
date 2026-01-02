#include "SimpleDataLoader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "Inventory.h"
#include "ItemFactory.h"
#include "Types.h"
#include "MapAsciiArtRepository.h"

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
bool SimpleDataLoader::load(const std::string& path,
    Inventory& inv,
    MapAsciiArtRepository& artRepo) const {
    std::ifstream in(path);
    if (!in.is_open()) {
        std::cerr << "파일 열기 실패: " << path << "\n";
        return false;
    }

    std::string line;
    std::string currentItemName;
    int currentPrice = 0;
    std::vector<Effect> currentEffects;

    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;

        if (line.rfind("ITEM|", 0) == 0) {
            // 이전 아이템 flush
            if (!currentItemName.empty()) {
                // 아이템 생성
                const Effect& e = currentEffects.front();
                Item* item = nullptr;

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

                if (item) inv.add(item);
            }

            currentEffects.clear();
            auto v = split(line, '|');
            currentItemName = v[1];
            currentPrice = std::stoi(v[2]);
        }

        else if (line.rfind("EFFECT|", 0) == 0) {
            auto v = split(line, '|');
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
            while (std::getline(in, line) && line != "END") {
                lines.push_back(line);
            }
            artRepo.upsert(name, lines);
        }
    }

    // 마지막 아이템 flush
    if (!currentItemName.empty() && !currentEffects.empty()) {
        const Effect& e = currentEffects.front();
        Item* item = nullptr;

        if (e.type == EffectType::HealFlat)
            item = ItemFactory::createHeal(currentItemName, currentPrice, e.value);
        else if (e.duration > 0)
            item = ItemFactory::createBuff(currentItemName, currentPrice, currentEffects);
        else if (e.stat == StatType::Attack)
            item = ItemFactory::createWeapon(currentItemName, currentPrice, e.value);
        else if (e.stat == StatType::MaxHealth)
            item = ItemFactory::createArmor(currentItemName, currentPrice, e.value);

        if (item) inv.add(item);
    }

    return true;
}
