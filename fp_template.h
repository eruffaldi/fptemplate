#ifndef INC_FP_TEMPLATE_H
#define INC_FP_TEMPLATE_H

#include <iostream>
#include <cmath>

namespace FPTemplate
{
	static const int max_digit = 31;
	static const int max_decimal_digit = 9;
	static const unsigned int top_bits = 0xffff0000;
	static const unsigned int bot_bits = 0x0000ffff;
	static const int half_width =16;
	static const int divide_iterations=2;
	static const int sqrt_iterations = 5;
	static const int exp_terms = 13;
	static const int biggest_exp = 2;

	//All floating point types need to look like this.
	//This is a basic way of generating one.
	template<unsigned int M, int E=0, bool P=1> struct fp
	{
		static const unsigned int		mant = M;
		static const int				expo = E;
		static const bool 				posi = P;
	};


	//Generate a floating point type with easy sign.
	template<int M> struct fps
	{
			static const unsigned int 	mant = M>0?M:-M;
			static const int 			expo = 0;
			static const bool			posi = M>=0?1:0;
	};
	
	//Some useful numbers
	namespace Nums
	{
		typedef fp<2918732882UL, -30> e;
		typedef fp<3037000501UL, -31> sqrt_2;
		typedef fp<3719599941UL, -31> sqrt_3;
		typedef fp<0>				  zero;
		typedef fp<1>				  one;
	}
	
	//Runtime evaluation
	template<class T> double eval()
	{
		if(T::mant == 0)
			return 0;
		else
			return  (T::posi?1:-1)* (double)T::mant * std::pow(2., (double) T::expo);
	}

	template<class T> void print()
	{
		std::cout.precision(15);
		std::cout << "Value:    " << eval<T>() << std::endl
			 << "Mantissa: " << T::mant  << "  0x" << std::hex << T::mant << std::dec << std::endl
			 << "Exponent: " << T::expo  << std::endl << std::endl;;
	}

	
	//We use this later in error checking
	template<class C> struct dummy{};
	template<class C> struct error_check{};

	#define FP_ERROR_TEST(ERR,NAME)\
	template<int i, class C=int> struct ERR##_error{};\
	template<class C> struct ERR##_error<0, C>: public dummy<C>\
	{\
		typedef Nums::zero val;\
		typedef typename dummy<C>::NAME##_in_##ERR f;\
	}


	//Use this inside a type to make it in to a floating point, from a given typedef
	#define FP_MAKE(...)\
		static const unsigned int 	mant = __VA_ARGS__::mant;\
		static const int 			expo = __VA_ARGS__::expo;\
		static const bool 			posi = __VA_ARGS__::posi
	#define FP_BITS(M, E, P)\
		static const unsigned int 	mant = M;\
		static const int 			expo = E;\
		static const bool 			posi = P



	#include "misc.h"
	#include "normalize.h"
	#include "factorial.h"
	#include "add.h"
	#include "multiply.h"
	#include "intpow.h"
	#include "tests.h"
	#include "divide.h"
	#include "misc2.h"
	#include "exp.h"
	#include "sqrt.h"
	#include "fpd.h"
	#include "to_float.h"
}
#endif
