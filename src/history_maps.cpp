#include "history_maps.hpp"
//tapeval
TapeValMap::TapeValMap() : _minKey(std::numeric_limits<int>::max()), _maxKey(std::numeric_limits<int>::min()) {}

void TapeValMap::updateMinMaxKey(int key, TapeVal value) {
    if (value != TapeVal::Unchanged) {
        if (key < _minKey) {
            _minKey = key;
        }
        if (key > _maxKey) {
            _maxKey = key;
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

    // Recalculate _minKey and _maxKey
    _minKey = std::numeric_limits<int>::max();
    _maxKey = std::numeric_limits<int>::min();
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

int TapeValMap::minKey() const {
    if (_minKey == std::numeric_limits<int>::max()) {
        throw std::runtime_error("No valid keys in the map.");
    }
    return _minKey;
}

int TapeValMap::maxKey() const {
    if (_maxKey == std::numeric_limits<int>::min()) {
        throw std::runtime_error("No valid keys in the map.");
    }
    return _maxKey;
}

//runtimeval
RunTimeValMap::RunTimeValMap() : _minKey(std::numeric_limits<int>::max()), _maxKey(std::numeric_limits<int>::min()) {}

void RunTimeValMap::updateMinMaxKey(int key, RunTimeVal value) {
    if (value != RunTimeVal::Unknown) {
        if (key < _minKey) {
            _minKey = key;
        }
        if (key > _maxKey) {
            _maxKey = key;
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

int RunTimeValMap::minKey() const {
    if (_minKey == std::numeric_limits<int>::max()) {
        throw std::runtime_error("No valid keys in the map.");
    }
    return _minKey;
}

int RunTimeValMap::maxKey() const {
    if (_maxKey == std::numeric_limits<int>::min()) {
        throw std::runtime_error("No valid keys in the map.");
    }
    return _maxKey;
}