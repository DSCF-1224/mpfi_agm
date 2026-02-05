#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <mpfr.h>
#include <mpfi.h>
#include "../src/mpfi_agm.h"

int main(void)
{
    const mpfr_prec_t mpfi_prec = 53;



    mpfi_t x, y, res_a, res_g;



    mpfi_init2(x, mpfi_prec);
    mpfi_init2(y, mpfi_prec);

    mpfi_init2(res_a, mpfi_prec);
    mpfi_init2(res_g, mpfi_prec);

    mpfi_init_set_ui(x, 1UL);
    mpfi_init_set_ui(y, 1UL);

    mpfi_agm_step(res_a, res_g, x, y);

    assert( mpfi_cmp_ui(res_a, 1UL) == 0 );
    assert( mpfi_cmp_ui(res_g, 1UL) == 0 );

    mpfi_clear(x);
    mpfi_clear(y);

    return EXIT_SUCCESS;
}
