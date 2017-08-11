#include <iostream>

int main(int argc, char *argv[]) {
    using namespace std;
    int rats = 101;
    int & rodents = rats;
    
    cout << "rats = " << rats;
    cout << ", rodents =  " << rodents << endl;
    
    cout << "rats address = " << &rats;
    cout << ", rodents address =  " << &rodents << endl;
    
    int bunnies = 50;
    rodents = bunnies;
    
    cout << "rats = " << rats;
    cout << ", bunnies = " << bunnies;
    cout << ", rodents =  " << rodents << endl;
    
    cout << "rats address = " << &rats;
    cout << ", bunnies address = " << &bunnies;
    cout << ", rodents address =  " << &rodents << endl;
    
    return 0;
}

//output:
//rats = 101, rodents =  101
//rats address = 0x7fff581d881c, rodents address =  0x7fff581d881c
//rats = 50, bunnies = 50, rodents =  50
//rats address = 0x7fff581d881c, bunnies address = 0x7fff581d880c, rodents address =  0x7fff581d881c
