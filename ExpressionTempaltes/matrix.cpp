#include <iostream>
#include <type_traits>


template <size_t N, size_t M, typename T>
struct Matrix {
    constexpr static size_t n = N;
    constexpr static size_t m = M;
    using Type = T;
    T* ptr;
    Matrix() : ptr(new T[N*M]) {};
    template <typename U>
    Matrix(const U& u) : ptr(new T[n*m]) {
        for (size_t i = 0;i < n;i++) {
            for (size_t j = 0; j < m;j++) {
                ptr[i * m + j] = u(i, j);
            }
        }
    }
    ~Matrix()  {
        delete[] ptr;
    }
    const Type& operator()(size_t i, size_t j) const {
        return ptr[i * m + j];
    }
    Type& operator()(size_t i, size_t j) {
    return ptr[i * m + j];
}
};

template <typename L, typename R>
struct Multiply_Matrix_Matrix {
    const L& lhs;
    const R& rhs;
    using Type = L::Type;
    constexpr static size_t n = L::n;
    constexpr static size_t m = R::m;
    Multiply_Matrix_Matrix(const L& lhs, const R& rhs) : lhs(lhs), rhs(rhs) {
        static_assert(L::m == R::n);
    };

    Type operator()(std::size_t i, std::size_t j) {
        Type ans{0};
        for (size_t ii = 0; ii < L::m;ii++) {
            ans += lhs(i, ii) * rhs(ii, j);
        }
        return ans;
    }
    Type operator()(std::size_t i, std::size_t j) const {
        Type ans{0};
        for (size_t ii = 0; ii < L::m;ii++) {
            ans += lhs(i, ii) * rhs(ii, j);
        }
        return ans;
    }
};

template <typename L, typename R>
struct Add_Matrix_Matrix {
    const L& lhs;
    const R& rhs;
    using Type = L::Type;
    constexpr static size_t n = L::n;
    constexpr static size_t m = R::m;
    Add_Matrix_Matrix(const L& lhs, const R& rhs) : lhs(lhs), rhs(rhs) {
        static_assert(L::n == R::n && L::m == R::m);
    };
    Type operator()(std::size_t i, std::size_t j) {
        return lhs(i, j) + rhs(i, j);
    }
    Type operator()(std::size_t i, std::size_t j) const {
        return lhs(i, j) + rhs(i, j);
    }
};

template <typename L, typename R>
struct Multiply_Scalar_Matrix {
    const L& lhs;
    const R& rhs;
    using Type = R::Type;
    constexpr static size_t n = R::n;
    constexpr static size_t m = R::m;
    Multiply_Scalar_Matrix(const L& lhs, const R& rhs) : lhs(lhs), rhs(rhs) {};
    Type operator()(std::size_t i, std::size_t j) {
        return lhs * rhs(i, j);
    }
    Type operator()(std::size_t i, std::size_t j) const {
        return lhs * rhs(i, j);
    }
};

template <typename T, typename R, typename = std::enable_if_t<std::is_integral_v<T>>>
Multiply_Scalar_Matrix<T, R> operator*(T lhs, const R& rhs) {
    return Multiply_Scalar_Matrix<T, R>(lhs, rhs);
}

template <typename L, typename R, typename = std::enable_if_t<!std::is_integral_v<L>>>
Multiply_Matrix_Matrix<L, R> operator*(const L& lhs, const R& rhs) {
    return Multiply_Matrix_Matrix<L, R>(lhs, rhs);
}

template <typename L, typename R>
Add_Matrix_Matrix<L, R> operator+(const L& lhs, const R& rhs) {
    return Add_Matrix_Matrix<L, R>(lhs, rhs);

}

int main() {
    Matrix<10, 10, double> matrix1, matrix2;
    for (size_t i = 0; i < 10;i++) {
        matrix1(i, i) = 2;
    }
    for (int i = 0; i < 10;i++) {
        matrix2(i, i) = 2;
    }
    Matrix<10, 10, double> matrix3 = 2 * matrix1 + matrix2 * matrix2;

    for (size_t i = 0; i < 10;i++) {
        for (int j = 0; j < 10;j++) {
            std::cout << matrix3(i, j) << " ";
        }
        std::cout << "\n";
    }
}