#ifndef HISTORY_MAPS_HPP
#define HISTORY_MAPS_HPP

#include <stdexcept>
#include <limits>
#include "tape_enums.hpp"
#include <unordered_map>



struct TapeValMap {
private:
    std::unordered_map<int, TapeVal> map;
    int _minKey;
    int _maxKey;

    void updateMinMaxKey(int key);

public:
    TapeValMap();
    
    void removeUnchangedEntries();
    const TapeVal operator[](int key) const;
    TapeVal& operator[](int key);
    int minKey() const;
    int maxKey() const;

    int size() const;

    TapeValMap copy() const;
    TapeValMap offset_copy(int offset) const;
};

struct RunTimeValMap {
private:
    std::unordered_map<int, RunTimeVal> map;
    int _minKey;
    int _maxKey;

    void updateMinMaxKey(int key);

public:
    RunTimeValMap();

    const RunTimeVal operator[](int key) const;
    RunTimeVal& operator[](int key);
    int minKey() const;
    int maxKey() const;

    int size() const;

    RunTimeValMap copy() const;
    RunTimeValMap offset_copy(int offset) const;
};



#endif // HISTORY_MAPS_HPP
