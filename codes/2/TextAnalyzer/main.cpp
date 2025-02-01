#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    string input, word = "";
    vector<string> words;
    int counter{0};
    getline(cin, input);
    for (int i = 0; i < input.size(); i++)
    {
        char a = input[i];
        if (a == ':' || a == ',' || a == '.')
        {
            input[i] = ' ';
        }
        input[i] = toupper(input[i]);
    }

    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == ' ')
        {
            if (word == "")
            {
                continue;
            }

            words.push_back(word);
            word = "";
            continue;
        }
        word += input[i];
    }
    if (word != "")
    {
        words.push_back(word);
    }

    for (int i = 0; i < words.size(); i++)
    {
        bool flg{true};
        for (int j = i-1; j >= 0; j--)
        {
            if (words.at(i) == words.at(j))
            {
                flg = false;
            }
        }
        if (flg)
        {
            counter++;
        }

    }
    cout<<counter;

    return 0;
}