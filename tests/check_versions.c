#include <stdio.h>
#include <stdlib.h>

#include <mpfr.h>
#include <mpfi.h>

int main(void)
{
#ifdef __STDC_VERSION__
    printf( "__STDC_VERSION__   : %ld\n"   , __STDC_VERSION__ );
#endif
#ifdef __STDC_ISO_10646__
    printf( "__STDC_ISO_10646__ : %ld\n\n" , __STDC_ISO_10646__ );
#endif
    printf( "GMP  VERSION; %s\n", gmp_version        );
    printf( "MPFR VERSION; %s\n", mpfr_get_version() );
    printf( "MPFI VERSION; %s\n", mpfi_get_version() );

    return EXIT_SUCCESS;
}
