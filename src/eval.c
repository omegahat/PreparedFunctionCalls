
/* Copyright Duncan Temple Lang, July 29th, 2008 */
#include "Rdefines.h"

SEXP R_NewHashedEnv(SEXP enclos, SEXP size);

/*
 Here we evaluate the body of the R function, filling in the 
 boxed values of the variables within the "call frame".
 Currently this assumes no additional arguments with default values
 as we don't create those in the call frame.
 This avoids the argument matching.

  We would want to use the functions environment of the 
 */
SEXP
R_evalFunBody(SEXP rx, SEXP ry, SEXP rz, SEXP fun)
{
    int *x;      /* The address where we put the actual value of the ith element of x */
    double *y;   /* for ry[i] */
    int *z;      /*  rz[i] */

    SEXP rans, tmp, env; /* The vector containing the answer elements for each iteration, 
                            a scratch variable for temporary handle to R variables we create and whose address of the unboxed element we need,
                            the environment in which we bind the "arguments". */

    SEXP body, funEnv;
    double *ans;

    int i, n;
    n = GET_LENGTH(rx);

    body = BODY(fun);
    funEnv = CLOENV(fun); // R_GlobalEnv

    PROTECT(env = R_NewHashedEnv(funEnv, ScalarInteger(3)));
    defineVar(Rf_install("x"), tmp = allocVector(INTSXP, 1), env);
    x = INTEGER(tmp);

    defineVar(Rf_install("y"), tmp = allocVector(REALSXP, 1), env);
    y = REAL(tmp);

    defineVar(Rf_install("z"), tmp = allocVector(LGLSXP, 1), env);
    z = LOGICAL(tmp);

    PROTECT(rans = NEW_NUMERIC(n));
    ans = REAL(rans);

    for(i = 0; i < n ; i++) {
	*x = INTEGER(rx)[i];
	*y = REAL(ry)[i];
	*z = INTEGER(rz)[i];
	tmp = Rf_eval(body, env);
	ans[i] = REAL(tmp)[0];
    }

    UNPROTECT(2);

    return(rans);
}
