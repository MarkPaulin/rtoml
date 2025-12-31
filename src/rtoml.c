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
  return Rf_ScalarReal(node.u.fp64);
}

SEXP parse_boolean(toml_datum_t node) {
  return Rf_ScalarLogical(node.u.boolean);
}

SEXP parse_local_date(toml_datum_t node) {
  SEXP vals = PROTECT(Rf_allocVector(INTSXP, 3));

  SET_INTEGER_ELT(vals, 0, node.u.ts.year);
  SET_INTEGER_ELT(vals, 1, node.u.ts.month);
  SET_INTEGER_ELT(vals, 2, node.u.ts.day);

  SEXP class = PROTECT(Rf_allocVector(STRSXP, 1));
  SET_STRING_ELT(class, 0, Rf_mkChar("rtoml_local_date"));
  Rf_classgets(vals, class);

  UNPROTECT(2);
  return vals;
}

SEXP parse_local_datetime(toml_datum_t node) {
  SEXP vals = PROTECT(Rf_allocVector(INTSXP, 7));

  SET_INTEGER_ELT(vals, 0, node.u.ts.year);
  SET_INTEGER_ELT(vals, 1, node.u.ts.month);
  SET_INTEGER_ELT(vals, 2, node.u.ts.day);
  SET_INTEGER_ELT(vals, 3, node.u.ts.hour);
  SET_INTEGER_ELT(vals, 4, node.u.ts.minute);
  SET_INTEGER_ELT(vals, 5, node.u.ts.second);
  SET_INTEGER_ELT(vals, 6, node.u.ts.usec);

  SEXP class = PROTECT(Rf_allocVector(STRSXP, 1));
  SET_STRING_ELT(class, 0, Rf_mkChar("rtoml_local_datetime"));
  Rf_classgets(vals, class);

  UNPROTECT(2);
  return vals;
}

SEXP parse_datetime(toml_datum_t node) {
  SEXP vals = PROTECT(Rf_allocVector(INTSXP, 8));

  SET_INTEGER_ELT(vals, 0, node.u.ts.year);
  SET_INTEGER_ELT(vals, 1, node.u.ts.month);
  SET_INTEGER_ELT(vals, 2, node.u.ts.day);
  SET_INTEGER_ELT(vals, 3, node.u.ts.hour);
  SET_INTEGER_ELT(vals, 4, node.u.ts.minute);
  SET_INTEGER_ELT(vals, 5, node.u.ts.second);
  SET_INTEGER_ELT(vals, 6, node.u.ts.usec);
  SET_INTEGER_ELT(vals, 7, node.u.ts.tz);

  SEXP class = PROTECT(Rf_allocVector(STRSXP, 1));
  SET_STRING_ELT(class, 0, Rf_mkChar("rtoml_datetime"));
  Rf_classgets(vals, class);

  UNPROTECT(2);
  return vals;
}

SEXP parse_time(toml_datum_t node) {
  SEXP vals = PROTECT(Rf_allocVector(INTSXP, 4));

  SET_INTEGER_ELT(vals, 0, node.u.ts.hour);
  SET_INTEGER_ELT(vals, 1, node.u.ts.minute);
  SET_INTEGER_ELT(vals, 2, node.u.ts.second);
  SET_INTEGER_ELT(vals, 3, node.u.ts.usec);

  SEXP class = PROTECT(Rf_allocVector(STRSXP, 1));
  SET_STRING_ELT(class, 0, Rf_mkChar("rtoml_time"));
  Rf_classgets(vals, class);

  UNPROTECT(2);
  return vals;
}

SEXP flatten_bool(SEXP x, int len) {
  SEXP out = PROTECT(Rf_allocVector(LGLSXP, len));
  for (int i = 0; i < len; i++) {
    SET_LOGICAL_ELT(out, i, LOGICAL(VECTOR_ELT(x, i))[0]);
  }
  UNPROTECT(1);
  return out;
}

SEXP flatten_numeric(SEXP x, int len) {
  SEXP out = PROTECT(Rf_allocVector(REALSXP, len));
  for (int i = 0; i < len; i++) {
    SET_REAL_ELT(out, i, REAL(VECTOR_ELT(x, i))[0]);
  }
  UNPROTECT(1);
  return out;
}

SEXP flatten_integer(SEXP x, int len) {
  SEXP out = PROTECT(Rf_allocVector(INTSXP, len));
  for (int i = 0; i < len; i++) {
    SET_INTEGER_ELT(out, i, INTEGER(VECTOR_ELT(x, i))[0]);
  }
  UNPROTECT(1);
  return out;
}

SEXP flatten_string(SEXP x, int len) {
  SEXP out = PROTECT(Rf_allocVector(STRSXP, len));
  for (int i = 0; i < len; i++) {
    SET_STRING_ELT(out, i, STRING_ELT(VECTOR_ELT(x, i), 0));
  }
  UNPROTECT(1);
  return out;
}

SEXP parse_array(toml_datum_t node) {
  int len = node.u.arr.size;
  bool single_type = true;
  SEXPTYPE last_type = NILSXP;

  SEXP out = PROTECT(Rf_allocVector(VECSXP, len));

  for (int i = 0; i < len; i++) {
    SET_VECTOR_ELT(out, i, parse_node(node.u.arr.elem[i]));
    if (single_type) {
      if (last_type == NILSXP) {
        last_type = TYPEOF(VECTOR_ELT(out, i));
      } else {
        single_type = last_type == TYPEOF(VECTOR_ELT(out, i));
      }
    }
  }

  if (single_type) {
    switch (last_type) {
      case STRSXP:
        out = flatten_string(out, len);
        break;
      case REALSXP:
        out = flatten_numeric(out, len);
        break;
      case INTSXP:
        out = flatten_integer(out, len);
        break;
      case LGLSXP:
        out = flatten_bool(out, len);
        break;
      default:
    }
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
    return parse_local_date(node);
  case TOML_TIME:
    return parse_time(node);
  case TOML_DATETIME:
    return parse_local_datetime(node);
  case TOML_DATETIMETZ:
    return parse_datetime(node);
  case TOML_ARRAY:
    return parse_array(node);
  default:
    return R_NilValue;
  }
}

SEXP parse_result(toml_result_t result) {
  if (!result.ok) {
    char msg[200];
    memcpy(msg, result.errmsg, 200); 
    toml_free(result);
    Rf_error("%s", msg);
  }
  
  if (result.toptab.type == TOML_UNKNOWN) {
    toml_free(result);
    Rf_error("Unknown datum type");
  }
  
  SEXP res = PROTECT(parse_node(result.toptab));
  toml_free(result);
  UNPROTECT(1);
  return res;
}

SEXP parse_toml_(SEXP str) {
    const char* str_c = Rf_translateCharUTF8(STRING_ELT(str, 0));
    toml_result_t result = toml_parse(str_c, strlen(str_c));

    return parse_result(result);
}

SEXP read_toml_(SEXP path) {
    const char* filename = Rf_translateCharUTF8(STRING_ELT(path, 0));
    filename = R_ExpandFileName(filename);
    toml_result_t result = toml_parse_file_ex(filename);

    return parse_result(result);
}
