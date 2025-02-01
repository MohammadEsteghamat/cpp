#include <iostream>
using namespace std;

class DynamicArray {
private:
    int* arr;
    size_t size;

public:
    DynamicArray(size_t s) : size(s) {
        arr = new int[size];
    }

    DynamicArray(DynamicArray& other){
        size = other.size;
        arr = new int[size];
        for (int i = 0; i < size; ++i) {
            arr[i] = other.arr[i];
        }
    }

    ~DynamicArray() {
        delete[] arr;
    }

    size_t getsize() { return size; }

    void sortArray() {
        int temp;
        for (size_t i = 0; i < size - 1; ++i) {
            for (size_t j = 0; j < size - 1 - i; ++j) {
                if (arr[j] > arr[j + 1]) {
                    temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }

    void printArray() const {
        for (size_t i = 0; i < size; ++i) {
            std::cout << arr[i] << " ";
        }
        cout << endl;
    }

    int& operator[](size_t index) {
        return arr[index];
    }
};

int main() {
    DynamicArray ob(10);
    for (int i{0}; i < ob.getsize(); ++i) {
        ob[i] = 10 - i;
    }
    ob.printArray();

    ob.sortArray();
    ob.printArray();

    return 0;
}