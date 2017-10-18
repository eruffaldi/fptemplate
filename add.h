////////////////////////////////////////////////////////////////////////////////
//
//How to add two integers of different signs where the relative size is known

template<class C> struct fail
{
	typedef typename C::failes fales;
};

template<unsigned int i, unsigned int j, bool i_pos, bool i_bigger> struct add__
{
};

template<unsigned int i, unsigned int j> struct add__<i, j, 1, 1>
{
	static const bool positive = 1;
	static const unsigned int val = i - j;
};

template<unsigned int i, unsigned int j> struct add__<i, j, 1, 0>
{
	static const bool positive = 0;
	static const unsigned int val = j - i;
};

template<unsigned int i, unsigned int j> struct add__<i, j, 0, 1>
{
	static const bool positive = 0;
	static const unsigned int val = i - j;
};

template<unsigned int i, unsigned int j> struct add__<i, j, 0, 0>
{
	static const bool positive = 1;
	static const unsigned int val = j - i;
};


////////////////////////////////////////////////////////////////////////////////
//
//How to add tow "unsigned" integers given their signs
//

//This fuction checks the signs of the numbers. If they are the same, it
//adds them, otherwise it delegates to add__
template<unsigned int i, unsigned int j, bool pi, bool pj>  struct add_
{
	typedef add__<i, j, pi, (i>j)> v;
	static const bool positive = v::positive;
	static const unsigned int val = v::val;
};

//This is the (simple) case where the signs are the same
template<unsigned int i, unsigned int j>  struct add_<i, j, 1, 1>
{
	static const bool positive = 1;
	static unsigned const int val = i + j;
};

template<unsigned int i, unsigned int j>  struct add_<i, j, 0, 0>
{
	static const bool positive = 0;
	static unsigned const int val = i + j;
};



//This function decides if any arguments are negative. If so, it does
//the right thing. Otherwise, it loses precision from the smaller 
//number to get the mantissas the same, then delegates to add_
template<class A_, class B_, int a_zero, int b_zero> struct add___
{

	template<class C> struct denorm
	{
		typedef typename general_normalize<C, max_digit - 1>::val val;
	};

	//First, denormalize the two numbers (so that they can always be added)
	//Add may cause overflow.
	typedef typename denorm<A_>::val Ad;
	typedef typename denorm<B_>::val Bd;

	//Now, loose precision from the smaller number
	static const int dir = Ad::expo - Bd::expo;



	template<class F, int n, int n_too_big> struct shifter__
	{
		typedef fp<F::mant >> n, F::expo + n, F::posi> val;
	};

	template<class F, int n> struct shifter__<F, n, 1>
	{
		typedef fp<0> val;
	};

	template<class F, int n, int doit> struct shifter_
	{
		//We have to switch on the size of the shift to prevent the compiler
		//warning about shifts which are wider than the type.
		typedef typename shifter__<F, n, (n > max_digit)>::val val;
	};

	template<class F, int n> struct shifter_<F, n, 0>
	{
		typedef F val;
	};

	template<class F, int dir> struct shifter
	{
		//Don't do negative shifts
		//Ie, only loose precision
		typedef typename shifter_<F, dir, (dir > 0) >::val val;
	};


	typedef typename shifter<Ad,-dir>::val As;
	typedef typename shifter<Bd, dir> ::val Bs;

	typedef add_<As::mant, Bs::mant, As::posi, Bs::posi> v;
	
	typedef fp<v::val, As::expo, v::positive> val_u;

	typedef typename normalize<val_u>::val val;
};

template<class A, class B> struct add___<A, B, 0, 1>
{
	typedef A val;
};

template<class A, class B> struct add___<A, B, 1, 0>
{
	typedef B val;
};

template<class A, class B> struct add___<A, B, 1, 1>
{
	typedef fp<0> val;
};

template<class A, class B> struct add
{
	typedef typename add___<A, B, is_zero<A>::val, is_zero<B>::val>::val val;
	FP_MAKE(val);
};


template<class A, class B> struct subtract
{
	typedef add<A, neg<B> > val;
	FP_MAKE(val);
};
