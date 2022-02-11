#include <iostream>
#include <cstdio>

using namespace std;

void countDown(int n) {
	printf("Countdown from %d\n", n);
	for(int i=n; i>0; i--) {
		cout << i << endl;
	}
	cout << "Blast off!" << endl;
	return;
}

int main(int argc, char **argv) {
	countDown(10);
	return 0;
}
