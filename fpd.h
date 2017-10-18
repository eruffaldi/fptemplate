// do decimal floating points
// fpd<256,3314> = 256.3314
template<int N, int D=0> struct fpd
{
	typedef fps<(N>=0?N:-N)> N_val;

	

	template<int n, int pow_digit, int digit> struct int_lg_10
	{
		static const int val = pow_digit > n ? digit: (int_lg_10<n, 10*pow_digit, digit+1>::val);
	};
		
	template<int n, int pow_digit> struct int_lg_10<n, pow_digit, max_decimal_digit>
	{
		static const int val = max_decimal_digit;
	};
	
	static const int D_digits = int_lg_10<D,1,0>::val;

	typedef divide<fps<D>, intpow<fp<10>, D_digits> > D_val;

	typedef add<N_val, D_val> foo;

	//typedef set_sign<add<N_val, divide<fps<D>, intpow<fp<10>, D_digits> > >, (N>=0)> val;
	typedef set_sign<foo, (N>=0)> val;

	FP_MAKE(val);
};
