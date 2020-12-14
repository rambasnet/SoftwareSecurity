// unittesting fruitful functions, v.2
// a better way using function to write all the test cases
#include <iostream>
#include <cassert>

using namespace std;

// function prototypes
long addfunc(int, int);
void test_addfunc();

int main(int argc, char* argv[]) {
    if (argc == 2 and string(argv[1]) == "test")
        // do automatic testing of addfunc
        test_addfunc();
    else {
        // manual testing
        long answer;
        int n1, n2;
        cout << "Enter two integers separated by space: ";
        cin >> n1 >> n2;
        // calculate answer
        answer = addfunc(n1, n2);
        cout << n1 << " + " << n2 << " = " << answer << endl;
    }
    return 0;
}

// function adds two numbers and returns the sum
long addfunc(int num1, int num2) {
    return num1 + num2;
}

void test_addfunc() {
    assert(addfunc(99, 1) == 100);
    assert(addfunc(-55, -45) == -100);
    assert(addfunc(950, 0) == 950);
    assert(addfunc(1000, -100) == 900);
    cerr << "all test cases passed!\n";
}