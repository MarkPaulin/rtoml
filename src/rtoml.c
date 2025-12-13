#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

#include "tomlc17.h"

SEXP parse_from_str_(SEXP str) {
    SEXP res = PROTECT(Rf_allocVector(VECSXP, 0));
    UNPROTECT(1);
    return res;
}
