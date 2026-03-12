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



void test_diff_input_arithmetic(mpfr_ptr res, mpfr_srcptr op1, mpfr_srcptr op2, mpfr_rnd_t rnd)
{
    mpfr_add(res, op1, op2, rnd);
    mpfr_div_ui(res, res, 2, rnd);
}



void test_diff_input_geometric(mpfr_ptr res, mpfr_srcptr op1, mpfr_srcptr op2, mpfr_rnd_t rnd)
{
    mpfr_mul(res, op1, op2, rnd);
    mpfr_sqrt(res, res, rnd);
}



void test_diff_input_assertion(mpfr_srcptr mpfr_res, mpfi_srcptr mpfi_res, mpfi_srcptr op1, mpfi_srcptr op2, const char *name)
{
    assert( mpfi_is_inside_fr(mpfr_res, mpfi_res) || printf_res_diff_input(mpfr_res, mpfi_res, op1, op2, name) );
}



void test_diff_input_unit_half(mpfr_ptr mpfr_res_a, mpfr_ptr mpfr_res_g, mpfi_ptr mpfi_res_a, mpfi_ptr mpfi_res_g, mpfi_srcptr op1, mpfi_srcptr op2)
{
    mpfi_agm_step(mpfi_res_a, mpfi_res_g, op1, op2);



    // validation: arithmetic mean

    test_diff_input_arithmetic(mpfr_res_a, &(op1->left), &(op2->left), MPFR_RNDD);
    test_diff_input_assertion(mpfr_res_a, mpfi_res_a, op1, op2, "ari");

    test_diff_input_arithmetic(mpfr_res_a, &(op1->right), &(op2->right), MPFR_RNDU);
    test_diff_input_assertion(mpfr_res_a, mpfi_res_a, op1, op2, "ari");



    // validation: geometric mean

    test_diff_input_geometric(mpfr_res_g, &(op1->left), &(op2->left), MPFR_RNDD);
    test_diff_input_assertion(mpfr_res_g, mpfi_res_g, op1, op2, "geo");

    test_diff_input_geometric(mpfr_res_g, &(op1->right), &(op2->right), MPFR_RNDU);
    test_diff_input_assertion(mpfr_res_g, mpfi_res_g, op1, op2, "geo");
}



void test_diff_input_unit(mpfr_ptr mpfr_res_a, mpfr_ptr mpfr_res_g, mpfi_ptr mpfi_res_a, mpfi_ptr mpfi_res_g, mpfi_srcptr op1, mpfi_srcptr op2)
{
    test_diff_input_unit_half(mpfr_res_a, mpfr_res_g, mpfi_res_a, mpfi_res_g, op1, op2);
    test_diff_input_unit_half(mpfr_res_a, mpfr_res_g, mpfi_res_a, mpfi_res_g, op2, op1);
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



void test_diff_input(const mpfr_prec_t mpfi_prec)
{
    mpfr_t mpfr_res_a, mpfr_res_g;
    mpfi_t mpfi_res_a, mpfi_res_g, x, y;



    mpfr_init2( mpfr_res_a , mpfi_prec );
    mpfr_init2( mpfr_res_g , mpfi_prec );

    mpfi_init2( mpfi_res_a , mpfi_prec );
    mpfi_init2( mpfi_res_g , mpfi_prec );
    mpfi_init2( x          , mpfi_prec );
    mpfi_init2( y          , mpfi_prec );



    for (unsigned long x_l = 0   ; x_l <= 5; x_l++)
    for (unsigned long x_r = x_l ; x_r <= 5; x_r++)
    {
        mpfi_interv_ui(x, x_l, x_r);



        for (unsigned long y_l = 0   ; y_l <= 5; y_l++)
        for (unsigned long y_r = y_l ; y_r <= 5; y_r++)
        {
            mpfi_interv_ui(y, y_l, y_r);

            if ( (x_l != y_l) || (x_r != y_r) )
            {
                test_diff_input_unit_half(mpfr_res_a, mpfr_res_g, mpfi_res_a, mpfi_res_g, x, y);
            }
        }



        if (x_l > 0)
        {
            mpfr_set_zero ( &( y->left  ) , 1 );
            mpfr_set_inf  ( &( y->right ) , 1 );

            test_diff_input_unit(mpfr_res_a, mpfr_res_g, mpfi_res_a, mpfi_res_g, x, y);



            mpfr_set_inf( &( y->left  ), 1 );
            mpfr_set_inf( &( y->right ), 1 );

            test_diff_input_unit(mpfr_res_a, mpfr_res_g, mpfi_res_a, mpfi_res_g, x, y);
        }
    }



    for (unsigned long x_l = 0   ; x_l <= 10; x_l++)
    for (unsigned long x_r = x_l ; x_r <= 10; x_r++)
    {
        mpfi_interv_d( x, ((double)x_l)/10, ((double)x_r)/10 );



        for (unsigned long y_l = 0   ; y_l <= 10; y_l++)
        for (unsigned long y_r = y_l ; y_r <= 10; y_r++)
        {
            mpfi_interv_d( y, ((double)y_l)/10, ((double)y_r)/10 );

            if ( (x_l != y_l) || (x_r != y_r) )
            {
                test_diff_input_unit_half(mpfr_res_a, mpfr_res_g, mpfi_res_a, mpfi_res_g, x, y);
            }
        }



        if (x_l > 0)
        {
            mpfr_set_zero ( &( y->left  ) , 1 );
            mpfr_set_inf  ( &( y->right ) , 1 );

            test_diff_input_unit(mpfr_res_a, mpfr_res_g, mpfi_res_a, mpfi_res_g, x, y);



            mpfr_set_inf( &( y->left  ), 1 );
            mpfr_set_inf( &( y->right ), 1 );

            test_diff_input_unit(mpfr_res_a, mpfr_res_g, mpfi_res_a, mpfi_res_g, x, y);
        }
    }



    mpfr_clear( mpfr_res_a );
    mpfr_clear( mpfr_res_g );

    mpfi_clear( mpfi_res_a );
    mpfi_clear( mpfi_res_g );
    mpfi_clear( x          );
    mpfi_clear( y          );
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
    test_diff_input(mpfi_prec);
}



int main(void)
{
    for (int i = 0; i < TEST_PREC_LIST_LEN; i++)
    {
        test_per_prec(TEST_PREC_LIST[i]);
    }

    return EXIT_SUCCESS;
}
