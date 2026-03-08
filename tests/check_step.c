#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <mpfr.h>
#include <mpfi.h>
#include "../src/mpfi_agm.h"
#include "mpfi_agm_test_prec.h"
#include "printf_interval.h"



int printf_res_diff_input(mpfr_srcptr mpfr_res, mpfi_srcptr mpfi_res, mpfi_srcptr op1, mpfi_srcptr op2, const char* name)
{
    char finalized_name[9];

    snprintf(finalized_name, sizeof(finalized_name), "mpfi.%s", name);

    printf("\n");

    printf_interval( "op1     "     , op1      );
    printf_interval( "op2     "     , op2      );
    printf_interval( finalized_name , mpfi_res );

    mpfr_printf("mpfr.%s :  %Rf\n", name, mpfr_res);

    printf("\n");

    return EXIT_SUCCESS;
}



int printf_res_same_input(mpfi_srcptr input, mpfi_srcptr res_a, mpfi_srcptr res_g)
{
    printf("\n");

    printf_interval("input", input);
    printf_interval("ari  ", res_a);
    printf_interval("geo  ", res_g);

    printf("\n");

    return EXIT_SUCCESS;
}



void test_diff_input_unit(mpfr_ptr mpfr_res_a, mpfr_ptr mpfr_res_g, mpfi_ptr mpfi_res_a, mpfi_ptr mpfi_res_g, mpfi_srcptr op1, mpfi_srcptr op2)
{
    mpfi_agm_step(mpfi_res_a, mpfi_res_g, op1, op2);



    // validation: arithmetic mean

    mpfr_add(mpfr_res_a, &(op1->right), &(op2->right), MPFR_RNDU);
    mpfr_div_ui(mpfr_res_a, mpfr_res_a, 2, MPFR_RNDU);
    assert( mpfi_is_inside_fr(mpfr_res_a, mpfi_res_a) || printf_res_diff_input(mpfr_res_a, mpfi_res_a, op1, op2, "ari") );



    // validation: geometric mean

    mpfr_mul(mpfr_res_g, &(op1->left), &(op2->left), MPFR_RNDD);
    mpfr_sqrt(mpfr_res_g, mpfr_res_g, MPFR_RNDD);
    assert( mpfi_is_inside_fr(mpfr_res_g, mpfi_res_g) || printf_res_diff_input(mpfr_res_g, mpfi_res_g, op1, op2, "geo") );
}



void test_same_input_unit(const mpfi_srcptr input)
{
    mpfi_t res_a, res_g;



    const mpfr_prec_t mpfi_prec = mpfi_get_prec(input);

    mpfi_init2(res_a, mpfi_prec);
    mpfi_init2(res_g, mpfi_prec);

    mpfi_agm_step(res_a, res_g, input, input);

    if ( mpfi_nan_p(input) )
    {
        assert( ( mpfi_nan_p(res_a) && mpfi_nan_p(res_g) ) || printf_res_same_input(input, res_a, res_g) );
    }
    else
    {
        assert( ( ( mpfi_cmp(res_a, input) == 0 ) && ( mpfi_cmp(res_g, input) == 0 ) ) || printf_res_same_input(input, res_a, res_g) );
    }

    mpfi_clear(res_a);
    mpfi_clear(res_g);
}



void test_same_input_unit_ui(const mpfr_prec_t mpfi_prec, const unsigned long input)
{
    mpfi_t x, res_a, res_g;



    mpfi_init2(x, mpfi_prec);

    mpfi_init2(res_a, mpfi_prec);
    mpfi_init2(res_g, mpfi_prec);

    mpfi_set_ui(x, input);

    mpfi_agm_step(res_a, res_g, x, x);

    assert( ( ( mpfi_cmp_ui(res_a, input) == 0 ) && ( mpfi_cmp_ui(res_g, input) == 0 ) ) || printf_res_same_input(x, res_a, res_g) );

    mpfi_clear( x     );
    mpfi_clear( res_a );
    mpfi_clear( res_g );
}



void test_same_input(const mpfr_prec_t mpfi_prec)
{
    mpfi_t input;

    mpfi_init2(input, mpfi_prec);



    for (unsigned long i = 0; i < 6; i++)
    {
        test_same_input_unit_ui(mpfi_prec, i);
    }



    for (int i = 0; i <= 10; i++)
    for (int j = 0; j <= 10; j++)
    {
        mpfi_interv_d(input, ((double)i)/10, ((double)j)/10);

        test_same_input_unit(input);
    }



    mpfr_set_zero ( &( input->left  ) , 1 );
    mpfr_set_inf  ( &( input->right ) , 1 );

    test_same_input_unit(input);



    mpfr_set_inf( &( input->left  ) , 1 );
    mpfr_set_inf( &( input->right ) , 1 );

    test_same_input_unit(input);



    mpfr_set_nan( &( input->left  ) );
    mpfr_set_nan( &( input->right ) );

    test_same_input_unit(input);



    mpfi_clear(input);
}



void test_per_prec(const mpfr_prec_t mpfi_prec)
{
    test_same_input(mpfi_prec);
}



int main(void)
{
    for (int i = 0; i < TEST_PREC_LIST_LEN; i++)
    {
        test_per_prec(TEST_PREC_LIST[i]);
    }

    return EXIT_SUCCESS;
}
