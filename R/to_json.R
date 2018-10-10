#' To JSON
#' 
#' Converts R objects to JSON
#' 
#' @param x object to convert to JSON
#' @param ... arguments passed to other methods
#' 
#' @examples 
#' 
#' to_json(1:3)
#' to_json(letters[1:3])
#' to_json(data.frame(x = 1:3, y = letters[1:3]))
#' to_json(list(x = 1:3, y = list(z = letters[1:3])))
#' to_json(seq(as.Date("2018-01-01"), as.Date("2018-01-05"), length.out = 5))
#' to_json(seq(as.Date("2018-01-01"), as.Date("2018-01-05"), length.out = 5), numeric_dates = FALSE)
#' 
#' @export
to_json <- function( x, ... ) UseMethod("to_json")

#' @rdname to_json
#' @export
#' @param numeric_dates logical indicating if dates should be treated as numerics. 
#' Defaults to TRUE for speed. If FALSE, the dates will be coerced to character
to_json.data.frame <- function( x, ..., numeric_dates = TRUE ) {
  if(!numeric_dates) x <- handle_dates( x ) 
  rcpp_df_to_json( x )
}

#' @rdname to_json
#' @export
to_json.numeric <- function( x, ... ) rcpp_numeric_to_json( x )

#' @rdname to_json
#' @export
to_json.character <- function( x, ... ) rcpp_character_to_json( x )

#' @rdname to_json
#' @export
to_json.integer <- function( x, ... ) rcpp_integer_to_json( x ) 

#' @rdname to_json
#' @export
to_json.logical <- function( x, ... ) rcpp_logical_to_json( x )

#' @rdname to_json
#' @export
to_json.complex <- function( x, ... ) rcpp_character_to_json( x )

#' @rdname  to_json
#' @export
to_json.matrix <- function( x, ... ) {
  if( is.integer( x ) ) return( rcpp_integer_matrix_to_json( x ) ) 
  if( is.numeric( x ) ) return( rcpp_numeric_matrix_to_json( x ) )
  return( rcpp_character_matrix_to_json( x ) )
}

#' @rdname to_json
#' @export
#' @inheritParams convert_to_json.data.frame
to_json.Date <- function( x, ..., numeric_dates = TRUE ) {
  if( numeric_dates ) return( rcpp_numeric_to_json( x ) ) 
  return( rcpp_character_to_json( as.character( x ) ) )
}

#' @rdname to_json
#' @export
#' @inheritParams convert_to_json.data.frame
to_json.POSIXct <- function( x, ..., numeric_dates = TRUE ) {
  if( numeric_dates ) return( rcpp_numeric_to_json( x ) ) 
  return( rcpp_character_to_json( as.character( x ) ) )
}

#' @rdname to_json
#' @export
#' @inheritParams convert_to_json.data.frame
to_json.POSIXlt <- function( x, ..., numeric_dates = TRUE ) {
  if( numeric_dates ) return( rcpp_list_to_json( x ) ) 
  return( rcpp_character_to_json( as.character( x ) ) )
}

#' @rdname to_json
#' @export
to_json.list <- function( x, ... ) rcpp_list_to_json( x )

#' @rdname to_json
#' @export
to_json.default <- function( x, ... ) rcpp_list_to_json( x ) # stop("this type is not supported")

date_columns <- function( df ) names(which(vapply(df , function(x) { inherits(x, "Date") || inherits(x, "POSIXct") }, T)))

handle_dates <- function( x ) {
  dte <- date_columns( x )
  for ( i in dte ) x[[i]] <- as.character( x[[i]] )
  return( x )
}





