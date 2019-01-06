#' @useDynLib jsonify, .registration = TRUE
#' @importFrom Rcpp sourceCpp
NULL


#' @export
print.json <- function( x, ... ) cat( x )
