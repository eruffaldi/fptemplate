// 1 + x + x^2/2! + x^3/3! + ...


//exp__ works out the exponential of a number using a Taylor series expansion
//This is only accurate for smallish values of x in e^x
template<class Sum, class X, class Xn, class Fact, int i> struct exp_
{
	typedef typename multiply<X, Xn>::val next_X;
	typedef typename multiply<Fact, fp<i> >::val next_fact;

	typedef typename add<Sum, typename divide<next_X, next_fact>::val>::val next_Sum;

	typedef typename exp_<next_Sum, X, next_X, next_fact, i+1>::val val;
};

template<class Sum, class X, class Xn, class Fact> struct exp_<Sum, X, Xn, Fact, exp_terms>
{
	typedef Sum val;
};

//This initializes the taylor series with the correct terms
template<class F> struct exponentiate__
{
	typedef typename exp_<fp<1>, F, fp<1>, fp<1>, 1>::val val;
};


//Since the Taylor series is only accurate with small numbers, this calculates:
//
// e^x = (...(((e^(x/2^n))^2)^2)...)^2
//
//so the exponential in the middle is quite small
//
//This isn't a good way of calculating exps, since the repeated squaring loses
//accuracy rather badly.
template<class F, bool F_greater_than_2> struct exponentiate_
{
	typedef typename divide_2<F>::val F_on_2;
	static const bool next_F_gt_N = gt<F_on_2, fp<biggest_exp> >::val;
	
	typedef typename square<typename exponentiate_<F_on_2, next_F_gt_N>::val>::val val;
};

template<class F> struct exponentiate_<F, 0>
{
	typedef typename exponentiate__<F>::val val;
};

template<class F_> struct exponentiate
{
	template<class C, int posi> struct q_reciprocate
	{
		typedef typename reciprocate<C>::val val;
	};

	template<class C> struct q_reciprocate<C, 1>
	{
		typedef C val;
	};
	
	typedef typename f_abs<F_>::val F;
	typedef typename exponentiate_<F, gt<F, fp<2> >::val >::val val_exp;
	typedef typename q_reciprocate<val_exp, F_::posi>::val val;	

	FP_MAKE(val);
};
