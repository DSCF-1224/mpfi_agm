#ifndef MPFI_AGM_H
#define MPFI_AGM_H

#include <stdbool.h>

#include <mpfi.h>



void mpfi_agm(mpfi_ptr res, mpfi_srcptr op1, mpfi_srcptr op2);

void mpfi_agm_step(mpfi_ptr res_a, mpfi_ptr res_g, mpfi_srcptr op1, mpfi_srcptr op2);

void mpfi_agm_step_tmp(mpfi_ptr res_a, mpfi_ptr res_g, mpfi_ptr tmp_a, mpfi_ptr tmp_g, mpfi_srcptr op1, mpfi_srcptr op2);

#endif /* MPFI_AGM_H */
