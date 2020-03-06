#' To JSON
#' 
#' Converts R objects to JSON
#' 
#' @param x object to convert to JSON
#' @param unbox logical indicating if single-value arrays should be 'unboxed', 
#' that is, not contained inside an array.
#' @param digits integer specifying the number of decimal places to round numerics.
#' Default is \code{NULL} - no rounding
#' @param numeric_dates logical indicating if dates should be treated as numerics. 
#' Defaults to TRUE for speed. If FALSE, the dates will be coerced to character in UTC time zone
#' @param factors_as_string logical indicating if factors should be treated as strings. Defaults to TRUE.
#' @param by either "row" or "column" indicating if data.frames and matrices should be processed
#' row-wise or column-wise. Defaults to "row"
#' 
#' @examples 
#' 
#' to_json(1:3)
#' to_json(letters[1:3])
#' 
#' ## factors treated as strings
#' to_json(data.frame(x = 1:3, y = letters[1:3], stringsAsFactors = TRUE ))
#' to_json(data.frame(x = 1:3, y = letters[1:3], stringsAsFactors = FALSE ))
#' 
#' to_json(list(x = 1:3, y = list(z = letters[1:3])))
#' to_json(seq(as.Date("2018-01-01"), as.Date("2018-01-05"), length.out = 5))
#' to_json(seq(as.Date("2018-01-01"), as.Date("2018-01-05"), length.out = 5), numeric_dates = FALSE)
#' 
#' psx <- seq(
#'   as.POSIXct("2018-01-01", tz = "Australia/Melbourne"), 
#'   as.POSIXct("2018-02-01", tz = "Australia/Melbourne"), 
#'   length.out = 5
#'   )
#' to_json(psx)
#' to_json(psx, numeric_dates = FALSE)
#' 
#' ## unbox single-value arrays
#' to_json(list(x = 1), unbox = TRUE)
#' to_json(list(x = 1, y = c("a"), z = list(x = 2, y = c("b"))), unbox = TRUE)
#' 
#' ## rounding numbers using the digits argument
#' to_json(1.23456789, digits = 2)
#' df <- data.frame(x = 1L:3L, y = rnorm(3), z = letters[1:3], stringsAsFactors = TRUE )
#' to_json(df, digits = 0 )
#' 
#' ## keeping factors
#' to_json(df, digits = 2, factors_as_string = FALSE )
#' 
#' 
#' @export
to_json <- function( x, unbox = FALSE, digits = NULL, numeric_dates = TRUE, 
                     factors_as_string = TRUE, by = "row" ) {
  if( "col" %in% by ) by <- "column"
  by <- match.arg( by, choices = c("row", "column") )
  digits <- handle_digits( digits )
  rcpp_to_json( x, unbox, digits, numeric_dates, factors_as_string, by )
}

#' To ndjson
#' 
#' Converts R objects to ndjson
#' 
#' @inheritParams to_json
#' 
#' @details 
#' 
#' Lists are converted to ndjson non-recursively. That is, each of the objects
#' in the list at the top level are converted to a new-line JSON object. Any nested
#' sub-elements are then contained within that JSON object. See examples
#' 
#' @examples 
#' 
#' to_ndjson( 1:5 )
#' to_ndjson( letters )
#' 
#' mat <- matrix(1:6, ncol = 2)
#' 
#' to_ndjson( x = mat )
#' to_ndjson( x = mat, by = "col" )
#' 
#' df <- data.frame(
#'   x = 1:5
#'   , y = letters[1:5]
#'   , z = as.Date(seq(18262, 18262 + 4, by = 1 ), origin = "1970-01-01" )
#'   )
#' 
#' to_ndjson( x = df )
#' to_ndjson( x = df, numeric_dates = FALSE )
#' to_ndjson( x = df, factors_as_string = FALSE )
#' to_ndjson( x = df, by = "column" )
#' to_ndjson( x = df, by = "column", numeric_dates = FALSE )
#' 
#' ## Lists are non-recurisve; only elements `x` and `y` are converted to ndjson
#' lst <- list(
#'   x = 1:5
#'   , y = list(
#'     a = letters[1:5]
#'     , b = data.frame(i = 10:15, j = 20:25)
#'   )
#' )
#'  
#' to_ndjson( x = lst )
#' to_ndjson( x = lst, by = "column")
#' 
#' 
#' @export
to_ndjson <- function( x, unbox = FALSE, digits = NULL, numeric_dates = TRUE, 
                       factors_as_string = TRUE, by = "row" ) {
  if( "col" %in% by ) by <- "column"
  by <- match.arg( by, choices = c("row", "column") )
  digits <- handle_digits( digits )
  rcpp_to_ndjson( x, unbox, digits, numeric_dates, factors_as_string, by )
}

handle_digits <- function( digits ) {
  if( is.null( digits ) ) return(-1L)
  return( as.integer( digits ) )
}

#' Coerce string to JSON
#' 
#' @param x string to coerce to JSON
#' 
#' @examples 
#' 
#' js <- '{"x":1,"y":2}'
#' as.json(js)
#' 
#' @export
as.json <- function(x) {
  if( !jsonify::validate_json( x ) ) 
    stop("Invalid JSON")

  attr(x, "class") <- "json"
  x
}


