#include <iostream>
#include <vector>
#include <list>

template <typename Key, typename Value, typename Hash, typename Compare>
class UnorderedMap {
    private:
        template <typename Value>
        struct Node {
            Node(Key key) : key_(key) {
                val_ = Value();
            }
            Key key_;
            Value val_;
        }
        void rehash(std::size_t new_bucket_number) {
            std::vector<std::list<Node>> new_buckets(new_bucket_number);
            for (auto& buck : buckets) {
                for (auto& pair : buck) {
                    std::size_t idx = hash(pair.first) % new_bucket_number;
                    new_buckets[idx].push_back(pair);
                }
            }
            buckets = std::move(new_buckets);
        }
        Hash hash;
        Compare comp;
        std::size_t buckets_;
        std::vector<std::list<Node<Value>> buckets;
    public:
        UnorderedMap(std::size_t bucks, Hash hash) : buckets_(bucks), hash_(hash) {
            buckets.resize(bucks);
        };
        Value& operator[](Key key) {
            auto idx = hash(key) % buckets_
            for (auto& node : buckets[idx]) {
                if (is_equal(key, node.key_)) {
                    return node.second;
                }
            }
            buckets[idx].emplace_back();
            num_elements++;
            check_load_factor();
            
        }
        void erase(Key key) {
            std::size_t idx = hash(key) % buckets_;
            for (auto it : buckets[idx].begin()) {
                if (is_equal(it->key_), key) {
                    buckets[idx].erase(it);
                    num_elements--;
                }
            }
        }
        T* find(const Key& key) {
            std::size_t idx = hash(key) % buckets_;
            for (auto pair : buckets[idx]) {
                if (is_equal(pair.key_, key)) {
                    return &pair.value_;
                }
            }
            return nullptr;
        }
};


int main() {

}