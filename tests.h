////////////////////////////////////////////////////////////////////////////////
//
// Perform a whole bunch of simple arithmetic tests
//

template<class A_, class B_> struct eq
{
	typedef normalize<A_> A;
	typedef normalize<B_> B;

	static const int val = (A::mant == B::mant && A::expo == B::expo && A::posi == B::posi);
};

template<class A, class B> struct gt
{
	static const bool val = add<A, neg<B> >::posi;
};

template<class A, class B> struct less_eq
{
	static const bool val =  ! gt<A,B>::val;
};

template<class A, class B> struct lt
{
	static const bool val =  add<B, neg<A> >::posi;
};

template<class A, class B> struct gt_eq
{
	static const bool val = ! lt<A,B>::val;
};

