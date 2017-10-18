//We could use pow, etc, for clarity, but it does not get evaluated
//at compile time. By writing out pow by hand here using macros
//and templates, gcc at any rate will evaluate the whole expression
//at compile time leaving behind a single literal floating point
//constant.

#define FP_SQ(X) (((double)X)*((double)X))

//We could probably impove this with ?: and hex floating point constants.
//Will the compiler do ?: of fp types at compile time?
#define FP_BIT0(X) X   //feed in 1,2
#define FP_BIT1(X) X   //feed in 1,4
#define FP_BIT2(X) X   //feed in 1,16
#define FP_BIT3(X) X   //feed in 1,256
#define FP_BIT4(X) X   //feed in 1,65536 and hereafter
#define FP_BIT5(X) FP_SQ(X)
#define FP_BIT6(X) FP_SQ(FP_SQ(X))
#define FP_BIT7(X) FP_SQ(FP_SQ(FP_SQ(X)))
#define FP_BIT8(X) FP_SQ(FP_SQ(FP_SQ(FP_SQ(X))))
#define FP_BIT9(X) FP_SQ(FP_SQ(FP_SQ(FP_SQ(FP_SQ(X)))))
template<class C> struct to_double
{
	//Result is:
	//(float)sign * mantissa * mul1*mul2*.../div1/div2/div3...
	//Where mul and div are all powers of powers of 2

	//Sign
	static const int sign = C::posi?1:-1;
	
	//Mantissa
	static const unsigned int mantissa = C::mant;


	//64 bit IEEE doubles have 11 bits of exponent (-1024 to 1023)
	//FIXME: we will loose really small doubles here, because
	//we might have 2^32 * 2^-1024, which is > 2^-1024
	//Oh, well

	//We need an unsigned exponent to perform bitwise operations below.
	static const int uexpo= C::expo > 0 ? C::expo : -C::expo;
	static const int bad_expo = uexpo > 1023;
	
	//We don't wish to be doing anything with oversized exponents
	static const int expo = bad_expo ? 0 : uexpo;

	//Something to multiply or divide by to generate zero or inf,
	//in the case where the mantissa is too big or small
	static const int zero = (bad_expo && C::expo < 0) ? 0 : 1;
	static const int inf  = (bad_expo && C::expo > 0) ? 0 : 1;

	static const int m_bit0 = 1 + ((expo &      0x1) && (C::expo > 0));
	static const int m_bit1 = 1 + ((expo &      0x2) && (C::expo > 0))*3;
	static const int m_bit2 = 1 + ((expo &      0x4) && (C::expo > 0))*15;
	static const int m_bit3 = 1 + ((expo &      0x8) && (C::expo > 0))*255;
	static const int m_bit4 = 1 + ((expo &     0x10) && (C::expo > 0))*65535;
	static const int m_bit5 = 1 + ((expo &     0x20) && (C::expo > 0))*65535;
	static const int m_bit6 = 1 + ((expo &     0x40) && (C::expo > 0))*65535;
	static const int m_bit7 = 1 + ((expo &     0x80) && (C::expo > 0))*65535;
	static const int m_bit8 = 1 + ((expo &    0x100) && (C::expo > 0))*65535;
	static const int m_bit9 = 1 + ((expo &    0x200) && (C::expo > 0))*65535;

	static const int d_bit0 = 1 + ((expo &      0x1) && (C::expo < 0));
	static const int d_bit1 = 1 + ((expo &      0x2) && (C::expo < 0))*3;
	static const int d_bit2 = 1 + ((expo &      0x4) && (C::expo < 0))*15;
	static const int d_bit3 = 1 + ((expo &      0x8) && (C::expo < 0))*255;
	static const int d_bit4 = 1 + ((expo &     0x10) && (C::expo < 0))*65535;
	static const int d_bit5 = 1 + ((expo &     0x20) && (C::expo < 0))*65535;
	static const int d_bit6 = 1 + ((expo &     0x40) && (C::expo < 0))*65535;
	static const int d_bit7 = 1 + ((expo &     0x80) && (C::expo < 0))*65535;
	static const int d_bit8 = 1 + ((expo &    0x100) && (C::expo < 0))*65535;
	static const int d_bit9 = 1 + ((expo &    0x200) && (C::expo < 0))*65535;
};

#define FP_TO_DOUBLE(...) FP_TO_DOUBLE_(__VA_ARGS__)

#define FP_TO_DOUBLE_(...)\
((double)FPTemplate::to_double<__VA_ARGS__ >::sign * FPTemplate::to_double<__VA_ARGS__ >::mantissa  * FPTemplate::to_double<__VA_ARGS__ >::zero\
* FP_BIT0((FPTemplate::to_double<__VA_ARGS__ >::m_bit0)) \
* FP_BIT1((FPTemplate::to_double<__VA_ARGS__ >::m_bit1)) \
* FP_BIT2((FPTemplate::to_double<__VA_ARGS__ >::m_bit2)) \
* FP_BIT3((FPTemplate::to_double<__VA_ARGS__ >::m_bit3)) \
* FP_BIT4((FPTemplate::to_double<__VA_ARGS__ >::m_bit4)) \
* FP_BIT5((FPTemplate::to_double<__VA_ARGS__ >::m_bit5)) \
* FP_BIT6((FPTemplate::to_double<__VA_ARGS__ >::m_bit6)) \
* FP_BIT7((FPTemplate::to_double<__VA_ARGS__ >::m_bit7)) \
* FP_BIT8((FPTemplate::to_double<__VA_ARGS__ >::m_bit8)) \
* FP_BIT9((FPTemplate::to_double<__VA_ARGS__ >::m_bit9)) \
/ FP_BIT0((FPTemplate::to_double<__VA_ARGS__ >::d_bit0)) \
/ FP_BIT1((FPTemplate::to_double<__VA_ARGS__ >::d_bit1)) \
/ FP_BIT2((FPTemplate::to_double<__VA_ARGS__ >::d_bit2)) \
/ FP_BIT3((FPTemplate::to_double<__VA_ARGS__ >::d_bit3)) \
/ FP_BIT4((FPTemplate::to_double<__VA_ARGS__ >::d_bit4)) \
/ FP_BIT5((FPTemplate::to_double<__VA_ARGS__ >::d_bit5)) \
/ FP_BIT6((FPTemplate::to_double<__VA_ARGS__ >::d_bit6)) \
/ FP_BIT7((FPTemplate::to_double<__VA_ARGS__ >::d_bit7)) \
/ FP_BIT8((FPTemplate::to_double<__VA_ARGS__ >::d_bit8)) \
/ FP_BIT9((FPTemplate::to_double<__VA_ARGS__ >::d_bit9))  / FPTemplate::to_double<__VA_ARGS__ >::inf)


#define FP_TO_FLOAT(...) ((float)FP_TO_DOUBLE(__VA_ARGS__))
