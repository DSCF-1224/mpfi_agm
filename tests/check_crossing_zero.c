#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
 
#include <mpfr.h>
#include <mpfi.h>
#include "../src/mpfi_agm.h"
#include "mpfi_agm_test_prec.h"
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



void test_crossing_zero_unit_half(mpfi_ptr res, mpfi_srcptr x, mpfi_srcptr y)
{
    mpfi_agm(res, x, y);

    assert( ( mpfr_nan_p( &(res->left) ) && mpfr_nan_p( &(res->right) ) ) || printf_res(res, x, y) );
}



void test_crossing_zero_unit(mpfi_ptr res, mpfi_srcptr x, mpfi_srcptr y)
{
    test_crossing_zero_unit_half(res, x, y);
    test_crossing_zero_unit_half(res, y, x);
}



void test_per_prec(const mpfr_prec_t prec)
{
    mpfi_t res, x, y;

    mpfi_init2(res , prec);
    mpfi_init2(x   , prec);
    mpfi_init2(y   , prec);



    for (long x_l = -5; x_l < 0; x_l++)
    for (long x_r =  1; x_r < 6; x_r++)
    {
        mpfi_interv_si(x, x_l, x_r); // [neg, pos]



        // [neg, pos] x non-zero

        for (long y_l = -5  ; y_l < 6; y_l++)
        for (long y_r = y_l ; y_r < 6; y_r++)
        {
            mpfi_interv_si(y, y_l, y_r);

            if ( !mpfi_is_zero(y) )
            {
                test_crossing_zero_unit_half(res, x, y);
            }
        }



        // [neg,  pos] x [ -0, +inf]
        // [ -0, +inf] x [neg,  pos]

        mpfr_set_zero ( &( y->left  ), -1 );
        mpfr_set_inf  ( &( y->right ),  1 );

        test_crossing_zero_unit(res, x, y);



        // [neg,  pos] x [ +0, +inf]
        // [ +0, +inf] x [neg,  pos]

        mpfr_set_zero ( &( y->left  ), 1 );
        mpfr_set_inf  ( &( y->right ), 1 );

        test_crossing_zero_unit(res, x, y);



        // [ neg,  pos] x [+inf, +inf]
        // [+inf, +inf] x [ neg,  pos]

        mpfr_set_inf( &( y->left  ), 1 );
        mpfr_set_inf( &( y->right ), 1 );

        test_crossing_zero_unit(res, x, y);



        // [ neg,  pos] x [-inf, -inf]
        // [-inf, -inf] x [ neg,  pos]

        mpfr_set_inf( &( y->left  ), -1 );
        mpfr_set_inf( &( y->right ), -1 );

        test_crossing_zero_unit(res, x, y);



        // [ neg,  pos] x [-inf, +inf]
        // [-inf, +inf] x [ neg,  pos]

        mpfr_set_inf( &( y->left  ), -1 );
        mpfr_set_inf( &( y->right ),  1 );

        test_crossing_zero_unit(res, x, y);
    }



    // [-inf, +inf] x [-inf, +inf]

    mpfr_set_inf( &( x->left  ), -1 );
    mpfr_set_inf( &( x->right ),  1 );

    test_crossing_zero_unit(res, x, x);



    mpfi_clear( res );
    mpfi_clear( x   );
    mpfi_clear( y   );
}



int main(void)
{
    for (int i = 0; i < TEST_PREC_LIST_LEN; i++)
    {
        test_per_prec(TEST_PREC_LIST[i]);
    }

    return EXIT_SUCCESS;
}
