#' To JSON
#' 
#' Converts a data.frame to JSON
#' 
#' @param df data.frame to convert to JSON
#' 
#' @export
to_json <- function( x ) UseMethod("to_json") 

#' @export
to_json.data.frame <- function(x) rcpp_df_to_json( x )

#' @export
to_json.numeric <- function( x ) rcpp_numeric_to_json( x )

#' @export
to_json.character <- function( x ) rcpp_character_to_json( x )

#' @export
to_json.integer <- function( x ) rcpp_integer_to_json( x ) 

#' @export
to_json.logical <- function( x ) rcpp_logical_to_json( x )

#' #' @export
#' to_json.list <- function( x ) rcpp_list_to_json( x )

#' @export
to_json.default <- function( x ) stop("this type not supported")


col_classes <- function( df ) vapply( df, function(x) class(x)[[1]], "")

date_columns <- function( sf ) names(which(vapply(sf , function(x) { inherits(x, "Date") || inherits(x, "POSIXct") }, T)))

handle_dates <- function( x ) {
  dte <- date_columns( x )
  for ( i in dte ) {
    x[[i]] <- as.character(x[[i]])
  }
  return( x )
}


# handle_dates2 <- function( x ) {
#   dte <- date_columns( x )
#   for ( i in dte ) {
#     x[[i]] <- as.character(x[[i]])
#   }
#   return(x)
# }
# 

# n <- 1e6
# df <- data.frame(id = 1:n, dte = sample(seq(as.Date("2018-01-01"), as.Date("2018-01-31"), length.out = n)))
# 
# library(microbenchmark)
# microbenchmark(
#   numeric = {
#     jsonify::to_json( df )
#   },
#   character ={
#     jsonify::to_json( jsonify:::handle_dates( df ) )
#   },
#   times = 5
# )
# Unit: milliseconds
#      expr       min       lq     mean   median       uq      max neval
#   numeric  982.7153 1113.085 1117.633 1140.885 1149.160 1202.320     5
# character 4802.2583 4829.550 5130.051 4916.636 5146.547 5955.265     5