FP_ERROR_TEST(intpow, neagtive_power);

template<class F, int i> struct intpow: public intpow_error<F::posi>
{
	typedef typename multiply<F, typename intpow<F,i-1>::val >::val val;
	FP_MAKE(val);
};

template<class F> struct intpow<F, 0>
{
	typedef fp<1> val;
	FP_MAKE(val);
};
