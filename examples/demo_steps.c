#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <mpfr.h>
#include <mpfi.h>
#include "../src/mpfi_agm.h"
#include "../tests/mpfi_agm_test_prec.h"



void demo_steps(const mpfr_prec_t mpfi_prec, const unsigned long init_x, const unsigned long init_y)
{
    mpfr_t wid_x, wid_y, wid_intersection, wid_union;

    mpfi_t x, y;
    mpfi_t intersection_xy, union_xy, union_xy_prev;
    mpfi_t res_a, res_g;
    mpfi_t tmp_a, tmp_g;

    // Initialize all variables
    mpfi_init2( x                , mpfi_prec );
    mpfi_init2( y                , mpfi_prec );
    mpfi_init2( intersection_xy  , mpfi_prec );
    mpfi_init2( union_xy         , mpfi_prec );
    mpfi_init2( union_xy_prev    , mpfi_prec );
    mpfi_init2( res_a            , mpfi_prec );
    mpfi_init2( res_g            , mpfi_prec );
    mpfi_init2( tmp_a            , mpfi_prec );
    mpfi_init2( tmp_g            , mpfi_prec );
    mpfr_init2( wid_x            , mpfi_prec );
    mpfr_init2( wid_y            , mpfi_prec );
    mpfr_init2( wid_intersection , mpfi_prec );
    mpfr_init2( wid_union        , mpfi_prec );

    // Initial values
    mpfi_set_ui(x, init_x);
    mpfi_set_ui(y, init_y);



    for (mpfr_prec_t i = 0; i <= mpfi_prec; i++)
    {
        mpfi_intersect (intersection_xy , x, y);
        mpfi_union     (union_xy        , x, y);

        if ( mpfi_is_empty(intersection_xy) )
        {
            mpfr_set_nan( &( intersection_xy->left  ) );
            mpfr_set_nan( &( intersection_xy->right ) );
        }



        mpfi_diam_abs( wid_x            ,              x  );
        mpfi_diam_abs( wid_y            ,               y );
        mpfi_diam_abs( wid_intersection , intersection_xy );
        mpfi_diam_abs( wid_union        , union_xy        );



        printf("STEP.%ld\n", i);

        mpfr_printf( "x.right  %Re\n", &( x->right         ) );
        mpfr_printf( "x.left   %Re\n", &( x->left          ) );
        mpfr_printf( "y.right  %Re\n", &( y->right         ) );
        mpfr_printf( "y.left   %Re\n", &( y->left          ) );

        printf("\n");

        mpfr_printf( "u.right  %Re\n", &( union_xy        ->right ) );
        mpfr_printf( "i.right  %Re\n", &( intersection_xy ->right ) );
        mpfr_printf( "i.left   %Re\n", &( intersection_xy ->left  ) );
        mpfr_printf( "u.left   %Re\n", &( union_xy        ->left  ) );

        printf("\n");

        mpfr_printf( "x.width  %Re\n" , &( wid_x            ) );
        mpfr_printf( "y.width  %Re\n" , &( wid_y            ) );
        mpfr_printf( "i.width  %Re\n" , &( wid_intersection ) );
        mpfr_printf( "u.width  %Re\n" , &( wid_union        ) );

        printf("u.inside ");

        if ( mpfi_nan_p(union_xy_prev) )
        {
            printf("#N/A");
        }
        else if ( mpfi_is_strictly_inside(union_xy, union_xy_prev) )
        {
            printf("true");
        }
        else
        {
            printf("false\n\n"); break;
        }

        printf("\n\n");


        mpfi_agm_step_tmp(res_a, res_g, tmp_a, tmp_g, x, y);

        mpfi_set(      x       , res_a    );
        mpfi_set(       y      , res_g    );
        mpfi_set(union_xy_prev , union_xy );
    }

    // Cleanup
    mpfi_clear( x                );
    mpfi_clear( y                );
    mpfi_clear( intersection_xy  );
    mpfi_clear( union_xy         );
    mpfi_clear( union_xy_prev    );
    mpfi_clear( res_a            );
    mpfi_clear( res_g            );
    mpfi_clear( tmp_a            );
    mpfi_clear( tmp_g            );
    mpfr_clear( wid_x            );
    mpfr_clear( wid_y            );
    mpfr_clear( wid_intersection );
    mpfr_clear( wid_union        );
}

int main(void)
{
    for (int i = 0; i < TEST_PREC_LIST_LEN; i++)
    for (unsigned long j = 1; j < 6; j++)
    {
        demo_steps(TEST_PREC_LIST[i], j, 1UL);
    }

    return EXIT_SUCCESS;
}
