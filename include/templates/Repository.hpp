#pragma once
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <utility>
#include <vector>

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

    template <typename Predicate>
    std::vector<T> filter(Predicate pred) const {
        std::vector<T> out;
        std::copy_if(_items.begin(), _items.end(), std::back_inserter(out), pred);
        return out;
    }

    template <typename Predicate>
    const T* findIf(Predicate pred) const {
        auto it = std::find_if(_items.begin(), _items.end(), pred);
        return it == _items.end() ? nullptr : &(*it);
    }

    template <typename Fn>
    void forEach(Fn fn) const {
        for (const T& item : _items) {
            fn(item);
        }
    }
};
