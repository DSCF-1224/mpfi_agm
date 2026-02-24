#include "mpfi_agm.h"



void mpfi_agm(mpfi_ptr res, mpfi_srcptr op1, mpfi_srcptr op2)
{
    const mpfr_prec_t mpfi_prec = mpfi_get_prec(res);

    bool is_converged = false;

    mpfi_t intersection_ag, res_a, res_a_prev, res_g, res_g_prev, tmp_a, tmp_g;

    mpfi_init2( intersection_ag , mpfi_prec );
    mpfi_init2( res_a           , mpfi_prec );
    mpfi_init2( res_a_prev      , mpfi_prec );
    mpfi_init2( res_g           , mpfi_prec );
    mpfi_init2( res_g_prev      , mpfi_prec );
    mpfi_init2( tmp_a           , mpfi_prec );
    mpfi_init2( tmp_g           , mpfi_prec );

    mpfi_set(res_a_prev, op1);
    mpfi_set(res_g_prev, op2);

    for (mpfr_prec_t i = 0; i < mpfi_prec; i++)
    {
        mpfi_agm_step_tmp(res_a, res_g, tmp_a, tmp_g, res_a_prev, res_g_prev);

        mpfi_intersect(intersection_ag, res_a, res_g);

        is_converged = !mpfi_is_empty(intersection_ag);

        if (is_converged) break;

        mpfi_set(res_a_prev, res_a);
        mpfi_set(res_g_prev, res_g);
    }

    if (is_converged)
    {
        mpfi_union(res, res_a, res_g);
    }
    else
    {
        mpfr_set_nan( &( res->left  ) );
        mpfr_set_nan( &( res->right ) );
    }

    mpfi_clear( intersection_ag );
    mpfi_clear( res_a           );
    mpfi_clear( res_a_prev      );
    mpfi_clear( res_g           );
    mpfi_clear( res_g_prev      );
    mpfi_clear( tmp_a           );
    mpfi_clear( tmp_g           );
}



void mpfi_agm_step(mpfi_ptr res_a, mpfi_ptr res_g, mpfi_srcptr op1, mpfi_srcptr op2)
{
    mpfi_t tmp_a, tmp_g;

    mpfi_init2(tmp_a, mpfi_get_prec(res_a));
    mpfi_init2(tmp_g, mpfi_get_prec(res_g));

    mpfi_agm_step_tmp(res_a, res_g, tmp_a, tmp_g, op1, op2);

    mpfi_clear(tmp_a);
    mpfi_clear(tmp_g);
}



void mpfi_agm_step_tmp(mpfi_ptr res_a, mpfi_ptr res_g, mpfi_ptr tmp_a, mpfi_ptr tmp_g, mpfi_srcptr op1, mpfi_srcptr op2)
{
    /* STEP.1 compute both intermediates */

    mpfr_add(&( tmp_a->left  ), &( op1->left  ), &( op2->left  ), MPFR_RNDD);
    mpfr_add(&( tmp_a->right ), &( op1->right ), &( op2->right ), MPFR_RNDU);

    mpfr_mul(&( tmp_g->left  ), &( op1->left  ), &( op2->left  ), MPFR_RNDD);
    mpfr_mul(&( tmp_g->right ), &( op1->right ), &( op2->right ), MPFR_RNDU);



    /* STEP.2.1 compute the arithmetic mean */

    mpfr_div_ui(&( res_a->left  ), &( tmp_a->left  ), 2UL, MPFR_RNDD);
    mpfr_div_ui(&( res_a->right ), &( tmp_a->right ), 2UL, MPFR_RNDU);



    /* STEP.2.2 compute the geometric mean mean */

    mpfr_sqrt(&( res_g->left  ), &( tmp_g->left  ), MPFR_RNDD);
    mpfr_sqrt(&( res_g->right ), &( tmp_g->right ), MPFR_RNDU);
}
