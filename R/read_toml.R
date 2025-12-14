#' Convert a TOML string to an R list
#'
#' @param x A length-one character vector
#'
#' @returns A list
#' @name read_toml
NULL

#' @rdname read_toml
#' @export
read_toml_str <- function(x) {
  parse_from_str(x)
}

#' @rdname read_toml
#' @export
read_toml <- function(x) {
  parse_from_file(x)
}
