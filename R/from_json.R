#' From JSON
#' 
#' Converts a JSON string to an R list
#' 
#' @param json JSON string to convert to R list
#' @param simplify logical, if \code{TRUE}, coerce JSON arrays 
#' containing only records (JSON objects) into a data frame. Default value is 
#' \code{TRUE}.
#' @param buffer_size size of buffer used when reading a file from disk. Defaults 1024
#' 
#' @examples 
#' 
#' from_json("{\"a\":[1, 2, 3]}")
#' from_json("{\"a\":8, \"b\":99.5, \"c\":true, \"d\":\"cats\", \"e\":[1, \"cats\", 3]}")
#' from_json("{\"a\":8, \"b\":{\"c\":123, \"d\":{\"e\":456}}}")
#' 
#' lst <- list("a" = 5L, "b" = 1.43, "c" = "cats", "d" = FALSE)
#' json_str <- jsonify::to_json(lst, unbox = TRUE)
#' from_json(json_str)
#' 
#' # Return a data frame
#' from_json("[{\"id\":1,\"val\":\"a\"},{\"id\":2,\"val\":\"b\"}]")
#' 
#' @export
from_json <- function(json, simplify = TRUE, buffer_size = 1024 ) {
  json_to_r( json, simplify, buffer_size )
}

json_to_r <- function( json, simplify = TRUE, buffer_size ) {
  UseMethod("json_to_r")
}

#' @export
json_to_r.character <- function( json, simplify = TRUE, buffer_size ) {
  if( is_url( json ) ) {
    return( json_to_r( curl::curl( json ), simplify, buffer_size ))
  } else if ( file.exists( json ) ) {
    return(
      rcpp_read_json_file(
        normalizePath( json )
        , get_download_mode()
        , simplify
        , buffer_size
      )
    )
  }
  return( rcpp_from_json( json, simplify ) )
}

#' @export
json_to_r.connection <- function( json, simplify = TRUE, buffer_size ) {
  json_to_r( read_url( json ), simplify, buffer_size )
}

#' @export
json_to_r.json <- function( json, simplify = TRUE, buffer_size ) {
  rcpp_from_json( json, simplify )
}

#' @export
json_to_r.default <- function( json, simplify = TRUE, buffer_size ) {
  stop("jsonify - expecting a JSON string, url or file")
}

# json_to_r.default <- function( json, simplify = TRUE, buffer_size ) {
#   res <- rcpp_from_json( json, simplify )
#   return( res )
# }


is_url <- function(geojson) grepl("^https?://", geojson, useBytes=TRUE)

read_url <- function(con) {
  out <- tryCatch({
    paste0(readLines(con), collapse = "")
  },
  error = function(cond){
    stop("There was an error downloading the geojson")
  },
  finally = {
    close(con)
  })
}

get_download_mode <- function() {
  ifelse( .Platform$OS.type == "windows", "r", "rb" )
}