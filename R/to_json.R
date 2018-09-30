#' To JSON
#' 
#' Converts a data.frame to JSON
#' 
#' @param x object to convert to JSON
#' @param numeric_dates logical indicating if dates should be treated as numerics. 
#' Defaults to TRUE for speed. If FALSE, the dates will be coerced to character
#' @export
to_json <- function( x, numeric_dates = TRUE ) {
  #if(!numeric_dates) x <- handle_dates( x ) 
  convert_to_json( x, numeric_dates )
}

convert_to_json <- function( x, numeric_dates = TRUE ) UseMethod("convert_to_json") 

#' @export
convert_to_json.data.frame <- function( x, numeric_dates = TRUE ) {
  if(!numeric_dates) x <- handle_dates( x ) 
  rcpp_df_to_json( x )
}

#' @export
convert_to_json.numeric <- function( x, ... ) rcpp_numeric_to_json( x )

#' @export
convert_to_json.character <- function( x, ... ) rcpp_character_to_json( x )

#' @export
convert_to_json.integer <- function( x, ... ) rcpp_integer_to_json( x ) 

#' @export
convert_to_json.logical <- function( x, ... ) rcpp_logical_to_json( x )

#' @export
convert_to_json.Date <- function( x, numeric_dates = TRUE ) {
  if( numeric_dates ) return( rcpp_numeric_to_json( x ) ) 
  return( rcpp_character_to_json( as.character( x ) ) )
}

#' @export
convert_to_json.POSIXct <- function( x, numeric_dates = TRUE ) {
  if( numeric_dates ) return( rcpp_numeric_to_json( x ) ) 
  return( rcpp_character_to_json( as.character( x ) ) )
}

#' @export
convert_to_json.POSIXlt <- function( x, numeric_dates = TRUE ) {
  if( numeric_dates ) return( rcpp_list_to_json( x ) ) 
  return( rcpp_character_to_json( as.character( x ) ) )
}

#' @export
convert_to_json.list <- function( x, ... ) rcpp_list_to_json( x )

#' @export
convert_to_json.default <- function( x, ... ) rcpp_list_to_json( x ) # stop("this type is not supported")

date_columns <- function( df ) names(which(vapply(df , function(x) { inherits(x, "Date") || inherits(x, "POSIXct") }, T)))

handle_dates <- function( x ) {
  dte <- date_columns( x )
  for ( i in dte ) x[[i]] <- as.character(x[[i]])
  return( x )
}