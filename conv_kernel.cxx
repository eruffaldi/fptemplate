#include <iostream>

#include "fp_template.h"

using namespace FPTemplate;
using namespace std;


//This class casts an fp type to an int.
template<class F> struct to_int
{
	//We need a better shifter to trap/avoid errors
	template<class C, bool exp_positive> struct shifter
	{
		static const unsigned int val = C::mant << C::expo;
	};

	template<class C> struct shifter<C, 0>
	{
		static const unsigned int val = C::mant >> -C::expo;
	};

	static const int val = shifter<F, (F::expo>0) >::val * (2*F::posi-1);
};


//This computes Gaussian(sigma,x)
template<class Sigma, class C> struct gauss
{	
	//We could do rather better here for pi.
	typedef fpd<3,14159> pi;
	typedef times_2<square<Sigma> > two_ss;
	typedef reciprocate<square_root<multiply<pi, two_ss> > > norm_factor;
	typedef divide<neg<square<C> >, two_ss> ev;

	typedef multiply<exponentiate<ev>, norm_factor> val;

	FP_MAKE(val);
};


template<class Sigma> struct gaussian_convolve
{
	//radius is s*sigma
	static const int radius = to_int<add<multiply<Sigma,fp<3> >, fp<1,-1,1> > >::val;

	//We could probably compute the best precision-preserving sigma.
	//but it must be a power of 2 to make the last division fast
	static const int scale=4096;

	//dd is a dummy class so we don't have to totally specify 
	//this struct.
	template<class dd, int pos> struct conv
	{
		inline static void convolve(int*ptr,int& acc)
		{
			typedef gauss<Sigma, fps<pos> > num;

			//Exploit symmetry
			acc += (int)(FP_TO_DOUBLE(num)*scale) * (ptr[pos] + ptr[-pos]);
			conv<int, pos-1>::convolve(ptr, acc);
		}

		inline static void print()
		{
			typedef gauss<Sigma, fps<pos> > num;
			cout << pos << " " << (int)(FP_TO_DOUBLE(num)*scale) << endl;
			cout << -pos << " " << (int)(FP_TO_DOUBLE(num)*scale) << endl;
			conv<int, pos-1>::print();
		}
	};

	template<class dd> struct conv<dd, 0>
	{
		static void convolve(int*ptr,int& acc)
		{
			typedef gauss<Sigma, fp<0> > num;
			acc += (int)(FP_TO_DOUBLE(num)*scale) * *ptr;
		}

		static void print()
		{
			typedef gauss<Sigma, fp<0> > num;
			cout << 0 << " " << (int)(FP_TO_DOUBLE(num)*scale) << endl;
		}
	};

	inline static void print()
	{
		conv<int, radius>::print();
	}

	inline static int convolve(int* data)
	{
		int acc = 0;
		conv<int, radius>::convolve(data, acc);
		return acc / scale;	
	}

};

int main()
{
	typedef fp<5,-1,1> s;
	gaussian_convolve<s>::print();
}

int external_num;
int* external_data;

int foo()
{
	typedef fp<5,-1,1> s;
	external_num = gaussian_convolve<s>::convolve(external_data);
}
