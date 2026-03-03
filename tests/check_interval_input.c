#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <mpfr.h>
#include <mpfi.h>
#include "../src/mpfi_agm.h"
#include "mpfi_agm_test_prec.h"
#include "printf_interval.h"



int printf_res(mpfr_srcptr mpfr_res, mpfi_srcptr mpfi_res, mpfi_srcptr x, mpfi_srcptr y)
{
    printf("\n");

    printf_interval( "mpfi.x  ", x        );
    printf_interval( "mpfi.y  ", y        );
    printf_interval( "mpfi.agm", mpfi_res );

    mpfr_printf("mpfr.agm :  %Rf\n", mpfr_res);

    printf("\n");

    return EXIT_SUCCESS;
}



void test_interval_input_assertion(mpfr_srcptr mpfr_res, mpfi_srcptr mpfi_res, mpfi_srcptr x, mpfi_srcptr y, const bool flag_nan)
{
    if (flag_nan)
    {
        assert( mpfr_nan_p( &( mpfi_res->left  ) ) || printf_res(mpfr_res, mpfi_res, x, y) );
        assert( mpfr_nan_p( &( mpfi_res->right ) ) || printf_res(mpfr_res, mpfi_res, x, y) );
    }
    else
    {
        assert( mpfi_is_inside_fr(mpfr_res, mpfi_res) || printf_res(mpfr_res, mpfi_res, x, y) );
    }
}



void test_interval_input_unit_half(mpfr_ptr mpfr_res, mpfi_ptr mpfi_res, mpfi_srcptr x, mpfi_srcptr y)
{
    mpfi_agm(mpfi_res, x, y);



    mpfr_agm(mpfr_res, &(x->left), &(y->left), MPFR_RNDD);

    const bool flag_nan = mpfr_nan_p(mpfr_res);

    test_interval_input_assertion(mpfr_res, mpfi_res, x, y, flag_nan);



    mpfr_agm(mpfr_res, &(x->right), &(y->right), MPFR_RNDU);

    test_interval_input_assertion(mpfr_res, mpfi_res, x, y, flag_nan);
}



void test_interval_input_unit(mpfr_ptr mpfr_res, mpfi_ptr mpfi_res, mpfi_srcptr x, mpfi_srcptr y)
{
    test_interval_input_unit_half(mpfr_res, mpfi_res, x, y);
    test_interval_input_unit_half(mpfr_res, mpfi_res, y, x);
}



void test_interval_input_per_prec(const mpfr_prec_t prec)
{
    mpfr_t mpfr_res;
    mpfi_t mpfi_res, x, y;



    mpfr_init2(mpfr_res, prec);

    mpfi_init2(mpfi_res , prec);
    mpfi_init2(x        , prec);
    mpfi_init2(y        , prec);



    for (long x_l =        0  ; x_l < 6; x_l++)
    for (long x_r = (x_l + 1) ; x_r < 6; x_r++)
    {
        mpfi_interv_si(x, x_l, x_r);



        for (long y_l =        0  ; y_l < 6; y_l++)
        for (long y_r = (y_l + 1) ; y_r < 6; y_r++)
        {
            mpfi_interv_si(y, y_l, y_r);

            test_interval_input_unit_half(mpfr_res, mpfi_res, x, y);
        }



        // [ -0, +inf] x [pos,  pos]
        // [pos,  pos] x [ -0, +inf]

        mpfr_set_zero ( &( y->left  ), -1 );
        mpfr_set_inf  ( &( y->right ),  1 );

        test_interval_input_unit(mpfr_res, mpfi_res, x, y);



        // [ +0, +inf] x [pos,  pos]
        // [pos,  pos] x [ +0, +inf]

        mpfr_set_zero ( &( y->left  ), 1 );
        mpfr_set_inf  ( &( y->right ), 1 );

        test_interval_input_unit(mpfr_res, mpfi_res, x, y);



        // [+inf, +inf] x [ pos,  pos]
        // [ pos,  pos] x [+inf, +inf]

        mpfr_set_inf( &( y->left  ), 1 );
        mpfr_set_inf( &( y->right ), 1 );

        test_interval_input_unit(mpfr_res, mpfi_res, x, y);
    }



    // [-0, +inf] x [-0, +inf]

    mpfr_set_zero ( &( x->left  ), -1 );
    mpfr_set_inf  ( &( x->right ),  1 );

    test_interval_input_unit_half(mpfr_res, mpfi_res, x, x);



    // [+0, +inf] x [+0, +inf]

    mpfr_set_zero ( &( x->left  ), 1 );
    mpfr_set_inf  ( &( x->right ), 1 );

    test_interval_input_unit_half(mpfr_res, mpfi_res, x, x);



    // [+inf, +inf] x [+inf, +inf]

    mpfr_set_inf( &( x->left  ), 1 );
    mpfr_set_inf( &( x->right ), 1 );

    test_interval_input_unit_half(mpfr_res, mpfi_res, x, x);



    mpfr_clear( mpfr_res );

    mpfi_clear( mpfi_res );
    mpfi_clear( x        );
    mpfi_clear( y        );
}



int main(void)
{
    for (int i = 0; i < TEST_PREC_LIST_LEN; i++)
    {
        test_interval_input_per_prec(TEST_PREC_LIST[i]);
    }

    return EXIT_SUCCESS;
}
