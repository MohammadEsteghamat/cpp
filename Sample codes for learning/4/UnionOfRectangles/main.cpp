#include <iostream>
using namespace std;
struct c{
    double  a,ca;
};
class Rectangle{
    friend Rectangle union_Rectangle(Rectangle ,Rectangle );
private:
    double length,width,center_x,center_y;
    double x[2],y[2];
public:
    void set_Rectangle(double a,double b,double c,double d){
        length = a;
        width = b;
        center_x = c;
        center_y = d;
        x[0] = center_x - length/2;
        x[1] = center_x + length/2;
        y[0] = center_y - width/2;
        y[1] = center_y + width/2;
    }
    double area(){return length*width;}
};
c union_help(double x1,double x2,double x3,double x4){
    c elm;

    if(x1<=x3 && x3<=x2){ //   x1 < x3 < x2
        if(x4<=x2){       //       x4 < x2
            elm.a = x4 - x3;
            elm.ca = (x4+x3)/2;
        }
        else if(x2<=x4){  //        x2 < x4
            elm.a = x2 - x3;
            elm.ca = (x2 + x3)/2;
        }
    }
    else if(x1<x4 && x4<x2){ // x1 < x4 << x2
        if(x1>=x3){          // x3 < x1
            elm.a = x4 - x1;
            elm.ca = (x1 + x4)/2;
        }
    }
    else if(x3<x1 && x2<x4){
        elm.a = x2 - x1;
        elm.ca = (x1+x2)/2;
    }
    else{
        elm.a = 0;
        elm.ca = 0;
    }
    return elm;
}
Rectangle union_Rectangle(Rectangle a,Rectangle b){
    c ux,uy;
    ux = union_help(a.x[0],a.x[1],b.x[0],b.x[1]);
    uy = union_help(a.y[0],a.y[1],b.y[0],b.y[1]);
    Rectangle c;
    c.set_Rectangle(ux.a,uy.a,ux.ca,uy.ca);
    return c;
}

int main(){
    int n{0};
    double temp{0},temp1{0},temp2{0},temp3{0};
    cin>>n;
    Rectangle *arr_rect = new Rectangle[n];
    for (int i = 0; i < n; ++i) {
        cin>>temp>>temp1>>temp2>>temp3;
        arr_rect[i].set_Rectangle(temp,temp1,temp2,temp3);
    }
    for (int i = 1; i < n; ++i) {
        arr_rect[0] = union_Rectangle(arr_rect[0],arr_rect[i]);
    }
    cout<<arr_rect[0].area();
    return 0;
}