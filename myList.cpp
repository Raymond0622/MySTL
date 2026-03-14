#include <iostream>

template <typename T>
class List {
    private:
        struct Node {
            T val;
            Node* next = nullptr;
            Node* prev = nullptr;
        };
        struct Iterator {
            Node* ptr;
            T operator*() {
                return ptr->val;
            }
            Iterator& operator++() {
                std::cout << "meh" << std::endl;
                ptr = ptr->next;
                return *this;
            }
            Iterator& operator--() {
                ptr = ptr->prev;
                return *this;
            }
            Iterator& operator++(int) {
                Iterator temp = *this;
                ++(*this);
                return temp;
            }
            Iterator& operator--(int) {
                Iterator temp = *this;
                --(*this);
                return temp;
            }
        };
        Node* head = new Node();
        Node* tail = new Node();
        std::size_t size_;
    public:
        List() {
            head->next = tail;
            tail->prev = head;
            size_ = 0;
        }
        ~List() {
            while (head) {
                Node* next = head->next;
                delete head;
                head = next;
            }
        }
        void push_back(T&& other) {
            tail->val = T(std::forward<T>(other));
            tail->next = new Node();
            Node* tmp = tail;
            tail = tail->next;
            tail->prev = tmp;
            size_++;
        }
        void pop_back() noexcept {
            if (size_ == 0) {
                return;
            }
            Node* ptr = tail->prev;
            
            if (tail->prev->prev) {
                Node* n = tail->prev->prev;
                n->next = tail;
            }            
            delete ptr;
            size_--;
            
        }
        void push_front(T&& other) {
            Node* ptr = new Node();
            head->val = other;
            head->prev = ptr;
            ptr->next = head;
            head = ptr;
            size_++;
        }
        void pop_front() noexcept {
            if (size_ == 0) {
                return;
            }
            Node* ptr = head->next;
            Node* p = ptr->next;
            delete ptr;
            head->next = p;
            p->prev = head;
            size_--;
        }
        void insert(Iterator& pos, T&& other) {
            Node* ptr = pos.ptr;
            Node* n = new Node();
            Node* prev = ptr->prev;
            
            new (n) T(std::forward<T>(other));
            n->next = ptr;
            n->prev = prev;
            ptr->prev = n;
            prev->next = n;
            size_++;
        }
        Iterator begin() {
            return {head->next};
        }
        Iterator end() {
            return {tail};
        }
        void erase(Iterator& pos) {
            Node* prev = pos.ptr->prev;
            Node* next = pos.ptr->next;
            prev->next = next;
            next->prev = prev;
            delete pos.ptr;
            size_--;
        }
        std::size_t size() {
            return size_;
        }
        Iterator& pre(const Iterator& other) const {
            return {other->ptr->prev};
        }
};


int main() {
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    auto it = list.begin();
    std::cout << *(++it) << std::endl;
    std::cout << *it << std::endl;
    list.insert(it, 10);
    list.erase(it);
    auto it2 = list.begin();
    std::cout << *(++it2) << std::endl;
    std::cout << "size: " << list.size() << std::endl;
    
};