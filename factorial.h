template<int m> struct intfact
{
	static const int val = m * intfact<m-1>::val;
};

template<> struct intfact<0>
{
	static const int val = 1;
};


template<int n> struct factorial
{
	typedef fp<intfact<n>::val > val;
	FP_MAKE(val);
};
