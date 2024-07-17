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

    void updateMinMaxKey(int key, TapeVal value);

public:
    TapeValMap();
    void insert(int key, TapeVal value);
    void removeUnchangedEntries();
    TapeVal operator[](int key) const;
    TapeVal& operator[](int key);
    int minKey() const;
    int maxKey() const;

    int size() const;

    TapeValMap copy() const; // Declare the copy method
};

struct RunTimeValMap {
private:
    std::unordered_map<int, RunTimeVal> map;
    int _minKey;
    int _maxKey;

    void updateMinMaxKey(int key, RunTimeVal value);

public:
    RunTimeValMap();
    void insert(int key, RunTimeVal value);
    RunTimeVal operator[](int key) const;
    RunTimeVal& operator[](int key);
    int minKey() const;
    int maxKey() const;

    int size() const;

    RunTimeValMap copy() const; // Declare the copy method
};



#endif // HISTORY_MAPS_HPP
