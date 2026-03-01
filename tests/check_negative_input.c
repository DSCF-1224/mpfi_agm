#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <mpfr.h>
#include <mpfi.h>
#include "../src/mpfi_agm.h"
#include "printf_interval.h"



int printf_res(mpfi_srcptr res, mpfi_srcptr x, mpfi_srcptr y)
{
    printf("\n");

    printf_interval( "x  ", x   );
    printf_interval( "y  ", y   );
    printf_interval( "agm", res );

    printf("\n");

    return EXIT_SUCCESS;
}



void test_negative_input_unit_half(mpfi_ptr res, mpfi_srcptr x, mpfi_srcptr y)
{
    mpfi_agm(res, x, y);

    assert( mpfr_nan_p( &( res->left  )) || printf_res(res, x, y) );
    assert( mpfr_nan_p( &( res->right )) || printf_res(res, x, y) );
}



void test_negative_input_unit(mpfi_ptr res, mpfi_srcptr x, mpfi_srcptr y)
{
    test_negative_input_unit_half(res, x, y);
    test_negative_input_unit_half(res, y, x);
}



void test_negative_input_per_prec(const mpfr_prec_t prec)
{
    mpfi_t res, x, y;



    mpfi_init2(res , prec);
    mpfi_init2(x   , prec);
    mpfi_init2(y   , prec);



    for (long x_l =  -5; x_l < 0; x_l++)
    for (long x_r = x_l; x_r < 0; x_r++)
    {
        mpfi_interv_si(x, x_l, x_r);



        for (long y_l =  -5; y_l < 0; y_l++)
        for (long y_r = y_l; y_r < 0; y_r++)
        {
            mpfi_interv_si(y, y_l, y_r);

            // [neg, neg] x [neg, neg]
            test_negative_input_unit(res, x, y);
        }



        for (long y_l =   1; y_l < 6; y_l++)
        for (long y_r = y_l; y_r < 6; y_r++)
        {
            mpfi_interv_si(y, y_l, y_r);

            // [neg, neg] x [pos, pos]
            // [pos, pos] x [neg, neg]
            test_negative_input_unit(res, x, y);
        }



        mpfr_set_inf( &( y->left  ), -1 );
        mpfr_set_inf( &( y->right ), -1 );

        // [ neg,  neg] x [-inf, -inf]
        // [-inf, -inf] x [ neg,  neg]
        test_negative_input_unit(res, x, y);



        mpfr_set_inf  ( &( y->left  ), -1 );
        mpfr_set_zero ( &( y->right ), -1 );

        // [ neg, neg] x [-inf,  -0]
        // [-inf,  -0] x [ neg, neg]
        test_negative_input_unit(res, x, y);



        mpfr_set_inf  ( &( y->left  ), -1 );
        mpfr_set_zero ( &( y->right ),  1 );

        // [ neg, neg] x [-inf,  +0]
        // [-inf,  +0] x [ neg, neg]
        test_negative_input_unit(res, x, y);



        mpfr_set_zero ( &( y->left  ), -1 );
        mpfr_set_inf  ( &( y->right ),  1 );

        // [neg,  neg] x [ -0, +inf]
        // [ -0, +inf] x [neg,  neg]
        test_negative_input_unit(res, x, y);



        mpfr_set_zero ( &( y->left  ), 1 );
        mpfr_set_inf  ( &( y->right ), 1 );

        // [neg,  neg] x [ +0, +inf]
        // [ +0, +inf] x [neg,  neg]
        test_negative_input_unit(res, x, y);



        mpfr_set_inf( &( y->left  ), 1 );
        mpfr_set_inf( &( y->right ), 1 );

        // [ neg,  neg] x [+inf, +inf]
        // [+inf, +inf] x [ neg,  neg]
        test_negative_input_unit(res, x, y);
    }



    mpfi_clear( res );
    mpfi_clear( x   );
    mpfi_clear( y   );
}



int main(void)
{
    test_negative_input_per_prec( 53);
    test_negative_input_per_prec(113);
    test_negative_input_per_prec(237);

    return EXIT_SUCCESS;
}
