#include <iostream>
#include <math.h>

#include "fp_template.h"

using namespace FPTemplate;
using namespace std;

int main()
{

	//Basic floating point input: mantissa, exponent=0, sign=0
	print<fp<5> >();

	//Signed floating point input (mantissa only)
	print<fps<-1> >();

	//General floating point input: look at fpd.h
	// fpd<a,b> == a.b
	//slow and a bit inaccurate for powers of 2 in the decimal part
	print<fpd<-1,333333> >();
	print<fpd<0,333333> >();

	//Pop quiz: how do you get -0.333?



	
	//Basic stuff, look at misc.h, misc2.h
	print<neg<fps<27> > >();
	print<f_abs<fps<-3> > >();
	print<fpd<-1,333333> >();


	//Addition, subtraction, look at add.h
	//This is probably the most complex file
	print<add<fpd<-4>, fpd<1,5> > >();
	print<subtract<fpd<7>, fpd<1> > >();

	//Multiplication, look at multiply.h
	print<multiply<fp<3,-1>, fp<7> > >();

	//Simple positive integer powers, intpow.h
	print<intpow<fp<5>, 20> >();

	//Tests, tests.h
	cout << eq<fp<5>, fp<2> >::val << endl;
	cout << gt<fp<5>, fp<2> >::val << endl;
	//etc...


	//Division, reciprocation. S...l...o...w... divide.h
	print<reciprocate<fp<3> > >();
	print<reciprocate<fp<8> > >();
	print<divide<fp<3>, fpd<0,333333333> > >();

	//Square roots. Even slower. sqrt.h
	print<square_root<fp<2> > >();

	//Exponentials. Very, very, very slow. exp.h
	print<exponentiate<fp<1> > >();

	//A bit inaccurate for large numbers
	print<exponentiate<fp<65> > >();
	
	//A bit inaccurate for large numbers


	//Examine the assembly code here and for foo().
	//gcc gives literal constants instead of expressions, 
	//which is very, very nice (even with -O0).
	//cout << FP_TO_FLOAT(fp<1,6000>) << endl;
	cout << FP_TO_FLOAT(fp<1,-6000>) << endl;
	cout << FP_TO_FLOAT(Nums::e) << endl;
}

float f;
double d;
void foo()
{
	typedef fp<1,-1> half;
	f = FP_TO_FLOAT(fp<1>);
	d = FP_TO_DOUBLE(half);

}

