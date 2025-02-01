#include <iostream>
using namespace std;
struct Element
{
    int data;
    Element *next;
    Element(int value) : data(value), next(nullptr) {}
};

int main()
{
    int n, value;
    Element *head(nullptr), *current(nullptr);
    cin >> n;
    if (n > 0)
    {
        for (int i = 0; i < n; i++)
        {
            cin >> value;
            Element *newnode = new Element(value);
            if (!head)
            {
                head = newnode;
            }
            else
            {
                current->next = newnode;
            }
            current = newnode;
        }

        current = head;
        while (current != nullptr)
        {
            cout << current->data << endl;
            current = current->next;
        }
        //free memory
        while (head != nullptr)
        {
            Element *temp  = head;
            head = head->next;
            delete temp;
        }

    }

    return 0;
}