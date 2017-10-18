template<unsigned int mant, int i=max_digit> struct get_top_digit_
{
	static const int val = (mant & (1U<<i)) ? i : get_top_digit_<mant, i-1>::val;
};

template<unsigned int mant> struct get_top_digit_<mant, -1>
{
	static const int val = -1;
};


template<class C> struct get_top_digit
{
	static const int val = get_top_digit_<C::mant>::val;
};

template<class C, int first> struct general_normalize
{
	//This class normalizes a floating point number so that
	//the top digit is in the specified position.

	static const int top_digit=get_top_digit<C>::val;

	
	//Number too small, so right left
	template<class F, int top, int is_big> struct shifter
	{
		static const int shift = first - top;
		typedef fp<(F::mant << shift), F::expo - shift, F::posi> val;
	};
	
	//Number too big, so right shift
	template<class F, int top> struct shifter<F, top, 1>
	{
		static const int shift = top - first;
		typedef fp<(F::mant >> shift), F::expo + shift, F::posi> val;
	};

	//Special case for zero	
	template<class F> struct shifter<F, -1, 0>
	{
		typedef fp<0> val;
	};

	typedef typename shifter<C, top_digit, (top_digit > first)>::val val;

	FP_MAKE(val);
};


template<class C> struct normalize
{
	typedef general_normalize<C, max_digit> val;
	FP_MAKE(val);
};
