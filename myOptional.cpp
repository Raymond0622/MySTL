#include <iostream>
#include <vector>

template <typename T>
class Optional {
    public:
        Optional() = default;
        Optional(std::nullopt_t) {
            is_val = false;
        }
        template <typename... Args>
        Optional(Args&&... args) {
            is_val = true;
            new (&storage_) T(std::forward<Args>(args)...);
        }
        Optional(T&& value) {
            is_val = true;
            new (&storage_) T(std::move(value));
        }
        Optional(const T& value) {
            is_val = true;
            new (&storage_) T(value);
        }
        Optional(const Optional& other) {
            is_val = other.is_val;
            if (is_val) {
                new (&storage_) T(*reinterpret_cast<const T*>(&other.storage_));
            }
        }
        Optional(Optional&& other) {
            is_val = other.is_val;
            other.is_val = false;
            if (is_val) {
                new (&storage_) T(std::move(*reinterpret_cast<T*>(&other.storage_)));
            }
        }
        Optional& operator=(std::nullopt_t) {
            if (is_val) {
                std::cout << "Null assignemnt" << std::endl;
                reset();
            }
            
            return *this;
        }
        Optional& operator=(const Optional& other) {
            if (this != &other) {
                is_val = other.is_val;
                if (is_val) {
                    new (&storage_) T(*reinterpret_cast<const T*>(&other.storage_));
                }
            }
        }
        Optional& operator=(Optional&& other) {
            if (this != &other) {
                is_val = other.is_val;
                if (is_val) {
                    new (&storage_) T(std::move(*reinterpret_cast<T*>(&other.storage_)));
                }
            }
        }
        ~Optional() {
            reset();
        }
        T& operator*() {
            return *reinterpret_cast<T*>(&storage_);   
        }
        T& value() {
            if(is_val) {
                return *reinterpret_cast<T*>(&storage_);
            }
            throw std::bad_optional_access();
        }
        bool has_value() {
            return is_val;
        }
        template <typename... Args>
        void emplace(Args&&... args) {
            new (&storage_) T(std::forward<Args>(args)...);
            is_val = true;
        }
        void reset() {
            if (is_val) {
                reinterpret_cast<T*>(&storage_)->~T();
            }
            is_val = false;
            
        }
    private:
        typename std::aligned_storage<sizeof(T), alignof(T)>::type storage_;
        bool is_val = false; 
};

template <typename T, typename... Args>
Optional<T> make_optional(Args&&... args) {
    return Optional<T>(std::forward<Args>(args)...);
}

struct A {
    A(int x, std::string c) : x_(x), c_(c) {};
    A() = default;
    int x_;
    std::string c_;
};

int main() {
    Optional<A> a = make_optional<A>(1, "avc");
    a = std::nullopt;
    auto p= *a;
    auto q = a.value();

}