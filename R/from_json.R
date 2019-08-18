#' From JSON
#' 
#' Converts a JSON string to an R list
#' 
#' @param x JSON string to convert to R list
#' @param simplifyDataFrame logical, if \code{TRUE}, coerce JSON arrays 
#' containing only records (JSON objects) into a data frame. Default value is 
#' \code{TRUE}.
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
from_json <- function(x, simplifyDataFrame = TRUE) {
  res <- rcpp_from_json(x, simplifyDataFrame)
  
  # if (!simplifyDataFrame || !inherits(res, "list")) {
    return(res)
  # }
  # 
  # list_to_df(res)
}

# Helper function to convert a "from_json()" list to a data frame.
list_to_df <- function(res) {
  # Get all unique names from a list object.
  cols <- rcpp_get_col_headers(res)
  
  # If column headers are NULL, return res.
  if (is.null(cols)) {
    return(res)
  }
  
  # Convert row lists to column lists.
  columnlist <- rcpp_transpose_list(res, cols)
  
  # Convert all NULL values in each nested list to NA.
  for (i in columnlist) {
    rcpp_null_to_na(i)
  }
  
  # If the value of each inner list is length one, unlist each element, which 
  # will naturally convert data types and NA values within each "list column" 
  # to be the same, and thus data frame friendly.
  unlist_values <- TRUE
  for (i in columnlist) {
    for (j in i) {
      if (length(j) > 1) {
        unlist_values <- FALSE
        break
      }
      if (!unlist_values) {
        break
      }
    }
  }
  
  if (unlist_values) {
    # Unlist each element.
    columnlist <- lapply(columnlist, unlist, recursive = FALSE, 
                         use.names = FALSE)
  }

  # Convert columnlist to a data frame, then return.
  class(columnlist) <- "data.frame"
  row.names(columnlist) <- seq_len(length(res))
  colnames(columnlist) <- cols
  columnlist
}
