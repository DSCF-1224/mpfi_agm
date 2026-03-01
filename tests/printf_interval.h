static inline int printf_interval(const char* name, mpfi_srcptr x)
{
    return mpfr_printf( "%s : [%Rf %Rf]\n", name, &(x->left), &(x->right) );
}
