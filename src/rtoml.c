#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

#include "tomlc17.h"

SEXP parse_from_str_(SEXP str) {
    return Rf_ScalarReal(1);
}