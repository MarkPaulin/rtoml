#' @useDynLib rtoml, .registration = TRUE
parse_from_str <- function(x) {
  .Call(parse_from_str_, x)
}
