#include <iostream>

template <typename T>
class myUniquePtr {
    public:
        myUniquePtr() = default;
        explicit myUniquePtr(T* other) {
            ptr = other;
            std::cout << "Raw pointer constructor called\n";
        }
        myUniquePtr(const myUniquePtr& other) = delete;
        myUniquePtr& operator=(const myUniquePtr& other) = delete;
        myUniquePtr& operator=(myUniquePtr&& other) {
            if (this != &other) {
                delete ptr;
                ptr = other.ptr;
                other.ptr = nullptr;
            }
            return *this;
        };
        myUniquePtr(myUniquePtr&& other) {
            ptr = other.ptr;
            other.ptr = nullptr;
        };
        ~myUniquePtr() {
            delete ptr;
        };
        T& operator*() const {
            return *ptr;
        }
        T* operator->() const {
            return ptr;
        }
        T* release() {
            T* other= ptr;
            ptr = nullptr;
            return other;
        }
        T* get() {
            return ptr;
        }
    private:
        T* ptr = nullptr;
};

template <typename T, typename... Ts>
myUniquePtr<T> make_unique(Ts&&... objs) {
    return myUniquePtr<T>(new T(std::forward<Ts>(objs)...));
}

class A {
    public:
        void print() {
            std::cout << "printing A!" << std::endl;
        }
};

int main() {
    int* s = new int(10);
    myUniquePtr<int> ptr{s};
    myUniquePtr<int> ptr2 = std::move(ptr);
    ptr2 = std::move(ptr2);
    myUniquePtr<A> ptrA = make_unique<A>();
    ptrA->print();
}