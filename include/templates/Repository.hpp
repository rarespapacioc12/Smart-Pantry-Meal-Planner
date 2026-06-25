#pragma once
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <utility>
#include <vector>

// ============================================================================
// Tema 3 - CLASA SABLON (template class)
// ----------------------------------------------------------------------------
// Container generic in-memory folosit pentru a stoca si interoga uniform orice
// entitate de domeniu din Smart Pantry (StockItem din inventar, Recipe din
// reteta, etc.). Inlocuieste cod duplicat de tip "vector + cautare/filtrare"
// scris separat pentru fiecare tip.
//
// In main.cpp este instantiat de cel putin doua ori cu tipuri diferite:
//     Repository<StockItem>  si  Repository<Recipe>.
// ============================================================================
template <typename T>
class Repository {
private:
    std::vector<T> _items;

public:
    Repository() = default;
    explicit Repository(std::vector<T> items) : _items(std::move(items)) {}

    void add(const T& item) { _items.push_back(item); }
    void add(T&& item) { _items.push_back(std::move(item)); }

    std::size_t size() const { return _items.size(); }
    bool empty() const { return _items.empty(); }

    const std::vector<T>& all() const { return _items; }

    // Functie membru sablon: returneaza copii ale elementelor care satisfac
    // predicatul primit (orice callable T -> bool).
    template <typename Predicate>
    std::vector<T> filter(Predicate pred) const {
        std::vector<T> out;
        std::copy_if(_items.begin(), _items.end(), std::back_inserter(out), pred);
        return out;
    }

    // Primul element care satisface predicatul, sau nullptr daca nu exista.
    template <typename Predicate>
    const T* findIf(Predicate pred) const {
        auto it = std::find_if(_items.begin(), _items.end(), pred);
        return it == _items.end() ? nullptr : &(*it);
    }

    // Aplica o operatie pe fiecare element (vizualizare read-only).
    template <typename Fn>
    void forEach(Fn fn) const {
        for (const T& item : _items) {
            fn(item);
        }
    }
};
