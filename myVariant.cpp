#include <iostream>
#include <stdexcept>
#include <variant>

template <typename... Ts>
class Variant;

template <typename... Ts>
constexpr std::size_t max_sizeof() {
    return std::max({sizeof(Ts)...}); 
}

template <typename T, typename... Ts>
struct check : std::bool_constant<(std::is_same_v<T, Ts> || ...)> {};

template <size_t N, typename T>
struct variant_alternative;

template <size_t N, typename T, typename... Ts> 
struct variant_alternative<N, Variant<T, Ts...>> {
    using type = variant_alternative<N - 1, Variant<Ts...>>::type;
};

template <typename T, typename... Ts> 
struct variant_alternative<0, Variant<T, Ts...>> {
    using type = T;
};

template <size_t N, typename... Ts>
struct which_index;

template <size_t N, typename U, typename T, typename... Ts>
struct which_index<N, U, T, Ts...> {
    constexpr static size_t value = 
        std::is_same_v<U, T> ? N : which_index<N + 1, U, Ts...>::value;
};
template <size_t N, typename U>
struct which_index<N, U> {
    constexpr static size_t value = -1;
};

template <typename... Ts>
class Variant {
    public:
        Variant() {
            id = 0;
        }
        template <typename T>
        Variant(T x) {
            static_assert(check<T, Ts...>::value);
            id = which_index<0, T, Ts...>::value;
            new (&data) T(std::move(x));
        }
        Variant(const Variant& other) {
            memcpy(data, other.data, sizeof(other.data));
        }
        size_t index() {
            return id;
        }
        char data[max_sizeof<Ts...>()];
        size_t id;
};

template <size_t N, typename... Ts>
auto& get(Variant<Ts...>& other) {
    try {
        if (N != other.index()) {
            throw std::bad_variant_access{};
        }
        else {
            return *reinterpret_cast<typename variant_alternative<N, Variant<Ts...>>::type*>(&other.data);
        }
    }
    catch (std::bad_variant_access& e) {
        std::cout << e.what() << std::endl;
    }
}

class A {
    public:
        double x;
        int y;
};

int main(int argc, char** argv) {
    A b = {1, 2};
    Variant<int, double, A> a = b;
    std::cout << get<2>(a).x;

}