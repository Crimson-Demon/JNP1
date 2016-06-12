//Authors: Marcin Kania, Filip Stefaniuk

#ifndef _PRIORITY_QUEUE_HH_
#define _PRIORITY_QUEUE_HH_

#include <set>
#include <memory>
#include <utility>
#include <exception>

class PriorityQueueEmptyException : public std::exception {

    public:

        virtual char const* what() const throw() {
                return "Priority queue is empty!";
        };
};

class PriorityQueueNotFoundException : public std::exception {

    public:

        virtual char const* what() const throw() {
                return "Key not found in priority queue!";
        };
};

template <typename K, typename V>
class PriorityQueue {

        typedef std::shared_ptr<std::pair<K, V>> Item;


        //Komparatory, aby kolejka działała poprawnie wymagane jest jedynie
        //zdefiniowanie operacji "<"(tak samo jak dla kontenerów STL).
        struct cmp_by_key{
            bool operator()(Item const & a, Item const & b) const {
                if(!(a->first < b->first) && !(b->first < a->first) )
                    return a->second < b->second;
                return a->first < b->first;
            }
        };

        struct cmp_by_val{
            bool operator()(Item const & a, Item const & b) const {
                if(!(a->second < b->second) && !(b->second < a->second))
                    return a->first < b->first;
                return a->second < b->second;
            }
        };

        //Multisety w których wartości są posortowane w pierwszym
        //po kluczu w drugim po wartości.
        typedef std::multiset<Item, cmp_by_key> ByKeys;
        typedef std::multiset<Item, cmp_by_val> ByValues;

        ByKeys by_keys;
        ByValues by_values;


    public:

        typedef size_t size_type;
        typedef K key_type;
        typedef V value_type;

        //Konstruktory.
        PriorityQueue() noexcept : by_keys(), by_values() {}
        PriorityQueue(PriorityQueue const & queue)
        : by_keys(queue.by_keys), by_values(queue.by_values) {}
        PriorityQueue(PriorityQueue && queue)
        : by_keys(std::move(queue.by_keys)), by_values(std::move(queue.by_values)) {}

        //Operator =.
        PriorityQueue<K, V>& operator=(PriorityQueue<K, V> queue) {
            if(*this != queue)
                queue.swap(*this);
            return (*this);
        }

        //Sprawdza czy kolejka priorytetowa jest pusta.
        bool empty() const {
            return by_keys.empty();
        }

        //Zwraca ilość elementów w kolejce.
        size_type size() const {
            return by_keys.size();
        }

        //Wstawia nowy element do kolejki.
        //Sprawdzanie czy element jest w kolejce [O(log size())].
        //Wstawienie elementu do zbioru posortowanego po kluczach [O(1)].
        //Wstawienie elementu do zbioru posortowanego po wartościach [O(log size())].
        void insert(const K& key, const V& value) {
            Item item = std::make_shared<std::pair<K, V>>(std::make_pair(key, value));
            typename ByKeys::iterator next = by_keys.lower_bound(item);
            if(next != by_keys.end() &&
            !(next->get()->first < key) &&  !(key < next->get()->first) &&
            !(next->get()->second < value) && !(value < next->get()->second))
                item = *next;

            by_keys.insert(next, item);

            try {
                by_values.insert(item);
            }
            catch(std::exception &e) {
                by_keys.erase(item);
                throw e;
            }
        }

        //Funkcje zwracające minimalne(maksymalne) wartości i klucze.
        //W czasie [O(1)].
        //Jesli kolejka jest pusta, to rzuca wyjatek
        //PriorityQueueEmptyException().
        const V& minValue() const {
            if(empty())
                throw PriorityQueueEmptyException();
            return by_values.begin()->get()->second;
        }

        const V& maxValue() const {
            if(empty())
                throw PriorityQueueEmptyException();
            return by_values.rbegin()->get()->second;
        }

        const K& minKey() const {
            if(empty())
                throw PriorityQueueEmptyException();
            return by_values.begin()->get()->first;
        }

        const K& maxKey() const {
            if(empty())
                throw PriorityQueueEmptyException();
            return by_values.rbegin()->get()->first;
        }

        //Funkcje usuwające element z największą(najmniejszą) wartością.
        //Usuwanie ze zbioru posortowanego po wartościach [O(1)].
        //Usuwanie ze zbioru posortowanego po kluczach [O(log size())].
        void deleteMin() {
            if(!empty()) {
                by_keys.erase(by_keys.find(*by_values.begin()));
                by_values.erase(by_values.begin());
            }
        }

        void deleteMax() {
            if(!empty()) {
                by_keys.erase(by_keys.find(*by_values.rbegin()));
                by_values.erase(--by_values.end());
            }
        }

        //Zmienia wartość elementu o zadanym kluczu.
        //Wyszukuje element który jest następny po zadanym [O(log size())].
        //Jeśli nie ma takiego, albo ma inny klucz to sprawdza czy poprzedni element
        //nie ma odpowiadającego klucza.
        //Jeśli jest w kolejce element o takim samym kluczu i wartości na jaką,
        //ma być zmiana to nie dzieje się nic.
        //Jeśli jest o takim samym kluczu ale innej wartości to jest on usuwany
        //czas [O(log size())] , i wstawiany nowy [O(log size())].
        //Jeśli nie ma elementu o zadanym kluczu to jest zgłaszany wyjątek
        //PriorityQueueNotFoundException().
        void changeValue(const K& key, const V& value) {
            Item item = std::make_shared<std::pair<K, V>>(std::make_pair(key, value));
            typename ByKeys::iterator next = by_keys.lower_bound(item);
            if(next == by_keys.end() || next->get()->first != key)
                --next;
            if(next->get()->first == key && next->get()->second != value) {
                    by_values.erase(by_values.find(*next));
                    by_keys.erase(next);
                    by_values.insert(item);
                    by_keys.insert(item);
            }
            else if(next->get()->first != key) {
                throw PriorityQueueNotFoundException();
            }
        }

        //Scala dwie kolejki.
        //Wstawiane jest queue.size() elementów więc złożoność:
        //[O(queue.size() * log(size() + queue.size()))].
        //Usuwanie elementów z drugiej kolejki [O(size())].
        void merge(PriorityQueue<K, V>& queue){
            if(this != &queue) {
                PriorityQueue<K, V> tmp(*this);
                typename ByKeys::iterator it = queue.by_keys.begin();
                while(it != queue.by_keys.end()) {
                    tmp.insert(it->get()->first, it->get()->second);
                    ++it;
                }
                tmp.swap(*this);
                queue.by_keys.clear();
                queue.by_values.clear();
            }
        }

        //Zamienia dwie kolejki.
        void swap(PriorityQueue<K, V>& queue) {
            by_keys.swap(queue.by_keys);
            by_values.swap(queue.by_values);
        }

        friend void swap(PriorityQueue<K, V> & q1, PriorityQueue<K, V> & q2) {
            q1.swap(q2);
        }

        bool operator<(PriorityQueue const & queue) const {
            typename ByKeys::iterator it_1, it_2;
            cmp_by_key comparator;
            it_1 = this->by_keys.begin();
            it_2 = queue.by_keys.begin();
            while(it_1 != this->by_keys.end() && it_2 != queue.by_keys.end()) {
                if(it_1->get()->first < it_2->get()->first ||
                   it_2->get()->first < it_1->get()->first ||
                   it_2->get()->second < it_2->get()->second ||
                   it_2->get()->second < it_2->get()->second)
                    return comparator(*it_1, *it_2);
                ++it_1;
                ++it_2;
            }
            return it_1 == this->by_keys.end() && it_2 != queue.by_keys.end();
        }

        bool operator>(PriorityQueue const & queue) const {
            return (queue < *this);
        }

       bool operator==(PriorityQueue const & queue) const {
            return !(*this < queue) && !(*this > queue);
        }

        bool operator!=(PriorityQueue const & queue) const {
            return !(*this == queue);
        }

        bool operator<=(PriorityQueue const & queue) const {
            return !(*this > queue);
        };
        bool operator>=(PriorityQueue const & queue) const {
            return !(*this < queue);
        };
};

#endif
