
#' @export
print.json <- function( x, ... ) cat( x )

#' @export
print.ndjson <- function( x, ... ) cat( x )

#' Pretty Json
#' 
#' Adds indentiation to a JSON string
#' 
#' @param json string of JSON
#' @param ... other argments passed to \link{to_json}
#' 
#' @examples
#' 
#' df <- data.frame(id = 1:10, val = rnorm(10))
#' js <- to_json( df )
#' pretty_json(js)
#' 
#' ## can also use directly on an R object
#' pretty_json( df )
#' 
#' @export
pretty_json <- function( json, ... ) UseMethod("pretty_json") 

#' @export
pretty_json.json <- function( json, ... ) rcpp_pretty_json( json )

#' @export
pretty_json.character <- function( json, ... ) pretty_json( as.json( json ) )

#' @export
pretty_json.default <- function( json, ... ) {
  js <- to_json( json, ... )
  rcpp_pretty_json( js )
}


#' Minify Json
#' 
#' Removes indentiation from a JSON string
#' 
#' @param json string of JSON
#' @param ... other argments passed to \link{to_json}
#' 
#' @examples 
#' 
#' df <- data.frame(id = 1:10, val = rnorm(10))
#' js <- to_json( df )
#' jsp <- pretty_json(js)
#' minify_json( jsp )
#' 
#' @export
minify_json <- function( json, ... ) UseMethod("minify_json") 

#' @export
minify_json.json <- function( json, ... ) rcpp_minify_json( json ) 

#' @export
minify_json.character <- function( json, ... ) minify_json( as.json( json ) )

#' @export
minify_json.default <- function( json, ... ) to_json( json, ... )



