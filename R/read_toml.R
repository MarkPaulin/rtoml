#' Convert a TOML string to an R list
#'
#' @param x A length-one character vector, either a path to a file for
#' `read_toml()` or a TOML document for `parse_toml()`
#' @param flatten Convert single-type arrays to a vector, default `TRUE`
#'
#' @returns A list
#' @name read_toml
#' @useDynLib rtoml, .registration = TRUE
NULL

#' @rdname read_toml
#' @export
parse_toml <- function(x, flatten = TRUE) {
  if (!is.character(x) || length(x) != 1) {
    stop("`x` must be a single string")
  }
  
  if (!is.logical(flatten) || length(flatten) != 1) {
    stop("`flatten` must be a single logical")
  }
  
  .Call(parse_toml_, x, flatten)
}

#' @rdname read_toml
#' @export
read_toml <- function(x, flatten = TRUE) {
  if (!is.character(x) || !length(x) == 1) {
    stop("`x` must be a single path")
  }
  
  if (!file.exists(x)) {
    stop("'", x, "' does not exist")
  }
  
  if (!is.logical(flatten) || length(flatten) != 1) {
    stop("`flatten` must be a single logical")
  }
  
  x <- normalizePath(x, mustWork = TRUE)
  .Call(read_toml_, x, flatten)
}
