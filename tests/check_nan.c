#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <mpfr.h>
#include <mpfi.h>
#include "../src/mpfi_agm.h"



void test_nan_unit_half(mpfi_ptr res, mpfi_srcptr x, mpfi_srcptr nan)
{
    mpfi_agm(res, x, nan);

    assert( mpfr_nan_p( &( res->left  ) ) );
    assert( mpfr_nan_p( &( res->right ) ) );
}



void test_nan_unit(mpfi_ptr res, mpfi_srcptr x, mpfi_srcptr nan)
{
    test_nan_unit_half( res, x   , nan );
    test_nan_unit_half( res, nan , x   );
}



void test_nan_per_prec(const mpfr_prec_t prec)
{
    mpfi_t nan, res, x;



    mpfi_init2(nan, prec);

    mpfr_set_nan( &( nan->left  ) );
    mpfr_set_nan( &( nan->right ) );



    mpfi_init2( x   , prec );
    mpfi_init2( res , prec );



    mpfr_set_inf( &( x->left  ), -1 );
    mpfr_set_inf( &( x->right ),  1 );

    test_nan_unit(res, x, nan);



    for (long i = -5; i <= 5; i++)
    for (long j = -5; j <= 5; j++)
    {
        mpfi_interv_si(x, i, j); test_nan_unit(res, x, nan);
    }



    test_nan_unit_half(res, nan, nan);



    mpfi_clear( nan );
    mpfi_clear( res );
    mpfi_clear( x   );
}



int main(void)
{
    test_nan_per_prec( 53);
    test_nan_per_prec(113);
    test_nan_per_prec(237);

    return EXIT_SUCCESS;
}
