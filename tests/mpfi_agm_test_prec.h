#ifndef MPFI_AGM_TEST_PREC_H
#define MPFI_AGM_TEST_PREC_H

#include <mpfr.h>

static const mpfr_prec_t TEST_PREC_LIST[] = {53, 113, 237};

static const int TEST_PREC_LIST_LEN = sizeof(TEST_PREC_LIST) / sizeof(TEST_PREC_LIST[0]);

#endif /*  MPFI_AGM_TEST_PREC_H */
