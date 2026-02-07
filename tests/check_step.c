#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <mpfr.h>
#include <mpfi.h>
#include "../src/mpfi_agm.h"


void test_unit(const mpfr_prec_t mpfi_prec, const unsigned long input)
{
    mpfi_t x, res_a, res_g;



    mpfi_init2(x, mpfi_prec);

    mpfi_init2(res_a, mpfi_prec);
    mpfi_init2(res_g, mpfi_prec);

    mpfi_init_set_ui(x, input);

    mpfi_agm_step(res_a, res_g, x, x);

    assert( mpfi_cmp_ui(res_a, input) == 0 );
    assert( mpfi_cmp_ui(res_g, input) == 0 );

    mpfi_clear(x);
}

int main(void)
{
    test_unit(53, 1UL);

    return EXIT_SUCCESS;
}
