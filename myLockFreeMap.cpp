#include <iostream>
#include <atomic>
#include <memory>
#include <thread>


template <typename T>
class LockFreeList {
    public:
        LockFreeList()  {
            head(std::make_unique<Node>());
        }
        void push_front(const T& val) {
            auto ptr = Node(val);
            std::unique_lock<std::mutex> lk(head->m);
            ptr->next = std::move(tmp);
            head->next = std::make_unique<Node>(ptr);
        }
        template <typename F>
        void for_each(F& f) {
            Node* curr = head;
            std::unique_lock<std::mutex> lk(head->m);
            while (curr->next.get()) {
                std::unique_lock<std::mutex> next_lock(curr->next.m);
                const Node* next = curr->next.get();
                *next->data = f(*next->data);
                curr = next;
                lk = std::move(next_lock);
            }
        }
        template <typename F>
        void remove_if(F& f) {
            Node* curr = head;
            std::unique_lock<std::mutex> lk(head->m);
            while (curr->next.get()) {
                std::unique_lock<std::mutex> next_lock(curr->next.m);
                const Node* next = curr->next.get();
                if (f(*next->data)) {
                    curr->next = std::move(next->next);

                }
                else {
                    curr = next;
                    lk = std::move(next_lock);
                }
            }
        }

    private:
        struct Node {
            std::unique_ptr<T> data = nullptr;
            std::unique_ptr<Node> next = nullptr;
            std::mutex m;
            Node(const T& val) {
                data(std::make_unique<T>(val));
            }
        };
        std::unique_ptr<Node> head;
};


template <typename T>
class LockFreeUnorderedMap {
    public:
        

    private:
        

}