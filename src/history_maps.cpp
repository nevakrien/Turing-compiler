#include "history_maps.hpp"
//tapeval
TapeValMap::TapeValMap() : _minKey(std::numeric_limits<int>::max()), _maxKey(std::numeric_limits<int>::min()) {}

void TapeValMap::updateMinMaxKey(int key) {
    if (key < _minKey) {
        _minKey = key;
    }
    if (key > _maxKey) {
        _maxKey = key;
    }

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
        updateMinMaxKey(kvp.first);
    }
}

const TapeVal TapeValMap::operator[](int key) const {
    auto it = map.find(key);
    if (it != map.end()) {
        return it->second;
    } else {
        return TapeVal::Unchanged;
    }
}

TapeVal& TapeValMap::operator[](int key) {
    updateMinMaxKey(key);
    return map[key];
}

int TapeValMap::minKey() const {
    // if (_minKey == std::numeric_limits<int>::max()) {
    //     throw std::runtime_error("No valid keys in the map.");
    // }
    return _minKey;
}

int TapeValMap::maxKey() const {
    // if (_maxKey == std::numeric_limits<int>::min()) {
    //     throw std::runtime_error("No valid keys in the map.");
    // }
    return _maxKey;
}

//runtimeval
RunTimeValMap::RunTimeValMap() : _minKey(std::numeric_limits<int>::max()), _maxKey(std::numeric_limits<int>::min()) {}

void RunTimeValMap::updateMinMaxKey(int key) {
    if (key < _minKey) {
        _minKey = key;
    }
    if (key > _maxKey) {
        _maxKey = key;
    }
}


const RunTimeVal RunTimeValMap::operator[](int key) const {
    auto it = map.find(key);
    if (it != map.end()) {
        return it->second;
    } else {
        return RunTimeVal::Unknown;
    }
}

RunTimeVal& RunTimeValMap::operator[](int key) {
    if(key<_minKey||key>_maxKey){
        map[key]=RunTimeVal::Unknown;
        updateMinMaxKey(key);
    }
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

// //COPY STUFF

TapeValMap TapeValMap::copy() const {
    return *this; // Use the implicitly defined copy constructor
}

TapeValMap TapeValMap::offset_copy(int offset) const {
    TapeValMap newMap;
    for (const auto& kvp : map) {
        int newKey = kvp.first + offset;
        newMap.map[newKey] = kvp.second;
    }
    
    // Adjust min and max keys with the offset
    if (_minKey != std::numeric_limits<int>::max()) {
        newMap._minKey = _minKey + offset;
    } else {
        newMap._minKey = std::numeric_limits<int>::max();
    }
    
    if (_maxKey != std::numeric_limits<int>::min()) {
        newMap._maxKey = _maxKey + offset;
    } else {
        newMap._maxKey = std::numeric_limits<int>::min();
    }
    
    return newMap;
}

RunTimeValMap RunTimeValMap::offset_copy(int offset) const {
    RunTimeValMap newMap;
    for (const auto& kvp : map) {
        int newKey = kvp.first + offset;
        newMap.map[newKey] = kvp.second;
    }
    
    // Adjust min and max keys with the offset
    if (_minKey != std::numeric_limits<int>::max()) {
        newMap._minKey = _minKey + offset;
    } else {
        newMap._minKey = std::numeric_limits<int>::max();
    }
    
    if (_maxKey != std::numeric_limits<int>::min()) {
        newMap._maxKey = _maxKey + offset;
    } else {
        newMap._maxKey = std::numeric_limits<int>::min();
    }
    
    return newMap;
}

// RunTimeValMap copy method
RunTimeValMap RunTimeValMap::copy() const {
    return *this; // Use the implicitly defined copy constructor
}

// SIZE 
int TapeValMap::size() const {
    return map.size(); // Use the implicitly defined copy constructor
}

// RunTimeValMap copy method
int RunTimeValMap::size() const {
    return map.size(); // Use the implicitly defined copy constructor
}