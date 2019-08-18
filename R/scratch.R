# 
# 
# library(jsonify)
# 
# lst <- list(
#   x = data.frame(id = 1:5, val = letters[1:5])
#   , y = 6:10
#   , z = matrix(1:9, ncol = 3)
# )
# 
# js <- to_json( lst )
# 
# from_json( js, simplifyDataFrame = F )
# 
# 
# df <- data.frame(
#   x = 1:3
#   , y = I(c(list(1:3), list(4:6), list(7:9)))
# )
# 
# js <- to_json( df )
# res <- from_json( js )
# 
# 
# df <- data.frame(
#   x = 1:3
#   , y = I(c(list(1:3), list(4:6), list(7:9)))
# )
# 
# lst <- list(
#   x = df
#   , y = 1:4
# )
# 
# js <- to_json( lst )
# res <- from_json( js )
# 
# x <- 1:4
# js <- to_json( x )
# js
# jsonify:::rcpp_get_dtypes( js )
# res <- from_json( js )
# ## type 9, length 4
# ## - int(4)
# 
# x <- matrix(1:9, ncol = 3)
# js <- to_json( x )
# js
# jsonify:::rcpp_get_dtypes( js )
# res <- from_json( js )
# ## type 4, length 3
# ## - array == matrix( row = 3, col = ?? )
# 
# x <- matrix(1:12, ncol = 3)
# js <- to_json( x )
# js
# jsonify:::rcpp_get_dtypes( js )
# res <- from_json( js )
# ## type 4, length 4
# ## - array == matrix( row = 4, col = ?? )
# 
# x <- data.frame(x = 1:4, y = 4:7)
# js <- to_json( x )
# js
# jsonify:::rcpp_get_dtypes( js )
# res <- from_json( js )
# ## type 3, length 4
# ## - object == data.frame( row = 4 , col = ?? )
# 
# x <- list(1:4)
# js <- to_json( x )
# js
# jsonify:::rcpp_get_dtypes( js )
# res <- from_json( js )
# ## type 4, length 1
# ## - array == vector, matrix or list??
# 
# x <- data.frame(x = 1:2, y = I(matrix(1:4, ncol = 2)))
# js <- to_json( x )
# js
# jsonify:::rcpp_get_dtypes( js )
# res <- from_json( js )
# ## type 3, length 2
# ## - object == data.frame( row = 2, col = ?? )
# 
# x <- data.frame(x = 1:2, y = I(matrix(1:4, ncol = 2)))
# js <- to_json( x, by = "col" )
# js
# jsonify:::rcpp_get_dtypes( js )
# res <- from_json( js )
# 
# 
# 
# 
