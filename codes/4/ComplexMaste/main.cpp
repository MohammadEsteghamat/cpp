#include <iostream>
#include <cmath>
using namespace std;
class complex{
private:
    double x,y,a,t;
public:
    complex(double a=0,double b=0) : x(a),y(b) {}
    void print(){
        if(x==0 && y==0) cout<<0;
        else if(x==0)cout<<y<<"*i";
        else if(y==0) cout<< x;
        else cout<< x << ' '<<(y>0?'+':'-')<<' ' <<abs(y)<<"*i";

    }
    void fizr(){
        a = sqrt(pow(x,2)+ pow(y,2));
        t = atan2(y,x);
    }
    void com(){
        x = a * cos(t);
        y = a * sin(t);
        if(abs(x) <= 4.89843e-16) x =0;
        if(abs(y) <= 4.89843e-16) y =0;
    }
    complex operator+(complex n){
        x+=n.x;
        y+=n.y;
        return *this;
    }
    complex operator-(complex n){
        x-=n.x;
        y-=n.y;
        return *this;
    }
    complex operator*(complex n){
        double v{x};
        x = x * n.x - y * n.y;
        y = v * n.y + y * n.x;
        return *this;
    }
    complex operator/(complex n){
        fizr();
        n.fizr();
        a /= n.a;
        t -= n.t;
        com();
        return *this;
    }
    void con(){y = -y;}
    void power(double n){
        fizr();
        a = pow(a,n);
        t *= n;
        com();
    }

};
int main(){
    char in[3];
    cin >> in[0];
    cin >> in[1];
    cin >> in[2];
    if (in[0] == 'a' && in[1] == 'd' && in[2] == 'd') {
        double a, b, c, d;
        cin >> a >> b >> c >> d;
        complex num1(a, b);
        complex num2(c, d);
        num1 + num2;
        num1.print();
    } else if (in[0] == 's' && in[1] == 'u' && in[2] == 'b') {
        double a, b, c, d;
        cin >> a >> b >> c >> d;
        complex num1(a, b);
        complex num2(c, d);
        num1 - num2;
        num1.print();
    } else if (in[0] == 'm' && in[1] == 'u' && in[2] == 'l') {
        double a, b, c, d;
        cin >> a >> b >> c >> d;
        complex num1(a, b);
        complex num2(c, d);
        num1 * num2;
        num1.print();
    } else if (in[0] == 'd' && in[1] == 'i' && in[2] == 'v') {
        double a, b, c, d;
        cin >> a >> b >> c >> d;
        complex num1(a, b);
        complex num2(c, d);
        num1 / num2;
        num1.print();
    } else if (in[0] == 'c' && in[1] == 'o' && in[2] == 'n') {
        double a, b;
        cin >> a >> b;
        complex num1(a, b);
        num1.con();
        num1.print();
    } else if (in[0] == 'p' && in[1] == 'o' && in[2] == 'w') {
        double a, b, c;
        cin >> a >> b >> c;
        complex num1(a, b);
        num1.power(c);
        num1.print();
    }
    return 0;
}