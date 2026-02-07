#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <mpfr.h>
#include <mpfi.h>
#include "../src/mpfi_agm.h"



void test_same_input_case_ui(const mpfr_prec_t mpfi_prec, const unsigned long input)
{
    mpfi_t x, res_a, res_g;



    mpfi_init2(x, mpfi_prec);

    mpfi_init2(res_a, mpfi_prec);
    mpfi_init2(res_g, mpfi_prec);

    mpfi_set_ui(x, input);

    mpfi_agm_step(res_a, res_g, x, x);

    assert( mpfi_cmp_ui(res_a, input) == 0 );
    assert( mpfi_cmp_ui(res_g, input) == 0 );

    mpfi_clear(x);
    mpfi_clear(res_a);
    mpfi_clear(res_g);
}



void test_same_input(const mpfr_prec_t mpfi_prec)
{
    for (unsigned long i = 0; i < 6; i++)
    {
        test_same_input_case_ui(mpfi_prec, i);
    }
}



void test_per_prec(const mpfr_prec_t mpfi_prec)
{
    test_same_input(mpfi_prec);
}



int main(void)
{
    test_per_prec( 53);
    test_per_prec(113);
    test_per_prec(237);

    return EXIT_SUCCESS;
}
