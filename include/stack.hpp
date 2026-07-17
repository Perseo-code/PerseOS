#include <stdint.hpp>

template <typename T, int size>
class Stack {
private:
    uint32_t current_index = 0;
    T objects[size];
public:
    void push(const T& data) {
        if (current_index == size) return;
        objects[current_index] = data;
        current_index++;
    }

    T pop() {
        if (current_index == 0) return T();
        T result = objects[current_index-1];
        current_index--;
        return result;
    }

    bool empty() {
        return current_index == 0;
    }
};
