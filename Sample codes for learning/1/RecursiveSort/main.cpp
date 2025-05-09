#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

void Swap(vector<int> &arr, int a, int b)
{
    int temp = arr.at(a);
    arr.at(a) = arr.at(b);
    arr.at(b) = temp;
}

void inputArray(vector<int> &arr, int index)
{
    if (index == arr.size())
        return;
    cin >> arr.at(index);
    inputArray(arr, index + 1);
}

int findMinIndex(const vector<int> &arr, int start)
{
    int Size = arr.size();
    if (start == Size - 1)
        return start;
    int minIndex = findMinIndex(arr, start + 1);
    if (arr[start] < arr[minIndex])
    {
        return start;
    }
    return minIndex;
}

void selectionSort(vector<int> &arr, int start)
{
    int min_index = findMinIndex(arr, start);
    Swap(arr, start, min_index);
    cout << arr[start] << endl;
    if (start == arr.size() - 1)
        return;
    selectionSort(arr, start + 1);
}

int main()
{
    int Size{0};
    cin >> Size;
    vector<int> arr(Size);

    inputArray(arr, 0);
    selectionSort(arr, 0);

    return 0;
}