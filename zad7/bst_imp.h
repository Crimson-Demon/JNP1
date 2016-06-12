//Author: Marcin Kania


#ifndef BST_IMP_H
#define BST_IMP_H

#include "bst.h"
#include <exception> //for std::logic_error
#include <vector>

/*Pomocnicza funkcja sluzaca do range konstruktora.
  Mamy wykasowany operator = przez move constructor
  wiec trzeba na wkaznikach zrobic konstrukcje.
  Raw sa niebezpieczne, wiec najpewniej za pomoca smart_ptr
*/
template <typename T, typename Iter>
BST<T> createBST(Iter it1, Iter it2) {
    std::unique_ptr<BST<T>> bst(new BST<T>());
    for(auto it = it1; it != it2; it++) {
        std::unique_ptr<BST<T>> bst2(new BST<T>(*bst + *it));
        bst = std::move(bst2);
    }
    return *bst;
}

/*KONSTRUKTURY
*/
template <typename T>
BST<T>::BST(std::initializer_list<T> init_list): BST<T>(init_list.begin(), init_list.end()) { }

template <typename T>
template <typename Iter>
BST<T>::BST(Iter iter1, Iter iter2):  BST(createBST<T, Iter>(iter1, iter2)) { }
/*To ponizej jest eleganckie i fajne, ale dziala tylko dla bidirectional iterator :(
template <typename T>
template <typename Iter>
BST<T>::BST(Iter iter1, Iter iter2): 
m_root(iter1 != iter2 ? (*(std::prev(iter2, 1)) + BST<T>(iter1, std::prev(iter2, 1))).m_root : BST<T>().m_root) { }
*/

template <typename T>
BST<T>::BST(T value, BST left, BST right): 
m_root(std::make_shared<const Node>(value, left.m_root, right.m_root)) { }

/*METODY
*/
template <typename T>
BST<T> BST<T>::left() const {
    if(empty())
        throw std::logic_error("Empty tree in call to left()");
    else
        return BST(m_root->m_left);
}

template <typename T>
BST<T> BST<T>::right() const {
    if(empty())
        throw std::logic_error("Empty tree in call to right()");
    else
        return BST(m_root->m_right);
}

template <typename T>
T const & BST<T>::value() const {
    if(empty())
        throw std::logic_error("Empty tree in call to value()");
    else
        return m_root->m_value;
}

template <typename T>
bool BST<T>::empty() const {
    return m_root == nullptr;
}

template <typename T>
T const & BST<T>::min() const {
    if(empty())
        throw std::logic_error("Empty tree in call to min()");
    else
        return left().empty() ? value() : left().min();
}

template <typename T>
T const & BST<T>::max() const {
    if(empty())
        throw std::logic_error("Empty tree in call to max()");
    else
        return right().empty() ? value() : right().max();
}

template <typename T>
template <typename Acc, typename Functor>
Acc BST<T>::fold(Acc a, Functor f) const {
    return empty() ? a : right().fold(f(left().fold(a, f), value()), f);
}

template <typename T>
BST<T> BST<T>::find(T const & t) const {
    if(empty())
        throw std::logic_error("Element not found");
    else {
        if(value() == t)
            return *this;
        else
            return t < value() ? left().find(t) : right().find(t);
    }
}

template <typename T>
std::size_t BST<T>::size() const {
    return empty() ? 0 : left().size() + right().size() + 1;
}

template <typename T>
std::size_t BST<T>::height() const {
    return empty() ? 0 : 1 + std::max(left().height(), right().height());
}

/*FUNKCJE PUBLICZNE
*/
template <typename T>
BST<T> spine(BST<T> bst) {
    return bst.fold(BST<T>(), [] (BST<T> acc, T t) { return BST<T>(t, acc, BST<T>()); });
}

template <typename T>
BST<T> operator+(T t, BST<T> bst) {
    if(bst.empty()) {
        return BST<T>(t, BST<T>(), BST<T>());
    } else {
        if(t <= bst.value()) {
            return BST<T>(bst.value(), t + bst.left(), bst.right());
        } else {
            return BST<T>(bst.value(), bst.left(), t + bst.right());
        }
    }
}

template <typename T>
BST<T> operator+(BST<T> bst, T t) {
    return t + bst;
}

template <typename T>
std::ostream & operator<<(std::ostream & stream, BST<T> bst) {
    bst.fold(0, [&stream] (int acc, T t) -> int { stream << t << " "; return 0; });
    return stream;
}

template <typename T>
T max_diff(BST<T> bst) {
    if(bst.size() < 2) {
        throw std::logic_error("Tree too small in call to max_diff()");
    } else {
        T max = 0;
        bst.fold(bst.value(), [&max] (T prev, T curr) -> T { max = std::max(curr - prev, max); return curr;});
        return max;
    }
}

#endif /*BST_IMP_H*/