#' @useDynLib rtoml, .registration = TRUE
parse_from_str <- function(file) {
  .Call(parse_from_str_, file)
}
