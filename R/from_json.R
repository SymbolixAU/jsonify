

## From JSON
## - design ideas
## 
## - two passes over the JSON, keeping track of keys, types (num, int, str), obj vs array,
## - and build an appropraite structure based on this? 
## - then pass again and fill?

##
## - 


#' From JSON
#' 
#' Converts a JSON string to an R list
#' 
#' @param x JSON string to convert to R list
#' 
#' @examples 
#' 
#' from_json("{\"a\":[1, 2, 3]}")
#' from_json("{\"a\":8, \"b\":99.5, \"c\":true, \"d\":\"cats\", \"e\":[1, \"cats\", 3]}")
#' from_json("{\"a\":8, \"b\":{\"c\":123, \"d\":{\"e\":456}}}")
#' 
#' json_str <- jsonify::to_json(list("a" = 5L, "b" = 1.43, "c" = "cats", "d" = FALSE))
#' from_json(json_str)
#' 
#' @export
from_json <- function(x) {
  rcpp_from_json(x)
}
