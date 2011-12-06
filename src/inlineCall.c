/* Copyright Duncan Temple Lang, July 29th, 2008 */

#include "Rdefines.h"

/*
  Have a prepared function call and evaluate it for each iteration.
  We still have the overhead of the function call and argument matching
  but avoid creating the call each time. Instead, we just fill in the
  boxed values of the arguments.
 */
SEXP
R_inlineFunCall(SEXP rx, SEXP ry, SEXP rz, SEXP fun)
{
    int *x;
    double *y;
    int *z;

    SEXP rans, e, tmp, p;
    double *ans;
    int i, n;
    
    n = GET_LENGTH(rx);
    PROTECT(rans = NEW_NUMERIC(n));
    ans = REAL(rans);

    PROTECT(e = allocVector(LANGSXP, 4)); 
    SETCAR(e, fun); p = CDR(e);
    SETCAR(p, tmp = allocVector(INTSXP, 1)); p = CDR(p);
    x = INTEGER(tmp);
    SETCAR(p, tmp = allocVector(REALSXP, 1)); p = CDR(p);
    y = REAL(tmp);
    SETCAR(p, tmp = allocVector(LGLSXP, 1));
    z = LOGICAL(tmp);


    for(i = 0; i < n ; i++) {
	*x = INTEGER(rx)[i];
	*y = REAL(ry)[i];
	*z = INTEGER(rz)[i];
	tmp = Rf_eval(e, R_GlobalEnv);
	ans[i] = REAL(tmp)[0];
    }

    UNPROTECT(2);
    return(rans);
}



#if 0
SEXP R_execClosure(SEXP call, SEXP op, SEXP arglist, SEXP rho, SEXP newrho);

SEXP
R_noArgMatchInlineFunCall(SEXP rx, SEXP ry, SEXP rz, SEXP fun)
{
    int *x;
    double *y;
    int *z;

    SEXP rans, args, tmp, p, call_env, call;
    double *ans;
    int i, n;
    
    n = GET_LENGTH(rx);
    PROTECT(rans = NEW_NUMERIC(n));
    ans = REAL(rans);

    PROTECT(args = allocVector(LANGSXP, 3)); p = args;
    SETCAR(p, tmp = allocVector(INTSXP, 1)); p = CDR(p);
    x = INTEGER(tmp);
    SETCAR(p, tmp = allocVector(REALSXP, 1)); p = CDR(p);
    y = REAL(tmp);
    SETCAR(p, tmp = allocVector(LGLSXP, 1));
    z = LOGICAL(tmp);

    PROTECT(call = allocVector(LANGSXP, 4));
    PROTECT(call_env = NewEnvironment(FORMALS(fun), args, CLOENV(fun)));

    for(i = 0; i < n ; i++) {
	*x = INTEGER(rx)[i];
	*y = REAL(ry)[i];
	*z = INTEGER(rz)[i];

	tmp = R_execClosure(call, fun, args, R_GlobalEnv, call_env);

	ans[i] = REAL(tmp)[0];
    }

    UNPROTECT(2);
    return(rans);
}


#endif

