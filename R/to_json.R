#' To JSON
#' 
#' Converts R objects to JSON
#' 
#' @param x object to convert to JSON
#' @param unbox logical indicating if single-value arrays should be 'unboxed', 
#' that is, not contained inside an array.
#' @param digits integer specifying the number of decimal places to round numerics. 
#' numeric values are coorced using \code{as.integer}, which may round-down the value you supply.
#' Default is \code{NULL} - no rounding
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
#' ## unbox single-value arrays
#' to_json(list(x = 1), unbox = TRUE)
#' to_json(list(x = 1, y = c("a"), z = list(x = 2, y = c("b"))), unbox = TRUE)
#' 
#' ## rounding numbers using the digits argument
#' to_json(1.23456789, digits = 2)
#' to_json(data.frame(x = 1L:3L, y = rnorm(3), z = letters[1:3], 
#'   stringsAsFactors = FALSE), digits = 0 )
#' 
#' 
#' @export
to_json <- function( x, unbox = FALSE, digits = NULL, ... ) {
  UseMethod("to_json")
}

#' @rdname to_json
#' @export
#' @param numeric_dates logical indicating if dates should be treated as numerics. 
#' Defaults to TRUE for speed. If FALSE, the dates will be coerced to character
to_json.data.frame <- function( x, unbox = FALSE, digits = NULL, ..., numeric_dates = TRUE ) {
  if(!numeric_dates) x <- handle_dates( x ) 
  digits <- handle_digits( digits )
  rcpp_to_json( x, unbox, digits )
}


#' @rdname to_json
#' @export
to_json.Date <- function( x, unbox = FALSE, ..., numeric_dates = TRUE ) {
  if( numeric_dates ) return( rcpp_to_json( x, unbox ) ) 
  return( rcpp_to_json( as.character( x ), unbox ) )
}

#' @rdname to_json
#' @export
to_json.POSIXct <- function( x, unbox = FALSE, ..., numeric_dates = TRUE ) {
  to_json.Date( x = x, unbox = unbox, ..., numeric_dates = numeric_dates )
}

#' @rdname to_json
#' @export
to_json.POSIXlt <- function( x, unbox = FALSE, ..., numeric_dates = TRUE )  {
  to_json.Date( x = x, unbox = unbox, ..., numeric_dates = numeric_dates )
}


#' @rdname to_json
#' @export
to_json.default <- function( x, unbox = FALSE, digits = NULL, ..., numeric_dates = TRUE ) {
  digits <- handle_digits( digits )
  cls <- get_classes( x )
  #if(!numeric_dates) x <- handle_dates( x ) 
  rcpp_to_json( x, cls, unbox, digits ) # stop("this type is not supported")
}

date_columns <- function( df ) names(which(vapply(df , function(x) { inherits(x, "Date") || inherits(x, "POSIXct") }, T)))

handle_dates <- function( x ) {
  dte <- date_columns( x )
  for ( i in dte ) x[[i]] <- as.character( x[[i]] )
  return( x )
}

handle_digits <- function( digits ) {
  if( is.null( digits )) return(-1)
  return( as.integer( digits ) )
}



handle_dates2 <- function( lst ) {
  rapply( lst, function(x) {
    any( grepl("Date|POSIX*", class(x)) )
  })
}

get_classes <- function( obj ) {
  rapply( obj, how = "list", function(x) {
    cls <- class(x)
    if("Date" %in% cls){
      "Date"
    } else if ("POSIXct" %in% cls ) {
      "POSIXct"
    } else {
      "other"
    }
    })
}


# sf <- mapdeck::roads[1:2, ]
# 
# l <- list(
#   n = NULL
#   , x = 1L:5L
#   , y = 1:5
#   , z = data.frame(i = letters[1:5], v = 21:25, l = list(lst = 1:5))
#   , aa = list(
#     bb = list(
#       cc = as.Date("2018-01-01")
#       )
#     )
#   # , d = as.POSIXlt("2018-01-01")
#   # , p = as.POSIXct("2018-01-01 00:00:01")
#    , sf = sf
#   )
# 
# v <- jsonify:::get_classes( l )
# length( v )
# v
# 
# to_json( l )

# handle_dates2 <- function(x) {
#   rapply( x, how = "replace", function(y) {
#     if(inherits(y, "Date") | inherits(y,"POSIXct") | inherits (y, "POSIXlt")) {
#       as.character( y )
#     } else { 
#       y
#     }
#   })
# }


# library(data.table)
# library(sf)
# 
# dtes <- seq(as.Date("2018-01-01"), as.Date("2019-01-01"), length.out = 365)
# psx <- seq(as.POSIXct("2018-01-01"), as.POSIXct("2019-01-01"), length.out = 365)
# n <- 1e5
# 
# sf <- mapdeck::roads
# 
# lst <- list(
#   x = sample(dtes, size = n, replace = T)
#   , y = list(
#     ya = sample(dtes, size = n, replace = TRUE)
#     , yb = rnorm(n = n)
#     , yx = list( sample(dtes, size = n, replace = T ) )
#   )
#   , p = psx
#   #, sf = sf
# )
# 
# library( microbenchmark )
# 
# get_classes( lst )
# 
# 
# 
# res <- jsonify::to_json( lst, numeric_dates = FALSE )
# 
# microbenchmark(
#   jsonify = {
#     v <- jsonify:::handle_dates2( lst )
#     jsonify::to_json( lst, numeric_dates = TRUE )
#   },
#   jsonlite = {
#     jsonlite::toJSON( lst )
#   },
#   times = 3
# )




