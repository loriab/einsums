/*
 * cblas_chpr.c
 * The program is a C interface to chpr.
 *
 * Keita Teranishi  3/23/98
 *
 */
#include "cblas.h"
#include "cblas_f77.h"

#include <stdio.h>
#include <stdlib.h>
void cblas_chpr(const CBLAS_LAYOUT layout, const CBLAS_UPLO Uplo, const CBLAS_INT N, const float alpha, const void *X, const CBLAS_INT incX,
                void *A) {
    char UL;
#ifdef F77_CHAR
    F77_CHAR F77_UL;
#else
#define F77_UL &UL
#endif

#ifdef F77_INT
    F77_INT F77_N = N, F77_incX = incX;
#else
#define F77_N N
#define F77_incX incx
#endif
    CBLAS_INT n, i, tincx, incx = incX;
    float *x = (float *)X, *xx = (float *)X, *tx, *st;

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
            cblas_xerbla(2, "cblas_chpr", "Illegal Uplo setting, %d\n", Uplo);
            CBLAS_CallFromC = 0;
            RowMajorStrg = 0;
            return;
        }
#ifdef F77_CHAR
        F77_UL = C2F_CHAR(&UL);
#endif

        F77_chpr(F77_UL, &F77_N, &alpha, X, &F77_incX, A);

    } else if (layout == CblasRowMajor) {
        RowMajorStrg = 1;
        if (Uplo == CblasUpper)
            UL = 'L';
        else if (Uplo == CblasLower)
            UL = 'U';
        else {
            cblas_xerbla(2, "cblas_chpr", "Illegal Uplo setting, %d\n", Uplo);
            CBLAS_CallFromC = 0;
            RowMajorStrg = 0;
            return;
        }
#ifdef F77_CHAR
        F77_UL = C2F_CHAR(&UL);
#endif
        if (N > 0) {
            n = N << 1;
            x = malloc(n * sizeof(float));
            tx = x;
            if (incX > 0) {
                i = incX << 1;
                tincx = 2;
                st = x + n;
            } else {
                i = incX * (-2);
                tincx = -2;
                st = x - 2;
                x += (n - 2);
            }
            do {
                *x = *xx;
                x[1] = -xx[1];
                x += tincx;
                xx += i;
            } while (x != st);
            x = tx;
#ifdef F77_INT
            F77_incX = 1;
#else
            incx = 1;
#endif
        } else
            x = (float *)X;

        F77_chpr(F77_UL, &F77_N, &alpha, x, &F77_incX, A);

    } else {
        cblas_xerbla(1, "cblas_chpr", "Illegal layout setting, %d\n", layout);
        CBLAS_CallFromC = 0;
        RowMajorStrg = 0;
        return;
    }
    if (X != x)
        free(x);
    CBLAS_CallFromC = 0;
    RowMajorStrg = 0;
    return;
}
