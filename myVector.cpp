#include <iostream>



template <typename T>
class MyVector {
    public:
        using value_type = T;
        MyVector() {
            std::cout << "Default constructor called" << std::endl;
        }
        MyVector(std::size_t count) {
            ptr = reinterpret_cast<T*>(::operator new(sizeof(T)* count));
            for (int i = 0;i < count;i++) {
                new (ptr + i) T();
            }
            size_ = count;
            capacity_ = count;
            std::cout << "Count constructor called" << std::endl;
        };
        template <typename U>
        MyVector(std::size_t count, U&& value) {
            ptr = reinterpret_cast<T*>(::operator new(sizeof(T)* count));
            for (int i = 0;i < count;i++) {
                new (ptr + i) T(std::forward<U>(value));
            }
            size_ = count;
            capacity_ = count;
            std::cout << "Count/Value constructor called" << std::endl;
        };
        MyVector(std::initializer_list<T> other) {
            ptr = reinterpret_cast<T*>(::operator new(sizeof(T) * other.size()));
            size_t i = 0;
            for (const T& val : other) {
                new (ptr + i) T(val);
                i++;
            }
            size_ = other.size();
            capacity_ = size_;
            std::cout << "Initializer list called" << std::endl;
        };
        MyVector(const MyVector& other) {
            ptr = reinterpret_cast<T*>(::operator new(sizeof(T) * other.capacity()));
            for (int i = 0;i < other.size();i++) {
                new (ptr + i) T(*(other.ptr + i));
            }
            size_ = other.size_;
            capacity_ = other.capacity_;
            std::cout << "Copy constructor called" << std::endl;
        };
        MyVector(MyVector&& other) : ptr(other.ptr), size_(other.size_), capacity_(other.capacity_) {
            other.ptr = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
            std::cout << "Move constructor called" << std::endl;
        }
        MyVector& operator=(const MyVector& other) {
            if (this == &other) {
                return *this;
            }
            clear();
            ptr = reinterpret_cast<T*>(::operator new(sizeof(T) * other.capacity()));
            for (int i = 0;i < other.size();i++) {
                new (ptr + i) T(*(other.ptr + i));
            }
            size_ = other.size_;
            capacity_ = other.capacity_;
            std::cout << "Copy assignment called" << std::endl;
            return *this;
        };
        MyVector& operator=(MyVector&& other) {
            ptr = other.ptr;
            size_ = other.size_;
            capacity_ = other.capacity_; 
            other.ptr = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
            std::cout << "Move assignment called" << std::endl;
            return *this;
        };
        std::size_t size() {
            return size_;
        };
        std::size_t capacity() {
            return capacity_;
        };
        void reserve(std::size_t count) {
            if (capacity_ < count) {
                T* new_ptr = ::operator new(sizeof(T) * count);
                copy(ptr, new_ptr, size_);
            }
            capacity_ = count;
        };
        T& back() {
           return *(ptr + size_ - 1);
        }
        void push_back(T&& value) {
            if (size_ == capacity_) {
                // usually grows double of size;
                T* new_ptr = reinterpret_cast<T*>(::operator new(sizeof(T)* (2 * size_)));
                copy(ptr, new_ptr, size_);
                new (ptr + size_) T(std::forward<T>(value));
            }
            size_ += 1;
            capacity_ = 2* capacity_;
        }
        void clear() {
            for (int i = 0; i < size_;i++) {
                (ptr + i)->~T();
            }
            ::operator delete(ptr);
        };
        ~MyVector() {
            clear();
        }
    private:
        // helper function to copy old pointer to new pointer location
        // count is # of elements to copy, not bytes
        // but original data is destroyed, this is for reserve features.
        void copy(T*& old, T*& new_ptr, std::size_t count) {
            for (int i = 0; i < count;i++) {
                new (new_ptr + i) T(*(old + i));
                (old + i)->~T();
            }
            ::operator delete(old);
            old = new_ptr;
        }
        T* ptr = nullptr;
        std::size_t capacity_ = 0;
        std::size_t size_ = 0;
};

class A {
    public:
        A() {
            std::cout << "A constructor called" << std::endl;
        }
        A(const A& other) {
            std::cout << "A Copy constructor called" << std::endl;
        }
        A(A&& other) {
            std::cout << "A Move constructor called" << std::endl;
        }
        A& operator=(A&& other) {
            std::cout << "A Move assignment called" << std::endl;
            return *this;
        }
        A& operator=(const A& other) {
            std::cout << "A Copy assignment called" << std::endl;
            return *this;
        }
        ~A() {
            std::cout << "A destructor called" << std::endl;
        }
};

int main() {

    MyVector<int> vec{1, 2, 3};
    vec.push_back(3);
    std::cout << vec.size() << std::endl;
    std::cout << vec.capacity() << std::endl;
    std::cout << vec.back() << std::endl;

    A a;
    MyVector<A> avec(5, A());
    avec.push_back(A());
}