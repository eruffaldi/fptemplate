//x = sqrt(a)
//x^2 - a = 0
//
// Using Newton-Raphson
//
//x <- x - (x^2 - a)/2x
//x <- (x + a/x)/2

template<class X, class A, int n> struct sqrt_
{
	typedef typename sqrt_<divide_2<add<X,divide<A,X> > >, A, n-1>::val val;
};


template<class X, class A> struct sqrt_<X, A, 0>
{
	typedef X val;
};

//FP_ERROR_TEST(sqrt, domain_error);


template<class F_> struct square_root_
{
	// x = a * 2^n
	// sqrt(x) = sqrt(a) * 2^(n/2)
	// sqrt(x) = sqrt(2*a) * 2^(n-1/2)
	// we want n even, since this is trivial
	// Then calculating sqrt(a) requires far fewer iterations, since
	// a is close to 1 so we have a good estimage of a: 1.
	
	
	typedef normalize<F_> F;
	
	//Make n even
	static const int n_2 = ((F::expo - -max_digit)/2);
	
	//Compute a above
	typedef fp<F::mant, F::expo - 2*n_2> A;

	typedef typename sqrt_<fp<1>,A,sqrt_iterations>::val sqrt_A;

	typedef fp<sqrt_A::mant, n_2 + sqrt_A::expo, 1> val;

};

template<class F, int i> struct sqrt_error : public square_root_<F>
{
};

template<class F> struct sqrt_error<F, 0>
{	
	typedef typename error_check<F>::attempting_negative_square_root foo;
	typedef Nums::zero val;
};

template<class F> struct square_root : public sqrt_error<F, F::posi>
{
	static const unsigned int mant = sqrt_error<F,F::posi>::val::mant;
	static const int 		  expo = sqrt_error<F,F::posi>::val::expo;
	static const bool 		  posi = sqrt_error<F,F::posi>::val::posi;
	//FP_MAKE(typename sqrt_error<F, F::posi>::val);
};
