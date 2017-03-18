#include <iostream>
#include "bmp.h"

using namespace std;

int main(void)
{
	Bmp test("in.bmp");
	test.resize(64,64);

	test.write("out.bmp");
	return 0;
}
