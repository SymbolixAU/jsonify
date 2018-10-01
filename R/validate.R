#' validate
#' 
#' Validates JSON
#' 
#' @param json character or json object
#' @return logical vector 
#' 
#' @examples
#' 
#' validate('[]')
#' df <- data.frame(id = 1:5, val = letters[1:5])
#' validate( to_json(df) )
#' 
#' validate('{"x":1,"y":2,"z":"a"}')
#' 
#' validate( c('{"x":1,"y":2,"z":"a"}', to_json(df) ) )
#' validate( c('{"x":1,"y":2,"z":a}', to_json(df) ) )
#' 
#' @export
validate <- function( json ) UseMethod("validate")

#' @export
validate.character <- function( json ) rcpp_validate_json( json )

#' @export
validate.json <- function( json ) rcpp_validate_json( json )

#' @export
validate.default <- function( json ) stop("Only character vectors are accepted")

