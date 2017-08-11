#include <iostream>

void swapr(int &a, int &b) {
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
}

int main(int argc, char *argv[]) {
    using namespace std;
    int wallet1 = 300;
    int wallet2 = 500;
    cout << "before wallet1 = " << wallet1;
    cout << ", wallet2 = " << wallet2 << endl;
    swapr(wallet1, wallet2);
    cout << "after wallet1 = " << wallet1;
    cout << ", wallet2 = " << wallet2 << endl;
    
    return 0;
}

//outputL:
//before wallet1 = 300, wallet2 = 500
//after wallet1 = 500, wallet2 = 300
