template<class F> struct square
{
	typedef multiply<F, F> val;
	FP_MAKE(val);
};
