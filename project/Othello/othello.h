#ifndef OTHELLO_H_INCLUDED
#define OTHELLO_H_INCLUDED
#include "othello.cpp"
class playground{
private:
    int8_t board[64];
    int _min_(int number,int number1);
public:
    playground();
    void print_playground();
    static void gotoxy(int x,int y);
    void print_white(int x,int y);
    void print_black(int x,int y);
    void print_dot(int x,int y);
    void able(bool color);

};

class othello{
private:
public:
    othello(){
    playground ob;
    ob.able(b);
    ob.print_playground();
    ob.gotoxy(30,26);
    }
    void getxy();


};
void othello::getxy()
{
    char x , y;
    cout<<"enter x , y:"<<endl;
    cin>>x>>y;
    if( '1' > x || x > '8')
    {
        char temp;
        temp = x;
        x = y;
        y = temp;
    }
    switch(y)
    {
        case 'a':
        case 'A':
            y = 'A';
            break;
        case 'b':
        case 'B':
            y = 'B';
            break;
        case 'c':
        case 'C':
            y = 'C';
            break;
        case 'd':
        case 'D':
            y = 'D';
            break;
        case 'e':
        case 'E':
            y = 'E';
            break;
        case 'f':
        case 'F':
            y = 'F';
            break;
        case 'g':
        case 'G':
            y = 'G';
            break;
        case 'h':
        case 'H':
            y = 'H';
            break;
    }


    cout<<"x = "<<int(x)<<" y = "<<y;
}



#endif // OTHELLO_H_INCLUDED
