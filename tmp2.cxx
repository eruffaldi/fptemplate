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



        template<unsigned int M, int E=0, bool P=1> struct fp
        {
                static const unsigned int mant = M;
                static const int expo = E;
                static const bool posi = P;
        };



        template<int M> struct fps
        {
                        static const unsigned int mant = M>0?M:-M;
                        static const int expo = 0;
                        static const bool posi = M>=0?1:0;
        };


        namespace Nums
        {
                typedef fp<2918732882UL, -30> e;
                typedef fp<3037000501UL, -31> sqrt_2;
                typedef fp<3719599941UL, -31> sqrt_3;
                typedef fp<0> zero;
                typedef fp<1> one;
        }


        template<class T> double eval()
        {
                if(T::mant == 0)
                        return 0;
                else
                        return (T::posi?1:-1)* (double)T::mant * std::pow(2., (double) T::expo);
        }

        template<class T> void print()
        {
                std::cout.precision(15);
                std::cout << "Value:    " << eval<T>() << std::endl
                         << "Mantissa: " << T::mant << "  0x" << std::hex << T::mant << std::dec << std::endl
                         << "Exponent: " << T::expo << std::endl << std::endl;;
        }



        template<class C> struct dummy{};
        template<class C> struct error_check{};
template<class F> struct neg
{
        typedef fp<F::mant, F::expo, !F::posi> val;

        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};



template<class A> struct is_zero
{
        static const int val = (A::mant==0);
};


template<class F> struct f_abs
{
        typedef fp<F::mant, F::expo, 1> val;
        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};

template<class F> struct times_2
{
        typedef fp<F::mant, F::expo+1, F::posi> val;
        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};

template<class F> struct divide_2
{
        typedef fp<F::mant, F::expo-1, F::posi> val;
        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};

template<class F, bool positive> struct set_sign
{
        typedef fp<F::mant, F::expo, positive> val;
        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};
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



        static const int top_digit=get_top_digit<C>::val;



        template<class F, int top, int is_big> struct shifter
        {
                static const int shift = first - top;
                typedef fp<(F::mant << shift), F::expo - shift, F::posi> val;
        };


        template<class F, int top> struct shifter<F, top, 1>
        {
                static const int shift = top - first;
                typedef fp<(F::mant >> shift), F::expo + shift, F::posi> val;
        };


        template<class F> struct shifter<F, -1, 0>
        {
                typedef fp<0> val;
        };

        typedef typename shifter<C, top_digit, (top_digit > first)>::val val;

        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};


template<class C> struct normalize
{
        typedef general_normalize<C, max_digit> val;
        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};
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
        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};




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
template<unsigned int i, unsigned int j, bool pi, bool pj> struct add_
{
        typedef add__<i, j, pi, (i>j)> v;
        static const bool positive = v::positive;
        static const unsigned int val = v::val;
};


template<unsigned int i, unsigned int j> struct add_<i, j, 1, 1>
{
        static const bool positive = 1;
        static unsigned const int val = i + j;
};

template<unsigned int i, unsigned int j> struct add_<i, j, 0, 0>
{
        static const bool positive = 0;
        static unsigned const int val = i + j;
};






template<class A_, class B_, int a_zero, int b_zero> struct add___
{

        template<class C> struct denorm
        {
                typedef typename general_normalize<C, max_digit - 1>::val val;
        };



        typedef typename denorm<A_>::val Ad;
        typedef typename denorm<B_>::val Bd;


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


                typedef typename shifter__<F, n, (n > max_digit)>::val val;
        };

        template<class F, int n> struct shifter_<F, n, 0>
        {
                typedef F val;
        };

        template<class F, int dir> struct shifter
        {


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
        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};


template<class A, class B> struct subtract
{
        typedef add<A, neg<B> > val;
        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};
template<class A_, class B_> struct multiply
{

        typedef normalize<A_> A;
        typedef normalize<B_> B;
        static const unsigned int A_u = (A::mant & top_bits) >> half_width;
        static const unsigned int B_u = (B::mant & top_bits) >> half_width;

        static const unsigned int A_l = (A::mant & bot_bits);
        static const unsigned int B_l = (B::mant & bot_bits);


        static const unsigned int uu = A_u * B_u;
        static const unsigned int m1 = A_u * B_l;
        static const unsigned int m2 = A_l * B_u;
        static const unsigned int ll = A_l * B_l;


        static const unsigned int res = uu + ((m1 + m2 + ll >> half_width) >> half_width);

        static const bool sign = !(A::posi ^ B::posi);

        typedef normalize<fp<res, A::expo + B::expo + half_width * 2, sign> > val;

        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};
template<int i, class C=int> struct intpow_error{}; template<class C> struct intpow_error<0, C>: public dummy<C> { typedef Nums::zero val; typedef typename dummy<C>::neagtive_power_in_intpow f; };

template<class F, int i> struct intpow: public intpow_error<F::posi>
{
        typedef typename multiply<F, typename intpow<F,i-1>::val >::val val;
        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};

template<class F> struct intpow<F, 0>
{
        typedef fp<1> val;
        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};





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
        static const bool val = ! gt<A,B>::val;
};

template<class A, class B> struct lt
{
        static const bool val = add<B, neg<A> >::posi;
};

template<class A, class B> struct gt_eq
{
        static const bool val = ! lt<A,B>::val;
};
template<class X, class A, int i> struct divide_newton
{
        typedef times_2<X> X_times_2;
        typedef multiply<X,X> X_squared;
        typedef multiply<X_squared, A> AX2;
        typedef neg<AX2> minus_AX2;
        typedef add<X_times_2, minus_AX2> next;

        typedef typename divide_newton<next, A, i-1>::val val;
};

template<class X, class A> struct divide_newton<X, A, 0>
{
        typedef X val;
};



template<class A_> struct approx_reciprocate
{



        typedef general_normalize<A_, max_digit/2> A_denorm;
        typedef normalize<fp<1> > one;


        static const unsigned int mantissa = one::mant / A_denorm::mant;

        typedef fp<mantissa, -A_denorm::expo - max_digit, A_denorm::posi> val;
};


template<class A_> struct reciprocate_
{
        typedef typename approx_reciprocate<A_>::val approx;
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
        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
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
        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};
template<class F> struct square
{
        typedef multiply<F, F> val;
        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};





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


template<class F> struct exponentiate__
{
        typedef typename exp_<fp<1>, F, fp<1>, fp<1>, 1>::val val;
};
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

        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};
template<class X, class A, int n> struct sqrt_
{
        typedef typename sqrt_<divide_2<add<X,divide<A,X> > >, A, n-1>::val val;
};


template<class X, class A> struct sqrt_<X, A, 0>
{
        typedef X val;
};




template<class F_> struct square_root_
{
        typedef normalize<F_> F;


        static const int n_2 = ((F::expo - -max_digit)/2);


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
        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};


template<int N, int D=0> struct fpd
{
        typedef fps<(N>=0?N:-N)> N_val;



        template<int n, int pow_digit, int digit> struct int_lg_10
        {
                static const int val = pow_digit > n ? digit: (int_lg_10<n, 10*pow_digit, digit+1>::val);
        };

        template<int n, int pow_digit> struct int_lg_10<n, pow_digit, max_decimal_digit>
        {
                static const int val = max_decimal_digit;
        };

        static const int D_digits = int_lg_10<D,1,0>::val;

        typedef divide<fps<D>, intpow<fp<10>, D_digits> > D_val;

        typedef add<N_val, D_val> foo;


        typedef set_sign<foo, (N>=0)> val;

        static const unsigned int mant = this::val::mant; static const int expo = this::val::expo; static const bool posi = this::val::posi;
};
template<class C> struct to_double
{





        static const int sign = C::posi?1:-1;


        static const unsigned int mantissa = C::mant;
        static const int uexpo= C::expo > 0 ? C::expo : -C::expo;
        static const int bad_expo = uexpo > 1023;


        static const int expo = bad_expo ? 0 : uexpo;



        static const int zero = (bad_expo && C::expo < 0) ? 0 : 1;
        static const int inf = (bad_expo && C::expo > 0) ? 0 : 1;

        static const int m_bit0 = 1 + ((expo & 0x1) && (C::expo > 0));
        static const int m_bit1 = 1 + ((expo & 0x2) && (C::expo > 0))*3;
        static const int m_bit2 = 1 + ((expo & 0x4) && (C::expo > 0))*15;
        static const int m_bit3 = 1 + ((expo & 0x8) && (C::expo > 0))*255;
        static const int m_bit4 = 1 + ((expo & 0x10) && (C::expo > 0))*65535;
        static const int m_bit5 = 1 + ((expo & 0x20) && (C::expo > 0))*65535;
        static const int m_bit6 = 1 + ((expo & 0x40) && (C::expo > 0))*65535;
        static const int m_bit7 = 1 + ((expo & 0x80) && (C::expo > 0))*65535;
        static const int m_bit8 = 1 + ((expo & 0x100) && (C::expo > 0))*65535;
        static const int m_bit9 = 1 + ((expo & 0x200) && (C::expo > 0))*65535;

        static const int d_bit0 = 1 + ((expo & 0x1) && (C::expo < 0));
        static const int d_bit1 = 1 + ((expo & 0x2) && (C::expo < 0))*3;
        static const int d_bit2 = 1 + ((expo & 0x4) && (C::expo < 0))*15;
        static const int d_bit3 = 1 + ((expo & 0x8) && (C::expo < 0))*255;
        static const int d_bit4 = 1 + ((expo & 0x10) && (C::expo < 0))*65535;
        static const int d_bit5 = 1 + ((expo & 0x20) && (C::expo < 0))*65535;
        static const int d_bit6 = 1 + ((expo & 0x40) && (C::expo < 0))*65535;
        static const int d_bit7 = 1 + ((expo & 0x80) && (C::expo < 0))*65535;
        static const int d_bit8 = 1 + ((expo & 0x100) && (C::expo < 0))*65535;
        static const int d_bit9 = 1 + ((expo & 0x200) && (C::expo < 0))*65535;
};
}

using namespace FPTemplate;
using namespace std;

int main()
{


        print<fp<5> >();


        print<fps<-1> >();




        print<fpd<-1,333333> >();
        print<fpd<0,333333> >();







        print<neg<fps<27> > >();
        print<f_abs<fps<-3> > >();
        print<fpd<-1,333333> >();




        print<add<fpd<-4>, fpd<1,5> > >();
        print<subtract<fpd<7>, fpd<1> > >();


        print<multiply<fp<3,-1>, fp<7> > >();


        print<intpow<fp<5>, 20> >();


        cout << eq<fp<5>, fp<2> >::val << endl;
        cout << gt<fp<5>, fp<2> >::val << endl;




        print<reciprocate<fp<3> > >();
        print<reciprocate<fp<8> > >();
        print<divide<fp<3>, fpd<0,333333333> > >();


        print<square_root<fp<2> > >();


        print<exponentiate<fp<1> > >();


        print<exponentiate<fp<65> > >();
        cout << ((float)((double)FPTemplate::to_double<fp<1,-6000> >::sign * FPTemplate::to_double<fp<1,-6000> >::mantissa * FPTemplate::to_double<fp<1,-6000> >::zero* (FPTemplate::to_double<fp<1,-6000> >::m_bit0) * (FPTemplate::to_double<fp<1,-6000> >::m_bit1) * (FPTemplate::to_double<fp<1,-6000> >::m_bit2) * (FPTemplate::to_double<fp<1,-6000> >::m_bit3) * (FPTemplate::to_double<fp<1,-6000> >::m_bit4) * (((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit5))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit5))) * (((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit6))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit6))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit6))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit6))))) * (((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit7))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit7))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit7))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit7))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit7))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit7))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit7))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit7))))))) * (((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit8))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit8))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit8))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit8))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit8))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit8))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit8))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit8))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit8))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit8))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit8))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit8))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit8))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit8))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit8))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit8))))))))) * (((double)(((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))))))))))*((double)(((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::m_bit9))))))))))) / (FPTemplate::to_double<fp<1,-6000> >::d_bit0) / (FPTemplate::to_double<fp<1,-6000> >::d_bit1) / (FPTemplate::to_double<fp<1,-6000> >::d_bit2) / (FPTemplate::to_double<fp<1,-6000> >::d_bit3) / (FPTemplate::to_double<fp<1,-6000> >::d_bit4) / (((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit5))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit5))) / (((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit6))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit6))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit6))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit6))))) / (((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit7))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit7))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit7))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit7))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit7))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit7))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit7))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit7))))))) / (((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit8))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit8))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit8))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit8))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit8))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit8))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit8))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit8))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit8))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit8))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit8))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit8))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit8))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit8))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit8))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit8))))))))) / (((double)(((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))))))))))*((double)(((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))*((double)(FPTemplate::to_double<fp<1,-6000> >::d_bit9))))))))))) / FPTemplate::to_double<fp<1,-6000> >::inf)) << endl;
        cout << ((float)((double)FPTemplate::to_double<Nums::e >::sign * FPTemplate::to_double<Nums::e >::mantissa * FPTemplate::to_double<Nums::e >::zero* (FPTemplate::to_double<Nums::e >::m_bit0) * (FPTemplate::to_double<Nums::e >::m_bit1) * (FPTemplate::to_double<Nums::e >::m_bit2) * (FPTemplate::to_double<Nums::e >::m_bit3) * (FPTemplate::to_double<Nums::e >::m_bit4) * (((double)(FPTemplate::to_double<Nums::e >::m_bit5))*((double)(FPTemplate::to_double<Nums::e >::m_bit5))) * (((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit6))*((double)(FPTemplate::to_double<Nums::e >::m_bit6))))*((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit6))*((double)(FPTemplate::to_double<Nums::e >::m_bit6))))) * (((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit7))*((double)(FPTemplate::to_double<Nums::e >::m_bit7))))*((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit7))*((double)(FPTemplate::to_double<Nums::e >::m_bit7))))))*((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit7))*((double)(FPTemplate::to_double<Nums::e >::m_bit7))))*((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit7))*((double)(FPTemplate::to_double<Nums::e >::m_bit7))))))) * (((double)(((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit8))*((double)(FPTemplate::to_double<Nums::e >::m_bit8))))*((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit8))*((double)(FPTemplate::to_double<Nums::e >::m_bit8))))))*((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit8))*((double)(FPTemplate::to_double<Nums::e >::m_bit8))))*((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit8))*((double)(FPTemplate::to_double<Nums::e >::m_bit8))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit8))*((double)(FPTemplate::to_double<Nums::e >::m_bit8))))*((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit8))*((double)(FPTemplate::to_double<Nums::e >::m_bit8))))))*((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit8))*((double)(FPTemplate::to_double<Nums::e >::m_bit8))))*((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit8))*((double)(FPTemplate::to_double<Nums::e >::m_bit8))))))))) * (((double)(((double)(((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit9))*((double)(FPTemplate::to_double<Nums::e >::m_bit9))))*((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit9))*((double)(FPTemplate::to_double<Nums::e >::m_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit9))*((double)(FPTemplate::to_double<Nums::e >::m_bit9))))*((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit9))*((double)(FPTemplate::to_double<Nums::e >::m_bit9))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit9))*((double)(FPTemplate::to_double<Nums::e >::m_bit9))))*((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit9))*((double)(FPTemplate::to_double<Nums::e >::m_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit9))*((double)(FPTemplate::to_double<Nums::e >::m_bit9))))*((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit9))*((double)(FPTemplate::to_double<Nums::e >::m_bit9))))))))))*((double)(((double)(((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit9))*((double)(FPTemplate::to_double<Nums::e >::m_bit9))))*((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit9))*((double)(FPTemplate::to_double<Nums::e >::m_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit9))*((double)(FPTemplate::to_double<Nums::e >::m_bit9))))*((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit9))*((double)(FPTemplate::to_double<Nums::e >::m_bit9))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit9))*((double)(FPTemplate::to_double<Nums::e >::m_bit9))))*((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit9))*((double)(FPTemplate::to_double<Nums::e >::m_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit9))*((double)(FPTemplate::to_double<Nums::e >::m_bit9))))*((double)(((double)(FPTemplate::to_double<Nums::e >::m_bit9))*((double)(FPTemplate::to_double<Nums::e >::m_bit9))))))))))) / (FPTemplate::to_double<Nums::e >::d_bit0) / (FPTemplate::to_double<Nums::e >::d_bit1) / (FPTemplate::to_double<Nums::e >::d_bit2) / (FPTemplate::to_double<Nums::e >::d_bit3) / (FPTemplate::to_double<Nums::e >::d_bit4) / (((double)(FPTemplate::to_double<Nums::e >::d_bit5))*((double)(FPTemplate::to_double<Nums::e >::d_bit5))) / (((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit6))*((double)(FPTemplate::to_double<Nums::e >::d_bit6))))*((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit6))*((double)(FPTemplate::to_double<Nums::e >::d_bit6))))) / (((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit7))*((double)(FPTemplate::to_double<Nums::e >::d_bit7))))*((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit7))*((double)(FPTemplate::to_double<Nums::e >::d_bit7))))))*((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit7))*((double)(FPTemplate::to_double<Nums::e >::d_bit7))))*((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit7))*((double)(FPTemplate::to_double<Nums::e >::d_bit7))))))) / (((double)(((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit8))*((double)(FPTemplate::to_double<Nums::e >::d_bit8))))*((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit8))*((double)(FPTemplate::to_double<Nums::e >::d_bit8))))))*((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit8))*((double)(FPTemplate::to_double<Nums::e >::d_bit8))))*((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit8))*((double)(FPTemplate::to_double<Nums::e >::d_bit8))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit8))*((double)(FPTemplate::to_double<Nums::e >::d_bit8))))*((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit8))*((double)(FPTemplate::to_double<Nums::e >::d_bit8))))))*((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit8))*((double)(FPTemplate::to_double<Nums::e >::d_bit8))))*((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit8))*((double)(FPTemplate::to_double<Nums::e >::d_bit8))))))))) / (((double)(((double)(((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit9))*((double)(FPTemplate::to_double<Nums::e >::d_bit9))))*((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit9))*((double)(FPTemplate::to_double<Nums::e >::d_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit9))*((double)(FPTemplate::to_double<Nums::e >::d_bit9))))*((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit9))*((double)(FPTemplate::to_double<Nums::e >::d_bit9))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit9))*((double)(FPTemplate::to_double<Nums::e >::d_bit9))))*((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit9))*((double)(FPTemplate::to_double<Nums::e >::d_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit9))*((double)(FPTemplate::to_double<Nums::e >::d_bit9))))*((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit9))*((double)(FPTemplate::to_double<Nums::e >::d_bit9))))))))))*((double)(((double)(((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit9))*((double)(FPTemplate::to_double<Nums::e >::d_bit9))))*((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit9))*((double)(FPTemplate::to_double<Nums::e >::d_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit9))*((double)(FPTemplate::to_double<Nums::e >::d_bit9))))*((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit9))*((double)(FPTemplate::to_double<Nums::e >::d_bit9))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit9))*((double)(FPTemplate::to_double<Nums::e >::d_bit9))))*((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit9))*((double)(FPTemplate::to_double<Nums::e >::d_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit9))*((double)(FPTemplate::to_double<Nums::e >::d_bit9))))*((double)(((double)(FPTemplate::to_double<Nums::e >::d_bit9))*((double)(FPTemplate::to_double<Nums::e >::d_bit9))))))))))) / FPTemplate::to_double<Nums::e >::inf)) << endl;
}

float f;
void foo()
{
        typedef fp<1,-1> half;
        f = ((double)FPTemplate::to_double<fp<1> >::sign * FPTemplate::to_double<fp<1> >::mantissa * FPTemplate::to_double<fp<1> >::zero* (FPTemplate::to_double<fp<1> >::m_bit0) * (FPTemplate::to_double<fp<1> >::m_bit1) * (FPTemplate::to_double<fp<1> >::m_bit2) * (FPTemplate::to_double<fp<1> >::m_bit3) * (FPTemplate::to_double<fp<1> >::m_bit4) * (((double)(FPTemplate::to_double<fp<1> >::m_bit5))*((double)(FPTemplate::to_double<fp<1> >::m_bit5))) * (((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit6))*((double)(FPTemplate::to_double<fp<1> >::m_bit6))))*((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit6))*((double)(FPTemplate::to_double<fp<1> >::m_bit6))))) * (((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit7))*((double)(FPTemplate::to_double<fp<1> >::m_bit7))))*((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit7))*((double)(FPTemplate::to_double<fp<1> >::m_bit7))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit7))*((double)(FPTemplate::to_double<fp<1> >::m_bit7))))*((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit7))*((double)(FPTemplate::to_double<fp<1> >::m_bit7))))))) * (((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit8))*((double)(FPTemplate::to_double<fp<1> >::m_bit8))))*((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit8))*((double)(FPTemplate::to_double<fp<1> >::m_bit8))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit8))*((double)(FPTemplate::to_double<fp<1> >::m_bit8))))*((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit8))*((double)(FPTemplate::to_double<fp<1> >::m_bit8))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit8))*((double)(FPTemplate::to_double<fp<1> >::m_bit8))))*((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit8))*((double)(FPTemplate::to_double<fp<1> >::m_bit8))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit8))*((double)(FPTemplate::to_double<fp<1> >::m_bit8))))*((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit8))*((double)(FPTemplate::to_double<fp<1> >::m_bit8))))))))) * (((double)(((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit9))*((double)(FPTemplate::to_double<fp<1> >::m_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit9))*((double)(FPTemplate::to_double<fp<1> >::m_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit9))*((double)(FPTemplate::to_double<fp<1> >::m_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit9))*((double)(FPTemplate::to_double<fp<1> >::m_bit9))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit9))*((double)(FPTemplate::to_double<fp<1> >::m_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit9))*((double)(FPTemplate::to_double<fp<1> >::m_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit9))*((double)(FPTemplate::to_double<fp<1> >::m_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit9))*((double)(FPTemplate::to_double<fp<1> >::m_bit9))))))))))*((double)(((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit9))*((double)(FPTemplate::to_double<fp<1> >::m_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit9))*((double)(FPTemplate::to_double<fp<1> >::m_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit9))*((double)(FPTemplate::to_double<fp<1> >::m_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit9))*((double)(FPTemplate::to_double<fp<1> >::m_bit9))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit9))*((double)(FPTemplate::to_double<fp<1> >::m_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit9))*((double)(FPTemplate::to_double<fp<1> >::m_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit9))*((double)(FPTemplate::to_double<fp<1> >::m_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1> >::m_bit9))*((double)(FPTemplate::to_double<fp<1> >::m_bit9))))))))))) / (FPTemplate::to_double<fp<1> >::d_bit0) / (FPTemplate::to_double<fp<1> >::d_bit1) / (FPTemplate::to_double<fp<1> >::d_bit2) / (FPTemplate::to_double<fp<1> >::d_bit3) / (FPTemplate::to_double<fp<1> >::d_bit4) / (((double)(FPTemplate::to_double<fp<1> >::d_bit5))*((double)(FPTemplate::to_double<fp<1> >::d_bit5))) / (((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit6))*((double)(FPTemplate::to_double<fp<1> >::d_bit6))))*((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit6))*((double)(FPTemplate::to_double<fp<1> >::d_bit6))))) / (((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit7))*((double)(FPTemplate::to_double<fp<1> >::d_bit7))))*((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit7))*((double)(FPTemplate::to_double<fp<1> >::d_bit7))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit7))*((double)(FPTemplate::to_double<fp<1> >::d_bit7))))*((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit7))*((double)(FPTemplate::to_double<fp<1> >::d_bit7))))))) / (((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit8))*((double)(FPTemplate::to_double<fp<1> >::d_bit8))))*((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit8))*((double)(FPTemplate::to_double<fp<1> >::d_bit8))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit8))*((double)(FPTemplate::to_double<fp<1> >::d_bit8))))*((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit8))*((double)(FPTemplate::to_double<fp<1> >::d_bit8))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit8))*((double)(FPTemplate::to_double<fp<1> >::d_bit8))))*((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit8))*((double)(FPTemplate::to_double<fp<1> >::d_bit8))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit8))*((double)(FPTemplate::to_double<fp<1> >::d_bit8))))*((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit8))*((double)(FPTemplate::to_double<fp<1> >::d_bit8))))))))) / (((double)(((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit9))*((double)(FPTemplate::to_double<fp<1> >::d_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit9))*((double)(FPTemplate::to_double<fp<1> >::d_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit9))*((double)(FPTemplate::to_double<fp<1> >::d_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit9))*((double)(FPTemplate::to_double<fp<1> >::d_bit9))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit9))*((double)(FPTemplate::to_double<fp<1> >::d_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit9))*((double)(FPTemplate::to_double<fp<1> >::d_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit9))*((double)(FPTemplate::to_double<fp<1> >::d_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit9))*((double)(FPTemplate::to_double<fp<1> >::d_bit9))))))))))*((double)(((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit9))*((double)(FPTemplate::to_double<fp<1> >::d_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit9))*((double)(FPTemplate::to_double<fp<1> >::d_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit9))*((double)(FPTemplate::to_double<fp<1> >::d_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit9))*((double)(FPTemplate::to_double<fp<1> >::d_bit9))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit9))*((double)(FPTemplate::to_double<fp<1> >::d_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit9))*((double)(FPTemplate::to_double<fp<1> >::d_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit9))*((double)(FPTemplate::to_double<fp<1> >::d_bit9))))*((double)(((double)(FPTemplate::to_double<fp<1> >::d_bit9))*((double)(FPTemplate::to_double<fp<1> >::d_bit9))))))))))) / FPTemplate::to_double<fp<1> >::inf);
        f = ((double)FPTemplate::to_double<half >::sign * FPTemplate::to_double<half >::mantissa * FPTemplate::to_double<half >::zero* (FPTemplate::to_double<half >::m_bit0) * (FPTemplate::to_double<half >::m_bit1) * (FPTemplate::to_double<half >::m_bit2) * (FPTemplate::to_double<half >::m_bit3) * (FPTemplate::to_double<half >::m_bit4) * (((double)(FPTemplate::to_double<half >::m_bit5))*((double)(FPTemplate::to_double<half >::m_bit5))) * (((double)(((double)(FPTemplate::to_double<half >::m_bit6))*((double)(FPTemplate::to_double<half >::m_bit6))))*((double)(((double)(FPTemplate::to_double<half >::m_bit6))*((double)(FPTemplate::to_double<half >::m_bit6))))) * (((double)(((double)(((double)(FPTemplate::to_double<half >::m_bit7))*((double)(FPTemplate::to_double<half >::m_bit7))))*((double)(((double)(FPTemplate::to_double<half >::m_bit7))*((double)(FPTemplate::to_double<half >::m_bit7))))))*((double)(((double)(((double)(FPTemplate::to_double<half >::m_bit7))*((double)(FPTemplate::to_double<half >::m_bit7))))*((double)(((double)(FPTemplate::to_double<half >::m_bit7))*((double)(FPTemplate::to_double<half >::m_bit7))))))) * (((double)(((double)(((double)(((double)(FPTemplate::to_double<half >::m_bit8))*((double)(FPTemplate::to_double<half >::m_bit8))))*((double)(((double)(FPTemplate::to_double<half >::m_bit8))*((double)(FPTemplate::to_double<half >::m_bit8))))))*((double)(((double)(((double)(FPTemplate::to_double<half >::m_bit8))*((double)(FPTemplate::to_double<half >::m_bit8))))*((double)(((double)(FPTemplate::to_double<half >::m_bit8))*((double)(FPTemplate::to_double<half >::m_bit8))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<half >::m_bit8))*((double)(FPTemplate::to_double<half >::m_bit8))))*((double)(((double)(FPTemplate::to_double<half >::m_bit8))*((double)(FPTemplate::to_double<half >::m_bit8))))))*((double)(((double)(((double)(FPTemplate::to_double<half >::m_bit8))*((double)(FPTemplate::to_double<half >::m_bit8))))*((double)(((double)(FPTemplate::to_double<half >::m_bit8))*((double)(FPTemplate::to_double<half >::m_bit8))))))))) * (((double)(((double)(((double)(((double)(((double)(FPTemplate::to_double<half >::m_bit9))*((double)(FPTemplate::to_double<half >::m_bit9))))*((double)(((double)(FPTemplate::to_double<half >::m_bit9))*((double)(FPTemplate::to_double<half >::m_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<half >::m_bit9))*((double)(FPTemplate::to_double<half >::m_bit9))))*((double)(((double)(FPTemplate::to_double<half >::m_bit9))*((double)(FPTemplate::to_double<half >::m_bit9))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<half >::m_bit9))*((double)(FPTemplate::to_double<half >::m_bit9))))*((double)(((double)(FPTemplate::to_double<half >::m_bit9))*((double)(FPTemplate::to_double<half >::m_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<half >::m_bit9))*((double)(FPTemplate::to_double<half >::m_bit9))))*((double)(((double)(FPTemplate::to_double<half >::m_bit9))*((double)(FPTemplate::to_double<half >::m_bit9))))))))))*((double)(((double)(((double)(((double)(((double)(FPTemplate::to_double<half >::m_bit9))*((double)(FPTemplate::to_double<half >::m_bit9))))*((double)(((double)(FPTemplate::to_double<half >::m_bit9))*((double)(FPTemplate::to_double<half >::m_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<half >::m_bit9))*((double)(FPTemplate::to_double<half >::m_bit9))))*((double)(((double)(FPTemplate::to_double<half >::m_bit9))*((double)(FPTemplate::to_double<half >::m_bit9))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<half >::m_bit9))*((double)(FPTemplate::to_double<half >::m_bit9))))*((double)(((double)(FPTemplate::to_double<half >::m_bit9))*((double)(FPTemplate::to_double<half >::m_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<half >::m_bit9))*((double)(FPTemplate::to_double<half >::m_bit9))))*((double)(((double)(FPTemplate::to_double<half >::m_bit9))*((double)(FPTemplate::to_double<half >::m_bit9))))))))))) / (FPTemplate::to_double<half >::d_bit0) / (FPTemplate::to_double<half >::d_bit1) / (FPTemplate::to_double<half >::d_bit2) / (FPTemplate::to_double<half >::d_bit3) / (FPTemplate::to_double<half >::d_bit4) / (((double)(FPTemplate::to_double<half >::d_bit5))*((double)(FPTemplate::to_double<half >::d_bit5))) / (((double)(((double)(FPTemplate::to_double<half >::d_bit6))*((double)(FPTemplate::to_double<half >::d_bit6))))*((double)(((double)(FPTemplate::to_double<half >::d_bit6))*((double)(FPTemplate::to_double<half >::d_bit6))))) / (((double)(((double)(((double)(FPTemplate::to_double<half >::d_bit7))*((double)(FPTemplate::to_double<half >::d_bit7))))*((double)(((double)(FPTemplate::to_double<half >::d_bit7))*((double)(FPTemplate::to_double<half >::d_bit7))))))*((double)(((double)(((double)(FPTemplate::to_double<half >::d_bit7))*((double)(FPTemplate::to_double<half >::d_bit7))))*((double)(((double)(FPTemplate::to_double<half >::d_bit7))*((double)(FPTemplate::to_double<half >::d_bit7))))))) / (((double)(((double)(((double)(((double)(FPTemplate::to_double<half >::d_bit8))*((double)(FPTemplate::to_double<half >::d_bit8))))*((double)(((double)(FPTemplate::to_double<half >::d_bit8))*((double)(FPTemplate::to_double<half >::d_bit8))))))*((double)(((double)(((double)(FPTemplate::to_double<half >::d_bit8))*((double)(FPTemplate::to_double<half >::d_bit8))))*((double)(((double)(FPTemplate::to_double<half >::d_bit8))*((double)(FPTemplate::to_double<half >::d_bit8))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<half >::d_bit8))*((double)(FPTemplate::to_double<half >::d_bit8))))*((double)(((double)(FPTemplate::to_double<half >::d_bit8))*((double)(FPTemplate::to_double<half >::d_bit8))))))*((double)(((double)(((double)(FPTemplate::to_double<half >::d_bit8))*((double)(FPTemplate::to_double<half >::d_bit8))))*((double)(((double)(FPTemplate::to_double<half >::d_bit8))*((double)(FPTemplate::to_double<half >::d_bit8))))))))) / (((double)(((double)(((double)(((double)(((double)(FPTemplate::to_double<half >::d_bit9))*((double)(FPTemplate::to_double<half >::d_bit9))))*((double)(((double)(FPTemplate::to_double<half >::d_bit9))*((double)(FPTemplate::to_double<half >::d_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<half >::d_bit9))*((double)(FPTemplate::to_double<half >::d_bit9))))*((double)(((double)(FPTemplate::to_double<half >::d_bit9))*((double)(FPTemplate::to_double<half >::d_bit9))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<half >::d_bit9))*((double)(FPTemplate::to_double<half >::d_bit9))))*((double)(((double)(FPTemplate::to_double<half >::d_bit9))*((double)(FPTemplate::to_double<half >::d_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<half >::d_bit9))*((double)(FPTemplate::to_double<half >::d_bit9))))*((double)(((double)(FPTemplate::to_double<half >::d_bit9))*((double)(FPTemplate::to_double<half >::d_bit9))))))))))*((double)(((double)(((double)(((double)(((double)(FPTemplate::to_double<half >::d_bit9))*((double)(FPTemplate::to_double<half >::d_bit9))))*((double)(((double)(FPTemplate::to_double<half >::d_bit9))*((double)(FPTemplate::to_double<half >::d_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<half >::d_bit9))*((double)(FPTemplate::to_double<half >::d_bit9))))*((double)(((double)(FPTemplate::to_double<half >::d_bit9))*((double)(FPTemplate::to_double<half >::d_bit9))))))))*((double)(((double)(((double)(((double)(FPTemplate::to_double<half >::d_bit9))*((double)(FPTemplate::to_double<half >::d_bit9))))*((double)(((double)(FPTemplate::to_double<half >::d_bit9))*((double)(FPTemplate::to_double<half >::d_bit9))))))*((double)(((double)(((double)(FPTemplate::to_double<half >::d_bit9))*((double)(FPTemplate::to_double<half >::d_bit9))))*((double)(((double)(FPTemplate::to_double<half >::d_bit9))*((double)(FPTemplate::to_double<half >::d_bit9))))))))))) / FPTemplate::to_double<half >::inf);

}
