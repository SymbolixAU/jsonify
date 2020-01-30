# 
# 
# df <- data.frame(
#   x = 1:5
#   , y = letters[1:5]
# )
# 
# cat( jsonify:::rcpp_to_ndjson(df, FALSE, -1L, TRUE, TRUE, "row" ) )
# cat( jsonify:::rcpp_to_ndjson(df, FALSE, -1L, TRUE, TRUE, "column") )
# 
# jsonlite::stream_out(df)
# 
# mat <- matrix(c(T,F,F,T,T,F), ncol = 2)
# cat( jsonify:::rcpp_to_ndjson(mat, FALSE, -1L, TRUE, TRUE, "row" ) )
# cat( jsonify:::rcpp_to_ndjson(mat, FALSE, -1L, TRUE, TRUE, "column") )
# 
# jsonlite::stream_out(mat)
# 
# mat <- matrix(1:6, ncol = 2)
# cat( jsonify:::rcpp_to_ndjson(mat, FALSE, -1L, TRUE, TRUE, "row" ) )
# cat( jsonify:::rcpp_to_ndjson(mat, FALSE, -1L, TRUE, TRUE, "column") )
# 
# mat[1,1] <- 1.1
# cat( jsonify:::rcpp_to_ndjson(mat, FALSE, -1L, TRUE, TRUE, "row" ) )
# cat( jsonify:::rcpp_to_ndjson(mat, FALSE, -1L, TRUE, TRUE, "column") )
# 
# mat <- matrix(letters[1:6], ncol = 2)
# cat( jsonify:::rcpp_to_ndjson(mat, FALSE, -1L, TRUE, TRUE, "row" ) )
# cat( jsonify:::rcpp_to_ndjson(mat, FALSE, -1L, TRUE, TRUE, "column") )
# 
# lst <- list(
#   x = 1:5
#   , y = list(
#     a = letters[1:5]
#     , b = data.frame(i = 10:15, j = 20:25)
#   )
# )
# 
# cat( jsonify:::rcpp_to_ndjson(lst, FALSE, -1L, TRUE, TRUE, "row") )
# 
# N <- 1e6
# data <- data.frame(
#   longitude = runif(N, -180, 180),
#   latitude = runif(N, -180, 180),
#   tooltip = sample(LETTERS, N, replace = TRUE)
#   , stringsAsFactors = F
# )
# 
# 
# 
# library(microbenchmark)
# 
# microbenchmark::microbenchmark(
#   json = {
#     res <- jsonify:::rcpp_to_json( data )
#   },
#   ndjson = {
#     res <- jsonify:::rcpp_to_ndjson( data )
#   },
#   times = 5
# )
# 
# # Unit: seconds
# # expr      min       lq     mean   median       uq      max neval
# # json 1.314854 1.318062 1.351673 1.336339 1.372355 1.416754     5
# # ndjson 1.480732 1.481594 1.502879 1.483283 1.485288 1.583497     5
# 
# res <- jsonify:::rcpp_to_ndjson( data )
# str( res )
# # chr "{\"longitude\":18.95450374111533,\"latitude\":-76.69491718523205,\"tooltip\":\"X\"}\n{\"longitude\":138.2996058"| __truncated__
# 
# 
# 
# lst <- list(
#   x = 1:5
#   , y = list(
#     a = letters[1:5]
#     , b = data.frame(i = 10:15, j = 20:25)
#   )
# )
# 
# lst
# 
# jsonlite::stream_out( lst )
# 
# 
# 
# 
# 
