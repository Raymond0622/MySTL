#include <iostream>

template <typename T>
class SharedPtr {
    private:
        T* ptr = nullptr;
        struct ControlBlock {
            std::size_t ref_count;
            ControlBlock(size_t count) : ref_count(count) {};
        };
        ControlBlock* control;
        void release() {
            if (!control) {
                return;
            }
            control->ref_count--;
            if (control->ref_count == 0) {
                delete ptr;
                delete control;
            }
            ptr = nullptr;
            control = nullptr;
        }
    public:
        SharedPtr() {
            control = nullptr;
        };
        explicit SharedPtr(T* other) {
            ptr = other;
            if (other) {
                control = new ControlBlock(1);
            }
            else {
                control = nullptr;
            }
        };

        SharedPtr(const SharedPtr& other) {
            other.control->ref_count++;
            ptr = other.ptr;
            control = other.control;

        };
        SharedPtr(SharedPtr&& other) {
            control = other.control;
            ptr = other.ptr;
            other.control = nullptr;
            ptr = nullptr;
        };
        SharedPtr& operator=(const SharedPtr& other) {
            if (this != &other) {
                release();
                other.control->ref_count++;
                control = other.control;
                ptr = other.ptr;
            }
            return *this;
        };
        SharedPtr& operator=(SharedPtr&& other) {
            if (this != &other) {
                release();
                control = other.control;
                ptr = other.ptr;
                other.control = nullptr;
                other.control = nullptr;
            }
            return *this;
        };
        ~SharedPtr() {
            release();
        };
        void reset() {
            release();
        };
        void reset(T* other) {
            relase();
            if (other) {
                ptr = other;
                control = new ControlBlock(1);
            }
        };
        bool unique() {
            return control->ref_count == 1;
        }
        std::size_t use_count() const {
            return control ? control->ref_count : 0;
        }

        T* operator->() const {
            return ptr;
        };
        T& operator*() const {
            return *ptr;
        };

};

int main() {

    SharedPtr<int> ptr(new int(10));

}