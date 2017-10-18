//Solve the equation f(x) = 0, f(x): x-> 1/x - a
//ie, find 1/a
//
// x <- 2x - ax^2   (by Newton's method)
//
// Well, whaddya know! No divisions required!
//
// Fortunately, multiplications are quite cheap (ish)

template<class X, class A, int i> struct divide_newton
{
	typedef times_2<X> 					X_times_2;
	typedef multiply<X,X> 				X_squared;
	typedef multiply<X_squared, A> 		AX2;
	typedef neg<AX2> 					minus_AX2;
	typedef add<X_times_2, minus_AX2> 	next;

	typedef typename divide_newton<next, A, i-1>::val val;
};

template<class X, class A> struct divide_newton<X, A, 0>
{
	typedef X val;
};


//This will allow us to solve the reciprocal using Newton's method
template<class A_> struct approx_reciprocate
{
	//Find an approximate division by performing integer division on
	//the mantissas, where the divisor has half the precision of the
	//divisee
	typedef general_normalize<A_, max_digit/2> A_denorm;
	typedef normalize<fp<1> > one;

	
	static const unsigned int mantissa = one::mant / A_denorm::mant;

	typedef fp<mantissa, -A_denorm::expo - max_digit, A_denorm::posi> val;
};


template<class A_> struct reciprocate_
{
	typedef typename approx_reciprocate<A_>::val  approx;
	typedef typename divide_newton<approx, A_, divide_iterations>::val val;

};

template<class C, int i> struct reciprocate_error: public reciprocate_<C>
{
};

template<class C> struct reciprocate_error<C,0>
{
	typedef typename dummy<C>::divide_by_zero foo;
	typedef Nums::one val;
};

template<class C> struct reciprocate: public reciprocate_error<C, ! eq<C, fp<0> >::val>
{
	FP_MAKE(reciprocate_error<C, ! eq<C, fp<0> >::val>::val);
};

template<class A, class B> struct divide_
{
	typedef multiply<A, reciprocate<B> > val;
};

template<class C, class D, int i> struct divide_error: public divide_<C,D>
{
};

template<class C, class D> struct divide_error<C,D,0>
{
	typedef typename error_check<C>::divide_by_zero foo;
	typedef Nums::one val;
};


template<class A, class B> struct divide: public divide_error<A, B, !eq<B,fp<0> >::val>
{
	FP_MAKE(divide_error<A, B, ! eq<B, fp<0> >::val>::val);
	//FP_MAKE(val);
};


