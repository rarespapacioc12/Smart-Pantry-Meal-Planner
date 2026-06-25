#pragma once
#include <vector>

// ============================================================================
// Tema 3 - FUNCTII SABLON (template functions)
// ----------------------------------------------------------------------------
// Algoritmi generici reutilizabili pe orice tip de element din domeniu.
// In main.cpp sunt instantiati pentru cel putin doua tipuri diferite
// (StockItem si Recipe), demonstrand reutilizarea fara copy-paste.
// ============================================================================
namespace algo {

// Filtrare generica a unui vector dupa un predicat (callable T -> bool).
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

// Elementul cu cheia minima, calculata de extractorul `key` (callable T -> comparabil).
// Returneaza nullptr pentru un vector gol.
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

}  // namespace algo
