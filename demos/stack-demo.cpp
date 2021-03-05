#include <iostream>
#include <cstdio>

using namespace std;

int function(int para1, int para2, int para3) {
    int var1, var2, var3;
    var1 = 1;
    var2 = 2;
    var3 = 3;
    return (para1 + para2 + para2 + var1 + var2 + var3);
}

int main(int argc, char * argv[]) {
    printf("before function call\n");
    int n = function(10, 20, 30);
    printf("result = %d\n", n);
    return 0;
}