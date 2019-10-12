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
# from_json( js )
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
# df <- data.frame(
#   x = 1:3
#   , y = I(c(list(1:3), list(4:6), list(7:10)))
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
# res <- from_json( js ); res
# ## type 9, length 4
# ## - int(4)
# 
# x <- matrix(1:9, ncol = 3)
# js <- to_json( x )
# js
# jsonify:::rcpp_get_dtypes( js )
# res <- from_json( js ); res
# ## type 4, length 3
# ## - array == matrix( row = 3, col = ?? )
# 
# x <- matrix(c(1.1, 2:9), ncol = 3)
# js <- to_json( x )
# js
# jsonify:::rcpp_get_dtypes( js )
# res <- from_json( js ); res
# ## type 4, length 3
# ## - array == matrix( row = 3, col = ?? )
# 
# x <- matrix(sample(c(T,F), size = 9, replace = T), ncol = 3)
# js <- to_json( x )
# js
# jsonify:::rcpp_get_dtypes( js )
# res <- from_json( js ); res
# ## type 4, length 3
# ## - array == matrix( row = 3, col = ?? )
# 
# x <- matrix(letters[1:9], ncol = 3)
# js <- to_json( x )
# js
# jsonify:::rcpp_get_dtypes( js )
# res <- from_json( js ); res
# ## type 4, length 3
# ## - array == matrix( row = 3, col = ?? )
# 
# ## TODO
# ## - the second element isn't named, so don't return a name?
# x <- matrix( letters[1:9], ncol = 3)
# l <- list( x = 1:5, x )
# js <- to_json( l )
# from_json( js )
# l
# 
# x <- matrix(1:12, ncol = 3)
# js <- to_json( x )
# js
# jsonify:::rcpp_get_dtypes( js )
# res <- from_json( js ); res
# # type 4, length 4
# # - array == matrix( row = 4, col = ?? )
# 
# x <- data.frame(x = 1:4, y = 4:7)
# js <- to_json( x )
# js
# jsonify:::rcpp_get_dtypes( js )
# res <- from_json( js ); res
# # type 3, length 4
# # - object == data.frame( row = 4 , col = ?? )
# 
# ## TODO
# # data.frame with the last value as NA needs to also simplifyi to data.frame
# x <- data.frame(x = 1:2, y = c(1, NA))
# js <- to_json( x )
# from_json( js )
# 
# x <- data.frame(x = 1:2, y = I(matrix(1:4, ncol = 2)))
# js <- to_json( x )
# js
# jsonify:::rcpp_get_dtypes( js )
# res <- from_json( js ); res
# ## type 3, length 2
# ## - object == data.frame( row = 2, col = ?? )
# 
# l <- list( 1:4, v = 5:8, df = data.frame(id = 1:2, y = 3:4), m = matrix(1:12, ncol = 3), l = list( 1:4, v = 5:8, df = data.frame( id = 1:2, y = 3:4), m = matrix(1:12, ncol = 3)))
# js <- to_json( l )
# js; from_json( js )
# 
# l <- list( df = data.frame(id = 1:2, y = letters[1:2]), m = matrix(1:12, ncol = 3))
# js <- to_json( l )
# js; from_json( js )
# 
# x <- data.frame( x = 1:2, y = list(1:2, 3:4) )
# js <- to_json( x )
# js; from_json( js )


# x <- data.frame(x = 1:2, y = I(matrix(1:4, ncol = 2)))
# js <- to_json( x, by = "col" )
# js; from_json( js )

