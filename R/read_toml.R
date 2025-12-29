#' Convert a TOML string to an R list
#'
#' @param x A length-one character vector, either a path to a file for
#' `read_toml()` or a TOML document for `parse_toml()`
#'
#' @returns A list
#' @name read_toml
#' @useDynLib rtoml, .registration = TRUE
NULL

#' @rdname read_toml
#' @export
parse_toml <- function(x) {
  .Call(parse_from_str_, x)
}

#' @rdname read_toml
#' @export
read_toml <- function(x) {
  .Call(parse_from_file_, x)
}
