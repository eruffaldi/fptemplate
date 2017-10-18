template<class A_, class B_> struct multiply
{
	
	typedef normalize<A_> A;
	typedef normalize<B_> B;


	//We don't have enough width to do full width multiplication,
	//so instead, we extract the upper and lower halves of the
	//two numbers and do long multiplication.


	//Extract the halves
	static const unsigned int A_u  =  (A::mant & top_bits) >> half_width;
	static const unsigned int B_u  =  (B::mant & top_bits) >> half_width;

	static const unsigned int A_l  =  (A::mant & bot_bits);
	static const unsigned int B_l  =  (B::mant & bot_bits);

	//Perform the multiplications
	static const unsigned int uu = A_u * B_u;	
	static const unsigned int m1 = A_u * B_l;
	static const unsigned int m2 = A_l * B_u;
	static const unsigned int ll = A_l * B_l;

	//Add the multiplied bits with the correct shift
	static const unsigned int res = uu + ((m1 + m2 + ll >> half_width) >> half_width);

	static const bool sign = !(A::posi ^ B::posi);

	typedef normalize<fp<res, A::expo + B::expo + half_width * 2, sign> > val;

	FP_MAKE(val);
};
