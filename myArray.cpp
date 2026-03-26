#include <algorithm>
#include <stdexcept>
#include <cstddef>
#include <iostream>

template <typename T, std::size_t N>
class Array {
public:
    template <typename... Args, typename = std::enable_if_t<sizeof...(Args) == N>>
    Array(Args... args) : data{args...} {}

    Array() = default;

    Array(const Array& other) {
        std::copy(std::begin(other.data), std::end(other.data), std::begin(data));
    }

    Array(Array&& other) noexcept {
        for (std::size_t i = 0; i < N; ++i)
            data[i] = std::move(other.data[i]);
    }

    Array& operator=(const Array& other) {
        if (this != &other)
            std::copy(std::begin(other.data), std::end(other.data), std::begin(data));
        return *this;
    }

    Array& operator=(Array&& other) noexcept {
        if (this != &other)
            for (std::size_t i = 0; i < N; ++i)
                data[i] = std::move(other.data[i]);
        return *this;
    }

    T& operator[](std::size_t idx) { return data[idx]; }
    const T& operator[](std::size_t idx) const { return data[idx]; }

    T& at(std::size_t idx) {
        if (idx >= N) 
            throw std::out_of_range("Index out of range");
        return data[idx];
    }
    const T& at(std::size_t idx) const {
        if (idx >= N) 
            throw std::out_of_range("Index out of range");
        return data[idx];
    }

    constexpr std::size_t size() const { return N; }
    constexpr bool empty() const { return N == 0; }

private:
    T data[N];
};

template <typename T, typename... U>
Array(T, U...) -> Array<T, 1 + sizeof...(U)>;

int main() {
    Array t = {1, 2, 3, 4};
    std::cout << t.at(4);
}