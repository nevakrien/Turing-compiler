#ifndef HISTORY_MAPS_HPP
#define HISTORY_MAPS_HPP

class TapeValMap {
private:
    std::unordered_map<int, TapeVal> map;
    int minKey;
    int maxKey;

    void updateMinMaxKey(int key, TapeVal value);

public:
    TapeValMap();
    void insert(int key, TapeVal value);
    void removeUnchangedEntries();
    TapeVal operator[](int key) const;
    TapeVal& operator[](int key);
    int getMinKey() const;
    int getMaxKey() const;
};

class RunTimeValMap {
private:
    std::unordered_map<int, RunTimeVal> map;
    int minKey;
    int maxKey;

    void updateMinMaxKey(int key, RunTimeVal value);

public:
    RunTimeValMap();
    void insert(int key, RunTimeVal value);
    RunTimeVal operator[](int key) const;
    RunTimeVal& operator[](int key);
    int getMinKey() const;
    int getMaxKey() const;
};

//tapeval
TapeValMap::TapeValMap() : minKey(std::numeric_limits<int>::max()), maxKey(std::numeric_limits<int>::min()) {}

void TapeValMap::updateMinMaxKey(int key, TapeVal value) {
    if (value != TapeVal::Unchanged) {
        if (key < minKey) {
            minKey = key;
        }
        if (key > maxKey) {
            maxKey = key;
        }
    }
}

void TapeValMap::insert(int key, TapeVal value) {
    map[key] = value;
    updateMinMaxKey(key, value);
}

void TapeValMap::removeUnchangedEntries() {
    for (auto it = map.begin(); it != map.end(); ) {
        if (it->second == TapeVal::Unchanged) {
            it = map.erase(it);
        } else {
            ++it;
        }
    }

    // Recalculate minKey and maxKey
    minKey = std::numeric_limits<int>::max();
    maxKey = std::numeric_limits<int>::min();
    for (const auto& kvp : map) {
        updateMinMaxKey(kvp.first, kvp.second);
    }
}

TapeVal TapeValMap::operator[](int key) const {
    auto it = map.find(key);
    if (it != map.end()) {
        return it->second;
    } else {
        return TapeVal::Unchanged;
    }
}

TapeVal& TapeValMap::operator[](int key) {
    return map[key];
}

int TapeValMap::getMinKey() const {
    if (minKey == std::numeric_limits<int>::max()) {
        throw std::runtime_error("No valid keys in the map.");
    }
    return minKey;
}

int TapeValMap::getMaxKey() const {
    if (maxKey == std::numeric_limits<int>::min()) {
        throw std::runtime_error("No valid keys in the map.");
    }
    return maxKey;
}

//runtimeval
RunTimeValMap::RunTimeValMap() : minKey(std::numeric_limits<int>::max()), maxKey(std::numeric_limits<int>::min()) {}

void RunTimeValMap::updateMinMaxKey(int key, RunTimeVal value) {
    if (value != RunTimeVal::Unknown) {
        if (key < minKey) {
            minKey = key;
        }
        if (key > maxKey) {
            maxKey = key;
        }
    }
}

void RunTimeValMap::insert(int key, RunTimeVal value) {
    map[key] = value;
    updateMinMaxKey(key, value);
}

RunTimeVal RunTimeValMap::operator[](int key) const {
    auto it = map.find(key);
    if (it != map.end()) {
        return it->second;
    } else {
        return RunTimeVal::Unknown;
    }
}

RunTimeVal& RunTimeValMap::operator[](int key) {
    return map[key];
}

int RunTimeValMap::getMinKey() const {
    if (minKey == std::numeric_limits<int>::max()) {
        throw std::runtime_error("No valid keys in the map.");
    }
    return minKey;
}

int RunTimeValMap::getMaxKey() const {
    if (maxKey == std::numeric_limits<int>::min()) {
        throw std::runtime_error("No valid keys in the map.");
    }
    return maxKey;
}

#endif // HISTORY_MAPS_HPP
