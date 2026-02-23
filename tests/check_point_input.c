#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <mpfr.h>
#include <mpfi.h>
#include "../src/mpfi_agm.h"



void mpfr_mpfi_set_inf(mpfr_ptr mpfr_res, mpfi_ptr mpfi_res, const int sign)
{
    mpfr_set_inf(mpfr_res, sign);

    mpfi_set_fr(mpfi_res, mpfr_res);
}



void mpfr_mpfi_set_si(mpfr_ptr mpfr_res, mpfi_ptr mpfi_res, const long src, mpfr_rnd_t rnd)
{
    mpfr_set_si(mpfr_res, src, rnd);

    mpfi_set_fr(mpfi_res, mpfr_res);
}



void mpfr_mpfi_set_zero(mpfr_ptr mpfr_res, mpfi_ptr mpfi_res, const int sign)
{
    mpfr_set_zero(mpfr_res, sign);

    mpfi_set_fr(mpfi_res, mpfr_res);
}



int printf_res_mpfr_and_mpfi(mpfr_srcptr mpfr_x, mpfr_srcptr mpfr_y, mpfr_srcptr mpfr_res, mpfi_srcptr mpfi_x, mpfi_srcptr mpfi_y, mpfi_srcptr mpfi_res)
{
    printf("\n");

    mpfr_printf( "x          : %Rf [%Rf %Rf]\n", mpfr_x, &(mpfi_x-> left), &(mpfi_x-> right) );
    mpfr_printf( "y          : %Rf [%Rf %Rf]\n", mpfr_y, &(mpfi_y-> left), &(mpfi_y-> right) );

    printf("\n");

    mpfr_printf( "mpfi.right : %Re\n", &( mpfi_res -> right ) );
    mpfr_printf( "mpfr       : %Re\n",    mpfr_res            );
    mpfr_printf( "mpfi.left  : %Re\n", &( mpfi_res -> left  ) );

    printf("\n");

    return EXIT_SUCCESS;
}



void test_point_input_unit_half(mpfr_ptr mpfr_res, mpfr_srcptr mpfr_x, mpfr_srcptr mpfr_y, mpfi_ptr mpfi_res, mpfi_srcptr mpfi_x, mpfi_srcptr mpfi_y)
{
    mpfi_agm(mpfi_res, mpfi_x, mpfi_y);

    mpfr_agm(mpfr_res, mpfr_x, mpfr_y, MPFR_RNDN);

    if ( mpfr_nan_p(mpfr_res) )
    {
        assert( mpfr_nan_p( &( mpfi_res->left  )) || printf_res_mpfr_and_mpfi(mpfr_x, mpfr_y, mpfr_res, mpfi_x, mpfi_y, mpfi_res) );
        assert( mpfr_nan_p( &( mpfi_res->right )) || printf_res_mpfr_and_mpfi(mpfr_x, mpfr_y, mpfr_res, mpfi_x, mpfi_y, mpfi_res) );
    }
    else
    {
        assert( mpfr_lessequal_p( &(mpfi_res->left), mpfr_res  ) || printf_res_mpfr_and_mpfi(mpfr_x, mpfr_y, mpfr_res, mpfi_x, mpfi_y, mpfi_res) );
        assert( mpfr_lessequal_p( mpfr_res, &(mpfi_res->right) ) || printf_res_mpfr_and_mpfi(mpfr_x, mpfr_y, mpfr_res, mpfi_x, mpfi_y, mpfi_res) );
    }
}



void test_point_input_unit(mpfr_ptr mpfr_res, mpfr_srcptr mpfr_x, mpfr_srcptr mpfr_y, mpfi_ptr mpfi_res, mpfi_srcptr mpfi_x, mpfi_srcptr mpfi_y)
{
    test_point_input_unit_half(mpfr_res, mpfr_x, mpfr_y, mpfi_res, mpfi_x, mpfi_y);
    test_point_input_unit_half(mpfr_res, mpfr_y, mpfr_x, mpfi_res, mpfi_y, mpfi_x);
}



void test_point_input_per_prec(const mpfr_prec_t prec)
{
    mpfr_t mpfr_res, mpfr_x, mpfr_y;
    mpfi_t mpfi_res, mpfi_x, mpfi_y;



    mpfr_init2(mpfr_res , prec);
    mpfr_init2(mpfr_x   , prec);
    mpfr_init2(mpfr_y   , prec);

    mpfi_init2(mpfi_res , prec);
    mpfi_init2(mpfi_x   , prec);
    mpfi_init2(mpfi_y   , prec);



    for (int i =  1; i <= 5; i++)
    {
        mpfr_mpfi_set_si(mpfr_x, mpfi_x, i, MPFR_RNDN);



        for (int j =  i; j <= 5; j++)
        {
            mpfr_mpfi_set_si(mpfr_y, mpfi_y, j, MPFR_RNDN);

            if (i == j)
            {
                test_point_input_unit_half(mpfr_res, mpfr_x, mpfr_y, mpfi_res, mpfi_x, mpfi_y);
            }
            else
            {
                test_point_input_unit(mpfr_res, mpfr_x, mpfr_y, mpfi_res, mpfi_x, mpfi_y);
            }
        }



        mpfr_mpfi_set_inf(mpfr_y, mpfi_y, 1);

        test_point_input_unit(mpfr_res, mpfr_x, mpfr_y, mpfi_res, mpfi_x, mpfi_y);
    }



    mpfr_mpfi_set_inf(mpfr_x, mpfi_x, 1);

    mpfr_mpfi_set_inf(mpfr_y, mpfi_y, 1);

    test_point_input_unit_half(mpfr_res, mpfr_x, mpfr_y, mpfi_res, mpfi_x, mpfi_y);



    mpfr_mpfi_set_inf(mpfr_x, mpfi_x, -1);

    mpfr_mpfi_set_inf(mpfr_y, mpfi_y, 1);

    test_point_input_unit(mpfr_res, mpfr_x, mpfr_y, mpfi_res, mpfi_x, mpfi_y);



    mpfr_mpfi_set_zero(mpfr_x, mpfi_x, 1);

    mpfr_mpfi_set_inf(mpfr_y, mpfi_y, 1);

    test_point_input_unit(mpfr_res, mpfr_x, mpfr_y, mpfi_res, mpfi_x, mpfi_y);



    mpfr_clear( mpfr_res );
    mpfr_clear( mpfr_x   );
    mpfr_clear( mpfr_y   );

    mpfi_clear( mpfi_res );
    mpfi_clear( mpfi_x   );
    mpfi_clear( mpfi_y   );
}



int main(void)
{
    test_point_input_per_prec( 53);
    test_point_input_per_prec(113);
    test_point_input_per_prec(237);

    return EXIT_SUCCESS;
}
