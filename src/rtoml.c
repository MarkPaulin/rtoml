#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

#include "tomlc17.h"

SEXP parse_node(toml_datum_t node);

SEXP parse_table(toml_datum_t node) {
  int len = node.u.tab.size;
  SEXP keys = PROTECT(Rf_allocVector(STRSXP, len));
  SEXP vals = PROTECT(Rf_allocVector(VECSXP, len));

  for (int i = 0; i < len; i++) {
    SET_STRING_ELT(keys, i, Rf_mkCharLen(node.u.tab.key[i], node.u.tab.len[i]));
    SET_VECTOR_ELT(vals, i, parse_node(node.u.tab.value[i]));
  }

  Rf_setAttrib(vals, R_NamesSymbol, keys);
  UNPROTECT(2);
  return vals;
}

SEXP parse_string(toml_datum_t node) {
  return Rf_mkString(node.u.str.ptr);
}

SEXP parse_int64(toml_datum_t node) {
  long long int val = node.u.int64;
  if (val > 2147483647 || val < -2147483647) {
    return Rf_ScalarReal(val);
  }
  return Rf_ScalarInteger(val);
}

SEXP parse_float64(toml_datum_t node) {
  return Rf_ScalarInteger(node.u.fp64);
}

SEXP parse_boolean(toml_datum_t node) {
  return Rf_ScalarLogical(node.u.boolean);
}

SEXP parse_ts(toml_datum_t node) {
  SEXP out = PROTECT(Rf_allocVector(INTSXP, 8));
  SET_INTEGER_ELT(out, 0, node.u.ts.year);
  SET_INTEGER_ELT(out, 1, node.u.ts.month);
  SET_INTEGER_ELT(out, 2, node.u.ts.day);
  SET_INTEGER_ELT(out, 3, node.u.ts.hour);
  SET_INTEGER_ELT(out, 4, node.u.ts.minute);
  SET_INTEGER_ELT(out, 5, node.u.ts.second);
  SET_INTEGER_ELT(out, 6, node.u.ts.usec);
  SET_INTEGER_ELT(out, 7, node.u.ts.tz);
  UNPROTECT(1);
  return out;
}

SEXP parse_array(toml_datum_t node) {
  int len = node.u.arr.size;
  SEXP out = PROTECT(Rf_allocVector(VECSXP, len));

  for (int i = 0; i < len; i++) {
    SET_VECTOR_ELT(out, i, parse_node(node.u.arr.elem[i]));
  }

  UNPROTECT(1);
  return out;
}

SEXP parse_node(toml_datum_t node) {
  switch (node.type) {
  case TOML_TABLE:
    return parse_table(node);
  case TOML_STRING:
    return parse_string(node);
  case TOML_INT64:
    return parse_int64(node);
  case TOML_FP64:
    return parse_float64(node);
  case TOML_BOOLEAN:
    return parse_boolean(node);
  case TOML_DATE:
  case TOML_DATETIME:
  case TOML_DATETIMETZ:
  case TOML_TIME:
    return parse_ts(node);
  case TOML_ARRAY:
    return parse_array(node);
  default:
    return R_NilValue;
  }
}

SEXP parse_from_str_(SEXP str) {
    const char* str_c = Rf_translateCharUTF8(STRING_ELT(str, 0));
    toml_result_t result = toml_parse(str_c, strlen(str_c));

    if (!result.ok) {
        toml_free(result);
        return Rf_allocVector(STRSXP, 0);
    }

    if (result.toptab.type == TOML_UNKNOWN) {
        toml_free(result);
        return Rf_allocVector(STRSXP, 0);
    }

    SEXP res = PROTECT(parse_node(result.toptab));
    toml_free(result);
    UNPROTECT(1);
    return res;
}
