// implementation basic hashmap (unordered container)
// Mikhail Nesterenko: adapted from Proc C++
// 4/15/2014

#include <cstddef>
#include <utility>
#include <functional>
#include <vector>
#include <list>

using std::vector;
using std::list;
using std::pair;
using std::make_pair;

//////////////////////////////////////////
// hash function implemented as a class
//////////////////////////////////////////

// any Hash Class must provide 
// two methods: hash() and numBuckets().
template <typename T>
class DefaultHash {
public:
    DefaultHash(size_t numBuckets = defaultNumBuckets);
    size_t hash(const T& key) const;
    size_t numBuckets() const { return numBuckets_; }
private:
    static const size_t defaultNumBuckets = 101; // default number of buckets in the hash
    size_t numBuckets_;
};

template <typename T>
DefaultHash<T>::DefaultHash(size_t numBuckets) : numBuckets_(numBuckets) {}


// uses the division method for hashing.
// treats the key as a sequence of bytes, sums the ASCII
// values of the bytes, and mods the total by the number
// of buckets.
template <typename T>
size_t DefaultHash<T>::hash(const T& key) const {
    size_t res = 0;
    for (size_t i = 0; i < sizeof(key); ++i) {
        const unsigned char b = *(reinterpret_cast<const unsigned char *>(&key) + i);
        res += b;
    }
    return res % numBuckets_;
}


////////////////////////////////////////////////
// container class
////////////////////////////////////////////////

template <typename Key, typename Value,
    typename Compare = std::equal_to<Key>,
    typename Hash = DefaultHash<Key>>
    class hashmap {

    public:
        typedef pair<const Key, Value> Element;

        // constructor
        // invokes constructors for comparison and hash objects
        hashmap(const Compare& comp = Compare(),
            const Hash& hash = Hash());

        Element* find(const Key& x);      // returns pointer to element with key x,
                                          // nullptr if not found
        pair<Value*, bool> insert(const Element& x);    // inserts the key/value pair 
        pair<Element*, bool> erase(const Key& x);         // erases element with key x, if exists
        Value& operator[] (const Key& x); // returns reference on value of
                                          // element with key, inserts if does not exist
        void rehash(size_t num);
    private:

        // helper function for various searches
        typename list<Element>::iterator
            findElement(const Key& x,
                const size_t bucket);

        size_t size_;   // number of elements in the container
        Compare comp_;  // comparison functor, equal_to by default
        Hash hash_;     // hash functor 

        // hash contents: vector of buckets
        // each bucket is a list containing key->value pairs
        vector<list<Element>> elems_;
};


////////////////////////////////////////////////
// container member functions
////////////////////////////////////////////////

// Construct elems_ with the number of buckets.
template <typename Key, typename Value, typename Compare, typename Hash>
hashmap<Key, Value, Compare, Hash>::hashmap(
    const Compare& comp, const Hash& hash) :
    size_(0), comp_(comp), hash_(hash) {
    elems_ = vector<list<Element>>(hash_.numBuckets());
}


// helper function
template <typename Key, typename Value, typename Compare, typename Hash>
typename list<pair<const Key, Value>>::iterator // return type
hashmap<Key, Value, Compare, Hash>::findElement(const Key& x, size_t bucket) {

    // look for the key in the bucket
    for (auto it = elems_[bucket].begin();
        it != elems_[bucket].end(); ++it)
        if (comp_(it->first, x))
            return it;

    return elems_[bucket].end(); // element not found
}


// returns a pointer to the element with key x
// returns nullptr if no element with this key
template <typename Key, typename Value, typename Compare, typename Hash>
typename hashmap<Key, Value, Compare, Hash>::Element* // return value type
hashmap<Key, Value, Compare, Hash>::find(const Key& x) {

    size_t bucket = hash_.hash(x);
    auto it = findElement(x, bucket);    // use the findElement() helper   

    if (it != elems_[bucket].end())
        // found the element. Return a pointer to it.
        return &(*it); // dereference the iterator to list 
                       // then take the address of list element

    else // didn't find the element -- return nullptr
        return nullptr;
}


// finds the element with key x, inserts an
// element with that key if none exists yet. Returns a reference to
// the value corresponding to that key.
template <typename Key, typename Value, typename Compare, typename Hash>
pair <Value*, bool> hashmap<Key, Value, Compare, Hash>::insert(const Element& x) {

    size_t bucket = hash_.hash(x.first);
    auto it = findElement(x.first, bucket);    // try to find the element

    // if not found, insert a new one.
    if (it == elems_[bucket].end()) {
        ++size_;
        elems_[bucket].push_back(x);
        --it;
        return make_pair(&(it->second), true);
    }
    else {
        return make_pair(&(it->second), false);
    }
}


// removes the Element with key x, if it exists
template <typename Key, typename Value, typename Compare, typename Hash>
pair<typename hashmap<Key, Value, Compare, Hash>::Element*, bool>
hashmap<Key, Value, Compare, Hash>::erase(const Key& x) {

    size_t bucket = hash_.hash(x);
    auto it = findElement(x, bucket);    // try to find the element

    if (it != elems_[bucket].end()) {    // the element exists, erase it
        auto next_element = elems_[bucket].erase(it); //next_element is an iterator to the element after the one being erased
        --size_;
        if (next_element == elems_[bucket].end()) {   //no more elements in the bucket
            for (size_t i = bucket; i < elems_[bucket].size(); ++i) { //Loop through the buckets
                if (elems_[i].size() > 0) {   //if there is something in the bucket
                    next_element = elems_[i].begin();
                    return make_pair(&(*next_element), true);
                }
            }
            //reached the end of the hash without finding another element
            return make_pair(nullptr, true);
        }
        else {    //the bucket still has an element in it after the removal
            return make_pair(&(*next_element), true);
        }

    }
    //the element did not exist
    return make_pair(nullptr, false);
}


// returns reference to value of element with key x,
// inserts if does not exist
template <typename Key, typename Value, typename Compare, typename Hash>
Value& hashmap<Key, Value, Compare, Hash>::operator[] (const Key& x) {
    //insert returns a reference to a <Value*, bool> pair
    auto it = insert(make_pair(x, Value()));    //iterator to the returned reference
    return *it.first;
}

template <typename Key, typename Value, typename Compare, typename Hash>
void hashmap<Key, Value, Compare, Hash>::rehash(size_t num) {
    if (num < hash_.numBuckets()) {
        return;
    }
    hashmap<Key, Value, Compare, Hash> new_hash_map;
    Hash new_hash(num);
    new_hash_map.hash_ = new_hash;
    new_hash_map.elems_.resize(num);
    for (size_t bucket = 0; bucket < elems_.size(); ++bucket) { //Loop through original hash map 
        for (auto it = elems_[bucket].begin(); it != elems_[bucket].end(); ++it) {  //If there are elements in the bucket
            new_hash_map.insert(*it);
        }
    }
    std::swap(*this, new_hash_map); 
}