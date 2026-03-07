#include <iostream>


template <typename T>
class Deque {
    private:
        static constexpr std::size_t block_size = 32;
        T** pointers;
        std::size_t size_;
        int start_index;
        int end_index;
        int start_block;
        int end_block;
        std::size_t n_blocks;
        void initialize_blocks() {
            pointers = new T*[8];
            for (int i = 0; i < 8;i++) {
                pointers[i] = nullptr;
            }
            size_t = mid = 4;
            pointers[mid] = reinterpret_cast<T*>(::operator new(sizeof(T)* block_size));
            start_block = mid;
            end_block = mid;
            start_index = 0;
            end_index = 0;
            size_ = 0
        }
        void allocate_back_block() {
            pointers[end_block] = ::operator new(sizeof(T) * block_size);
        }
        void allocate_front_block() {
            pointers[start_block] = ::operator new(sizeof(T) * block_size);
        }
        void destory_all() {
            for (int i = start_block; i < end_block;i++) {
                ::operator delete(pointers[i]);
            }
        }
        void grow_blocks() {
            T** ptr = new T*[2* n_blocks];
            for (int i = 0; i < 2*n_blocks;i++) {
                if (n_blocks /2 <= i && i < 3*n_blocks/2) 
                    ptr[i] = pointers[i - n_blocks / 2];
                else
                    ptr[i] = nullptr;
            }
            delete[] pointers;
            pointers = ptr;
            start_block = n_blocks/2;
            end_block = start_block + n_blocks - 1;
            n_blocks = 2*n_blocks;
        }
    public:
        Deque() : pointers(nullptr) {};

        void push_back(T&& val) {
            if (end_block == n_blocks) {
                grow_blocks();
                new (ptr[end_block][0]) T(std::forward<T>(val));
                end_index = 1;
            }
            else {
                if (end_index == block_size) {
                    allocate_back_block();
                    end_block++;
                    new (ptr[end_block][0]) T(std::forward<T>(val));
                    end_index = 1;
                }
                else {
                    new (ptr[end_block][end_index]) T(std::forward<T>(val));
                    end_index++;
                }
                
            }
            size_++;
        }
        void push_front(T&& val) {
            if (start_block == -1) {
                grow_blocks();
                new (ptr[end_block][block_size - 1]) T(std::forward<T>(val));
                start_index = block_size - 2;
            }
            else {
                if (start_index == -1) {
                    allocate_front_block();
                    start_block--;
                    new (ptr[start_block][block_size - 1]) T(std::forward<T>(val));
                    start_index = block_size - 2;
                }
                else {
                    new (ptr[start_block][start_index]) T(std::forward<T>(val));
                    start_index--;
                } 
            }
            size_++;

        }
        void pop_back() {
            pointers[end_block][end_idex].~T();
            end_index--;
            if (end_index == -1) {
                ::operator delete(map[end_block]);
                end_block--;
                end_index = block_size - 1;
            }

        }
        void pop_front() {
            pointers[end_block][end_idex].~T();
                start_index++;
                if (start_index == block_size) {
                    ::operator delete(map[start_block]);
                    start_block++;
                    start_index = 0;
            }
        }
        void front() {
            return pointers[start_block][start_index];
        }
        void back() {
            return pointers[end_block][end_index];
        }
        T& operator[](size_t index) {
            int block_index = index / block_size;
            int ind = index % block_size;
            return *pointers[block_index][ind];
        }
        size_t size() const {
            return size_;
        }
        bool empty() const {
            return size_ == 0;
        }
};
int main() {
};