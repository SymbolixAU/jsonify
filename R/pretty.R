
#' Pretty Json
#' 
#' Adds indentiation to a JSON string
#' 
#' @param json string of JSON
#' 
#' @examples
#' 
#' df <- data.frame(id = 1:10, val = rnorm(10))
#' js <- to_json( df )
#' pretty_json(js)
#' 
#' @export
pretty_json <- function( json ) {
  rcpp_pretty_json( json )
}


#' Minify Json
#' 
#' Removes indentiation from a JSON string
#' 
#' @param json string of JSON
#' 
#' @examples 
#' 
#' df <- data.frame(id = 1:10, val = rnorm(10))
#' js <- to_json( df )
#' jsp <- pretty_json(js)
#' minify_json( jsp )
#' 
#' @export
minify_json <- function( json ) {
  rcpp_minify_json( json )
}