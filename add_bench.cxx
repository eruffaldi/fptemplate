#include <iostream>
#include <math.h>

#include "fp_template.h"

using namespace FPTemplate;
using namespace std;

#define A(X) multiply<X, fp<1> >

int main()
{
	print<exponentiate<fp<1> > >();
}
