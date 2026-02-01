#include <stdio.h>
#include <stdlib.h>

#include <mpfr.h>
#include <mpfi.h>

int main(void)
{
    printf( "GMP  VERSION; %s\n", gmp_version        );
    printf( "MPFR VERSION; %s\n", mpfr_get_version() );
    printf( "MPFI VERSION; %s\n", mpfi_get_version() );

    return EXIT_SUCCESS;
}
