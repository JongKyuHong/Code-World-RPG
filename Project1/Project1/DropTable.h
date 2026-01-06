#pragma once
#include <vector>
#include <string>
#include <cstdlib>

#include "Item.h"

class DropTable {
    std::vector<Item*> prototypes;

public:
    ~DropTable() { clear(); }

    void clear() {
        for (auto* p : prototypes) delete p;
        prototypes.clear();
    }

    void addPrototype(Item* p) {
        if (p) prototypes.push_back(p);
    }

    bool empty() const {
        return prototypes.empty();
    }

    // ----------------------------------------
    // (신규) 이름으로 prototype 찾아 clone
    // ----------------------------------------
    Item* cloneByName(const std::string& name) const {
        for (auto* p : prototypes) {
            if (!p) continue;
            if (p->getName() == name) {
                return p->clone(); // Inventory가 소유
            }
        }
        return nullptr; // 못 찾으면 nullptr
    }
};
