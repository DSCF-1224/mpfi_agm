#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <mpfr.h>
#include <mpfi.h>
#include "../src/mpfi_agm.h"



void demo_steps(const mpfr_prec_t mpfi_prec, const unsigned long init_x, const unsigned long init_y)
{

    mpfi_t x, y;
    mpfi_t intersection_xy, union_xy, union_xy_prev;
    mpfi_t res_a, res_g;
    mpfi_t tmp_a, tmp_g;
    mpfr_t width;

    // Initialize all variables
    mpfi_init2( x               , mpfi_prec );
    mpfi_init2( y               , mpfi_prec );
    mpfi_init2( intersection_xy , mpfi_prec );
    mpfi_init2( union_xy        , mpfi_prec );
    mpfi_init2( union_xy_prev   , mpfi_prec );
    mpfi_init2( res_a           , mpfi_prec );
    mpfi_init2( res_g           , mpfi_prec );
    mpfi_init2( tmp_a           , mpfi_prec );
    mpfi_init2( tmp_g           , mpfi_prec );
    mpfr_init2( width           , mpfi_prec );

    // Initial values
    mpfi_set_ui(x, init_x);
    mpfi_set_ui(y, init_y);
    mpfi_union(union_xy_prev, x, y);

    // Print header
    printf("===================================================================================================\n");
    printf("iter interval 1               interval 2               union interval           width       overlap\n");
    printf("---------------------------------------------------------------------------------------------------\n");

    // Evaluate agm sequence
    for (mpfr_prec_t i = 0; i <= mpfi_prec; i++)
    {
        mpfi_intersect(intersection_xy, x, y);
        
        // Print iteration number and intervals
        mpfr_printf("%4ld ", i);
        mpfr_printf("[%7.4Re, %7.4Re] ", &y->left, &y->right);
        mpfr_printf("[%7.4Re, %7.4Re] ", &x->left, &x->right);
        
        if ( mpfi_is_empty(intersection_xy) )
        {
            printf("#disjoint!               #N/A        No\n");
        }
        else
        {
            mpfi_union(union_xy, x, y);
            mpfi_diam_abs(width, union_xy);
            
            mpfr_printf("[%7.4Re, %7.4Re] %7.4Re  YES", &union_xy->left, &union_xy->right, width);
            
            // Check convergence
            if ( mpfi_is_strictly_inside(union_xy, union_xy_prev) ) {
                printf(" (shrinking)");
            }

            printf("\n");

            mpfi_set(union_xy_prev, union_xy);
        }

        // AGM step
        mpfi_agm_step_tmp(res_a, res_g, tmp_a, tmp_g, x, y);
        mpfi_set(x, res_a);
        mpfi_set(y, res_g);
    }

    // Print footer
    printf("===================================================================================================\n\n");

    // Cleanup
    mpfi_clear( x               );
    mpfi_clear( y               );
    mpfi_clear( intersection_xy );
    mpfi_clear( union_xy        );
    mpfi_clear( union_xy_prev   );
    mpfi_clear( res_a           );
    mpfi_clear( res_g           );
    mpfi_clear( tmp_a           );
    mpfi_clear( tmp_g           );
    mpfr_clear( width           );
}

int main(void)
{
    for (unsigned long i = 1; i < 6; i++)
    {
        demo_steps(  53, 1UL, i );
        demo_steps( 113, 1UL, i );
        demo_steps( 237, 1UL, i );
    }

    return EXIT_SUCCESS;
}
