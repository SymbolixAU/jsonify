#' validate JSON
#' 
#' Validates JSON
#' 
#' @param json character or json object
#' @return logical vector 
#' 
#' @examples
#' 
#' validate_json('[]')
#' df <- data.frame(id = 1:5, val = letters[1:5])
#' validate_json( to_json(df) )
#' 
#' validate_json('{"x":1,"y":2,"z":"a"}')
#' 
#' validate_json( c('{"x":1,"y":2,"z":"a"}', to_json(df) ) )
#' validate_json( c('{"x":1,"y":2,"z":a}', to_json(df) ) )
#' 
#' @export
validate_json <- function( json ) UseMethod("validate_json")

#' @export
validate_json.character <- function( json ) rcpp_validate_json( json )

#' @export
validate_json.json <- function( json ) rcpp_validate_json( json )

#' @export
validate_json.default <- function( json ) stop("Only character vectors are accepted")