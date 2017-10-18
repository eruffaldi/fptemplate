template<class F> struct neg
{
	typedef fp<F::mant, F::expo, !F::posi> val;

	FP_MAKE(val);
};

//We need this for add, and we need add for the other tests,
//so it's defined here, not in tests.h
template<class A> struct is_zero
{
	static const int val = (A::mant==0);	
};


template<class F> struct f_abs
{
	typedef fp<F::mant, F::expo, 1> val;
	FP_MAKE(val);
};

template<class F> struct times_2
{
	typedef fp<F::mant, F::expo+1, F::posi> val;
	FP_MAKE(val);
};

template<class F> struct divide_2
{
	typedef fp<F::mant, F::expo-1, F::posi> val;
	FP_MAKE(val);
};

template<class F, bool positive> struct set_sign
{
	typedef fp<F::mant, F::expo, positive> val;
	FP_MAKE(val);
};
