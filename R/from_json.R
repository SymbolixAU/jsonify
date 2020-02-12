#' From JSON
#' 
#' Converts JSON to an R object. 
#' 
#' @param json JSON to convert to R object. Can be a string, url or link to a file.
#' @param simplify logical, if \code{TRUE}, coerces JSON to the simplest R object possible. See Details
#' @param fill_na logical, if \code{TRUE} and \code{simplify} is \code{TRUE}, 
#' data.frames will be na-filled if there are missing JSON keys.
#' Ignored if \code{simplify} is \code{FALSE}. See details and examples.
#' @param buffer_size size of buffer used when reading a file from disk. Defaults to 1024
#' @details 
#' 
#' When \code{simplify = TRUE}
#' \itemize{
#'   \item{single arrays are coerced to vectors}
#'   \item{array of arrays (all the same length) are coerced to matrices}
#'   \item{objects with the same keys are coerced to data.frames}
#' }
#' 
#' When \code{simplify = TRUE} and \code{fill_na = TRUE}
#' \itemize{
#'   \item{objects are coerced to data.frames, and any missing values are filled with NAs}
#' }
#' 
#' @examples 
#' 
#' from_json('{"a":[1, 2, 3]}')
#' from_json('{"a":8, "b":99.5, "c":true, "d":"cats", "e":[1, "cats", 3]}')
#' from_json('{"a":8, "b":{"c":123, "d":{"e":456}}}')
#' 
#' lst <- list("a" = 5L, "b" = 1.43, "c" = "cats", "d" = FALSE)
#' js <- jsonify::to_json(lst, unbox = TRUE)
#' from_json( js )
#' 
#' ## Return a data frame
#' from_json('[{"id":1,"val":"a"},{"id":2,"val":"b"}]')
#' 
#' ## Return a data frame with a list column
#' from_json('[{"id":1,"val":"a"},{"id":2,"val":["b","c"]}]')
#' 
#' ## Without simplifying to a data.frame
#' from_json('[{"id":1,"val":"a"},{"id":2,"val":["b","c"]}]', simplify = FALSE )
#' 
#' ## Missing JSON keys 
#' from_json('[{"x":1},{"x":2,"y":"hello"}]')
#' 
#' ## Missing JSON keys - filling with NAs
#' from_json('[{"x":1},{"x":2,"y":"hello"}]', fill_na = TRUE )
#' 
#' ## Duplicate object keys
#' from_json('[{"x":1,"x":"a"},{"x":2,"x":"b"}]')
#' 
#' from_json('[{"id":1,"val":"a","val":1},{"id":2,"val":"b"}]', fill_na = TRUE )
#' 
#' 
#' @export
from_json <- function(json, simplify = TRUE, fill_na = FALSE, buffer_size = 1024 ) {
  json_to_r( json, simplify, fill_na, buffer_size )
}

#' from ndjson
#' 
#' Converts ndjson into R objects
#' 
#' @inheritParams from_json
#' 
#' @examples
#' 
#' js <- to_ndjson( data.frame( x = 1:5, y = 6:10 ) )
#' from_ndjson( js )
#' 
#' 
#' @export
from_ndjson <- function(ndjson, simplify = TRUE, fill_na = FALSE ) {
  ndjson_to_r( ndjson, simplify, fill_na )
}


json_to_r <- function( json, simplify = TRUE, fill_na = FALSE, buffer_size ) {
  UseMethod("json_to_r")
}

ndjson_to_r <- function( ndjson, simplify = TRUE, fill_na = FALSE ) {
  UseMethod("ndjson_to_r")
}

#' @export
json_to_r.character <- function( json, simplify = TRUE, fill_na, buffer_size ) {
  if( is_url( json ) ) {
    return(
      json_to_r( url( json ), simplify, fill_na, buffer_size )
    )
  } else if ( file.exists( json ) ) {
    return(
      rcpp_read_json_file(
        normalizePath( json )
        , get_download_mode()
        , simplify
        , fill_na
        , buffer_size
      )
    )
  }
  return( rcpp_from_json( json, simplify, fill_na ) )
}

#' @export
ndjson_to_r.character <- function( ndjson, simplify = TRUE, fill_na ) {
  if( is_url( ndjson ) ) {
    return(
      json_to_r( url( ndjson ), simplify, fill_na )
    )
  } else if ( file.exists( ndjson ) ) {
    return(
      rcpp_read_ndjson_file(
        normalizePath( ndjson )
        , get_download_mode()
        , simplify
        , fill_na
        #, buffer_size
      )
    )
  }
  return( rcpp_from_ndjson( ndjson, simplify, fill_na ) )
}

#' @export
json_to_r.connection <- function( json, simplify = TRUE, fill_na, buffer_size ) {
  json_to_r( read_url( json ), simplify, fill_na, buffer_size )
}

#' @export
ndjson_to_r.connection <- function( ndjson, simplify = TRUE, fill_na ) {
  json_to_r( read_url( ndjson ), simplify, fill_na )
}

#' @export
json_to_r.json <- function( json, simplify = TRUE, fill_na, buffer_size ) {
  rcpp_from_json( json, simplify, fill_na )
}

#' @export
ndjson_to_r.ndjson <- function( ndjson, simplify = TRUE, fill_na ) {
  rcpp_from_ndjson( ndjson, simplify, fill_na )
}

#' @export
json_to_r.default <- function( json, simplify = TRUE, fill_na, buffer_size ) {
  stop("jsonify - expecting a JSON string, url or file")
}

#' @export
ndjson_to_r.default <- function( ndjson, simplify = TRUE, fill_na ) {
  stop("jsonify - expecting an ndjson string, url or file")
}


is_url <- function(json) grepl("^https?://", json, useBytes = TRUE)

read_url <- function(con) {
  out <- tryCatch({
    paste0(readLines(con), collapse = "")
  },
  error = function(cond){
    stop("There was an error downloading the json")
  },
  finally = {
    close(con)
  })
}

get_download_mode <- function() {
  ifelse( .Platform$OS.type == "windows", "r", "rb" )
}