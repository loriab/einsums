/*
 *
 * cblas_ssyr.c
 * This program is a C interface to ssyr.
 * Written by Keita Teranishi
 * 4/6/1998
 *
 */
#include "cblas.h"
#include "cblas_f77.h"
void cblas_ssyr(const CBLAS_LAYOUT layout, const CBLAS_UPLO Uplo, const CBLAS_INT N, const float alpha, const float *X,
                const CBLAS_INT incX, float *A, const CBLAS_INT lda) {
    char UL;
#ifdef F77_CHAR
    F77_CHAR F77_UL;
#else
#define F77_UL &UL
#endif

#ifdef F77_INT
    F77_INT F77_N = N, F77_incX = incX, F77_lda = lda;
#else
#define F77_N N
#define F77_incX incX
#define F77_lda lda
#endif
    extern int CBLAS_CallFromC;
    extern int RowMajorStrg;
    RowMajorStrg = 0;
    CBLAS_CallFromC = 1;
    if (layout == CblasColMajor) {
        if (Uplo == CblasLower)
            UL = 'L';
        else if (Uplo == CblasUpper)
            UL = 'U';
        else {
            cblas_xerbla(2, "cblas_ssyr", "Illegal Uplo setting, %d\n", Uplo);
            CBLAS_CallFromC = 0;
            RowMajorStrg = 0;
            return;
        }
#ifdef F77_CHAR
        F77_UL = C2F_CHAR(&UL);
#endif

        F77_ssyr(F77_UL, &F77_N, &alpha, X, &F77_incX, A, &F77_lda);

    } else if (layout == CblasRowMajor) {
        RowMajorStrg = 1;
        if (Uplo == CblasLower)
            UL = 'U';
        else if (Uplo == CblasUpper)
            UL = 'L';
        else {
            cblas_xerbla(2, "cblas_ssyr", "Illegal Uplo setting, %d\n", Uplo);
            CBLAS_CallFromC = 0;
            RowMajorStrg = 0;
            return;
        }
#ifdef F77_CHAR
        F77_UL = C2F_CHAR(&UL);
#endif
        F77_ssyr(F77_UL, &F77_N, &alpha, X, &F77_incX, A, &F77_lda);
    } else
        cblas_xerbla(1, "cblas_ssyr", "Illegal layout setting, %d\n", layout);
    CBLAS_CallFromC = 0;
    RowMajorStrg = 0;
    return;
}
