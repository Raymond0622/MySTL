#include <iostream>
#include <atomic>
#include <memory>
#include <thread>
#include <vector>
#include <mutex>


template <typename Key, typename Value>
class LockFreeList {
    public:
        LockFreeList()  {
            head = std::make_unique<Node>();
            size_ = 0;
        }
        Value& push_front(const Key& key, const Value& val = Value{}) {
            auto ptr = std::make_unique<Node>(key, val);
            std::unique_lock<std::mutex> lk(head->m);
            ptr->next = std::move(head->next);
            head->next = std::move(ptr);
            size_++;
            return *(head->next->data);
        }
        template <typename F>
        void for_each(F& f) {
            Node* curr = head.get();
            std::unique_lock<std::mutex> lk(head->m);
            while (curr->next.get()) {
                std::unique_lock<std::mutex> next_lock(curr->next->m);
                const Node* next = curr->next.get();
                *next->data = f(*next->data);
                curr = next;
                lk = std::move(next_lock);
            }
        }
        void remove_if(const Key& key, std::size_t& count) {
            Node* curr = head.get();
            std::unique_lock<std::mutex> lk(head->m);
            while (curr->next.get()) {
                std::unique_lock<std::mutex> next_lock(curr->next->m);
                const Node* next = curr->next.get();
                if (next->key == key) {
                    curr->next = std::move(next->next);
                    count--;
                    return;
                }
                else {
                    curr = next;
                    lk = std::move(next_lock);
                }
            }
        }
        Value& find_first(const Key& key, std::size_t& count) {
            const Node* curr = head.get();
            std::unique_lock<std::mutex> lk(head->m);
            while (Node* next = curr->next.get()) {
                std::unique_lock<std::mutex> next_lock(next->m);
                if (next->key == key) {
                    return *next->data;
                }
                else {
                    curr = next;
                    lk = std::move(next_lock);
                }
            }
            lk.unlock();
            Value& val = push_front(key);
            count++;
            size_++;
            return val;
        }
        Value& find_first(const Key& key) {
            Node* curr = head.get();
            std::unique_lock<std::mutex> lk(head->m);
            while (curr->next.get()) {
                std::unique_lock<std::mutex> next_lock(curr->next->m);
                const Node* next = curr->next.get();
                if (next->key == key) {
                    return *next->data;
                }
            }
            throw std::out_of_range{};
        }
        void find_first(const Key& key, std::size_t& count, const Value& val) {
            const Node* curr = head.get();
            std::unique_lock<std::mutex> lk(head->m);
            while (curr->next) {
                std::unique_lock<std::mutex> next_lock(curr->next->m);
                const Node* next = curr->next.get();
                if (next->key == key) {
                    next->data = std::make_unique<Node>(val);
                    return;
                }
                else {
                    curr = next;
                    lk = std::move(next_lock);
                }
            }
            push_front(key, val);
            count++;
            size_++;
        }
        // void operator=(const Value& val) {
        //     replace(key, n_elements, val);
        //     return;
        // }
        std::size_t size() {
            return size_;
        }

    private:
        struct Node {
            Key key;
            std::unique_ptr<Value> data = nullptr;
            std::unique_ptr<Node> next = nullptr;
            std::mutex m;
            Node() = default;
            Node(const Key& key_, const Value& val_) {
                key = key_; 
                data = std::make_unique<Value>(val_);
            }
        };
        std::unique_ptr<Node> head;
        std::size_t size_;
};


template <typename Key, typename Value, typename Hash = std::hash<Key>>
class LockFreeUnorderedMap {
    public:
        LockFreeUnorderedMap(int buckets_){
            hasher = Hash{};
            buckets = buckets_;
            map.resize(buckets);
        }
        LockFreeUnorderedMap() {
            std::cout << "hi";
            hasher = Hash{};
            buckets = 17;
            map.resize(buckets);
        }
        Value& operator[](const Key& key) noexcept {
            int b = hasher(key) % buckets;
            int x = 1;
            return map[b].find_first(key, n_elements);
        }
        Value& at(const Key& key) const {
            int b = hasher(key) % buckets;
            std::cout << 1;
            return map[b].find_first(key, n_elements);
        }
        void insert(const Key& key, const Value& val) {
            map[hasher(key) % buckets].push_front(key, val);
            n_elements++;
        }
        std::size_t count(const Key& key) {
            int b = hasher(key) % buckets;
            return map[b].size() > 0;
        }
        std::size_t size() const {
            return n_elements;
        } 
    private:
        std::size_t n_elements = 0;
        std::size_t buckets = 17;
        std::vector<LockFreeList<Key, Value>> map;
        Hash hasher;
};

int main() {
    LockFreeUnorderedMap<int, int> map;
    std::cout << map.count(1);
    std::cout << map[1];
    map[1] = 1;
    std::cout << map[1];
    std::cout << map.size();
}