#' Convert a TOML string to an R list
#'
#' @param x A length-one character vector
#'
#' @returns A list 
#'
#' @export
read_toml_str <- function(x) {
  parse_from_str(x)
}
