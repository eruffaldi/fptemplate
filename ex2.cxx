#include <iostream>
#include <math.h>

#include "fp_template.h"

using namespace FPTemplate;
using namespace std;

int main()
{
	//print<exponentiate<add<fp<1>, divide<fp<1>, Nums::zero> > > >();

	//print<divide<fp<1>, fp<0> > >();

	print<square_root<fp<1> > >();
	print<square_root<fps<-1> > >();


	
}

