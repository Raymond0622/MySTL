#include <iostream>
#include <string>
class String {
    public:
        String() {
            size_ = 0;
            ptr = nullptr;
        }
        template <size_t N>
        String(const char (&str)[N]) {
            size_ = N - 1;
            if (size_ > 16) {
                std::cout << "heap allocated" << std::endl;
                ptr = reinterpret_cast<char*>(::operator new(size_));
                for (int i = 0; i < size_;i++) {
                    new (ptr + i) char(str[i]);
                }
            }
            else {
                ptr = nullptr;
                for (int i = 0; i < size_;i++) {
                    data[i] = str[i];
                }
                
            }
        }
        ~String() {
            if (size_ > 16) {
                ::operator delete(ptr);
            }
        }
        String(const String& other) {
            if (other.size_ > 16) {
                ptr = reinterpret_cast<char*>(::operator new(other.size_));
                for (int i = 0; i < other.size_;i++) {
                    new (ptr + i) char(*(other.ptr + i));
                }
            }
            else {
                std::copy(std::begin(other.data), std::end(other.data), std::begin(data));
            }
            size_ = other.size_;
        }
        String(String&& other) {
             if (other.size_ > 16) {
                ptr = reinterpret_cast<char*>(::operator new(other.size_));
                for (int i = 0; i < other.size_;i++) {
                    new (ptr + i) char(*(other.ptr + i));
                }
                ::operator delete(other.ptr);
                other.size_ = 0;
            }
            else {
                std::copy(std::begin(other.data), std::end(other.data), std::begin(data));
            }
            size_ = other.size_;
        }
        String& operator=(String&& other) {
            std::cout << "move assignment" << std::endl;
            if (&other != this) {
                if (other.size_ > 16) {
                    ptr = reinterpret_cast<char*>(::operator new(other.size_));
                    for (int i = 0; i < other.size_;i++) {
                        new (ptr + i) char(*(other.ptr + i));
                    }
                    ::operator delete(other.ptr);
                    other.size_ = 0;
                }
                else {
                    std::copy(std::begin(other.data), std::end(other.data), std::begin(data));
                }
                size_ = other.size_;
            }
            return *this;
        }
        String& operator=(const String& other) {
            std::cout << "copy assignment" << std::endl;
            if (&other != this) {
                if (&other != this) {
                if (other.size_ > 16) {
                    ptr = reinterpret_cast<char*>(::operator new(other.size_));
                    for (int i = 0; i < other.size_;i++) {
                        new (ptr + i) char(*(other.ptr + i));
                    }
                }
                else {
                    std::copy(std::begin(other.data), std::end(other.data), std::begin(data));
                }
                size_ = other.size_;
            }
            }
            return *this;
        }
        char& operator[](std::size_t idx) {
            if (idx >= N) {
                return *(ptr + idx);
            }
            else {
                return data[idx];
            }
        }
        std::size_t size() const {
            return size_;
        }
        friend std::ostream& operator<<(std::ostream& os, const String& str) {
            if (str.size() > 16) {
                for (size_t i = 0; i < str.size(); i++)
                    os << *(str.ptr + i);
            } else {
                for (size_t i = 0; i < str.size(); i++)
                    os << str.data[i];
            }
            return os; // must return the stream
        }
    private:
        constexpr static int N = 16;
        char data[N];
        char* ptr;
        std::size_t size_;
    
};


int main() {
    String str = "1231232141241212312312";
    String str1;
    str1 = std::move(str);
    std::cout << str;
}