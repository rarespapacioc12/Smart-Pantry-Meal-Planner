#pragma once
#include <vector>

template <typename T, typename Predicate>
std::vector<T> filterItems(const std::vector<T>& items, Predicate pred) {
    std::vector<T> out;
    for (const T& item : items) {
        if (pred(item)) {
            out.push_back(item);
        }
    }
    return out;
}

template <typename T, typename KeyFn>
const T* minBy(const std::vector<T>& items, KeyFn key) {
    const T* best = nullptr;
    for (const T& item : items) {
        if (best == nullptr || key(item) < key(*best)) {
            best = &item;
        }
    }
    return best;
}
