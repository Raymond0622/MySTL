#include <iostream>
#include <string>

class A {
    public:
        A() {
            std::cout << "A cosntructor" << std::endl;
        }
        A(const A& other) {
            std::cout << "A copy constructor" << std::endl;
        }
        A(A&& other) {
            std::cout << "move cosntructor" << std::endl;
        }
        A& operator=(A&& other) {
            std::cout << "move assignment" << std::endl;
            return *this;
        }
        A& operator=(const A& other) {
            std::cout << "copy assignment" << std::endl;
            return *this;
        }
        ~A() {
            std::cout << "A Destructirr" << std::endl;
        };
};


template <typename... Ts>
class Tuple;

template <>
class Tuple<> {};

template <typename T, typename... Ts>
class Tuple<T, Ts...> : private Tuple<Ts...> {
    public:
        Tuple() = default;
        Tuple(T x, Ts... args) {
            value = x;
            rest = Tuple<std::decay_t<Ts>...>(std::forward<Ts>(args)...);
        }
        Tuple(const Tuple& other) {
            value = other.value;
            rest = other.rest;
        }
        Tuple& operator=(Tuple&& other) {
            if (&other != this) {
                value = std::move(other.value);
                rest = std::move(other.rest);
            }
            return *this;
        }
        Tuple& operator=(const Tuple& other) {
            if (&other != this) {
                value = other.value;
                rest = other.rest;
            }
            return *this;
        }
        ~Tuple() = default;
        T value;
        Tuple<Ts...> rest;
};

template <size_t N, typename... Ts, template <typename...> class Tuple>
constexpr auto get(Tuple<Ts...>& tuple) {
    if constexpr (N == 0) {
        return tuple.value;
    }
    else {
        return get<N - 1>(tuple.rest);
    }
}

template <typename T>
struct tuple_size;

template <typename... Ts>
struct tuple_size<Tuple<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

template <typename T>
constexpr std::size_t tuple_size_v = tuple_size<T>::value;

template <typename... Ts>
Tuple(Ts...) -> Tuple<Ts...>;

int main() {
    A a{};
    Tuple<int, double, A> t{3, 2.0, a};
    Tuple s = std::move(t);
    std::cout << "hi" << std::endl;
    
    std::cout << get<1>(t) << std::endl;
    std::cout << tuple_size_v<decltype(s)> << std::endl;
}