#ifndef MPFI_AGM_H
#define MPFI_AGM_H

#include <mpfi.h>



static void mpfi_agm_step_tmp(mpfi_ptr res_a, mpfi_ptr res_g, mpfi_ptr tmp_a, mpfi_ptr tmp_g, mpfi_srcptr op1, mpfi_srcptr op2)
{
    mpfi_add(tmp_a, op1, op2); mpfi_div_ui(res_a, tmp_a, 2UL);

    mpfi_mul(tmp_g, op1, op2); mpfi_sqrt(res_g, tmp_g);
}



static void mpfi_agm(mpfi_ptr res, mpfi_srcptr op1, mpfi_srcptr op2)
{
    const mpfr_prec_t mpfi_prec = mpfi_get_prec(res);

    mpfi_intersect(res, op1, op2);

    if ( !mpfi_is_empty(res) )
    {
        mpfi_union(res, op1, op2); return;
    }

    mpfi_t intersection_ag, res_a, res_a_prev, res_g, res_g_prev, tmp_a, tmp_g, union_ag;

    mpfi_init_set(intersection_ag, res);

    mpfi_init2( res_a      , mpfi_prec );
    mpfi_init2( res_a_prev , mpfi_prec ); mpfi_set(res_a_prev, op1);
    mpfi_init2( res_g      , mpfi_prec );
    mpfi_init2( res_g_prev , mpfi_prec ); mpfi_set(res_g_prev, op2);
    mpfi_init2( tmp_a      , mpfi_prec );
    mpfi_init2( tmp_g      , mpfi_prec );
    mpfi_init2( union_ag   , mpfi_prec );

    for (mpfr_prec_t i = 0; i < mpfi_prec; i++)
    {
        mpfi_agm_step_tmp(res_a, res_g, tmp_a, tmp_g, res_a_prev, res_g_prev);

        mpfi_intersect(intersection_ag, res_a, res_g);

        if ( mpfi_is_empty(intersection_ag) )
        {
            mpfi_set(res_a_prev, res_a);
            mpfi_set(res_g_prev, res_g);

            continue;
        }

        break;
    }

    mpfi_union(res, res_a, res_g);

    mpfi_clear( intersection_ag );
    mpfi_clear( res_a           );
    mpfi_clear( res_a_prev      );
    mpfi_clear( res_g           );
    mpfi_clear( res_g_prev      );
    mpfi_clear( tmp_a           );
    mpfi_clear( tmp_g           );
    mpfi_clear( union_ag        );
}



static void mpfi_agm_step(mpfi_ptr res_a, mpfi_ptr res_g, mpfi_srcptr op1, mpfi_srcptr op2)
{
    mpfi_t tmp_a, tmp_g;

    mpfi_init2(tmp_a, mpfi_get_prec(res_a));
    mpfi_init2(tmp_g, mpfi_get_prec(res_g));

    mpfi_agm_step_tmp(res_a, res_g, tmp_a, tmp_g, op1, op2);

    mpfi_clear(tmp_a);
    mpfi_clear(tmp_g);
}

#endif /* MPFI_AGM_H */
