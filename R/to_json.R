#' To JSON
#' 
#' Converts a data.frame to JSON
#' 
#' @param df data.frame to convert to JSON
#' 
#' @export
to_json <- function( df ) rcpp_to_json( df )



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
# n <- 1e5
# df <- data.frame(id = 1:n, dte = sample(seq(as.Date("2018-01-01"), as.Date("2018-01-31"), length.out = n)))
# 
# library(microbenchmark)
# microbenchmark(
#   one = {
#     handle_dates( df )
#   },
#   two ={
#     handle_dates2( df )
#   },
#   times = 10
# )