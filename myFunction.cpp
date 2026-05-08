#include <iostream>

template <typename T>
class Function;

template <typename R, typename... Args>
class Function<R(Args...)> {
    private:
        struct MainCallable {
            virtual R call(Args... args) = 0;
            virtual ~MainCallable() = default;
        };

        template <typename F>
        struct Callable : MainCallable {
            F f;
            Callable(F&& f_) : f(std::forward<F>(f_)) {};

            virtual R call(Args... args) override {
                return f(args...);
            }
        };
    public:
        Function() = default;
        template <typename F>
        Function(F&& f) {
            using U = std::decay_t<F>;
            ptr = new Callable(std::forward<U>(f));
        }
        ~Function() {
            delete ptr;
        }
        R operator()(Args&&... args) {
            return ptr->call(std::forward<Args>(args)...);
        }
        MainCallable* ptr = nullptr;
};

struct A {
    auto operator()(int a, int b) -> int{
        return a + b;
    }
};

int func(int a, int b) {
    return a + b;
}

int main() {
    auto lam = [](int a, int b) -> int{
        return a + b;
    };

    Function<int(int, int)> f(lam);
        
    A a{};
    Function<int(int, int)> ff(a);
    Function<int(int, int)> fff(func);
    std::cout << f(2, 3);
    std::cout << ff(2, 3);
    std::cout << fff(2, 3);

}