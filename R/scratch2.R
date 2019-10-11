# 
# js <- '{"id":1,"val":2}'
# from_json( js )
# 
# js <- '[{"id":1,"val":2}]'
# from_json( js )
# 
# js <- '[{"id":1,"val":2},{"id":1}]'
# from_json( js )
# 
# js <- '[{"id":1,"val":1},{"id":2,"val":2}]'
# from_json( js )
# 
# js <- '[{"id":1,"val":1},{"id":2,"col":2}]'
# from_json( js )
# 
# js <- '[{"id":1,"val":1},{"id":2,"val":2,"col3":2}]'
# from_json( js )
# 
# js <- '[{"id":1,"val":2},{"id":1,"val":[1,2,3]}]'
# from_json( js )
# 
# js <- '[{"id":1,"val":2},{"id":1,"val":[1,2,3,{"inner_id":1}]}]'
# from_json( js )
# jsonlite::fromJSON( js )
# 
# js <- '[{"id":1,"val":2},{"id":1,"val":[1,2,3,{"inner_id":1}]}]'
# from_json( js )
# jsonlite::fromJSON( js )
# 
# js <- '[[1,2],[3,4],[5,6]]'
# from_json( js )
# jsonlite::fromJSON( js )
# 
# js <- '[[[1,2],[3,4],[5,6]]]'
# from_json( js )
# jsonlite::fromJSON( js )
# 
# js <- '[[[1,1],[1,2]],[1,3]]'
# res <- from_json( js )
# str( res )
# 
# js <- '[[1,1],[[1,2],[1,3]]]'
# res <- from_json( js )
# str( res )
# 
# js <- '[[[1,1],[1,"b"]],[1,3]]'
# res <- from_json( js )
# str( res )
# 
# res <- jsonlite::fromJSON( js )
# str( res )
# 
# js <- '[[[1,1],3,[1,2]],[1,3]]'
# res <- from_json( js ); res
# str( res )
# 
# res <- jsonlite::fromJSON( js ); res
# str( res )
# 
# 
# js <- '[[[1,1],[1,[3,4]]],[1,3]]'
# res <- from_json( js )
# str( res )
# 
# res <- jsonlite::fromJSON( js )
# str( res )
# 
# from_json( '[]' )
# from_json( '{}' )
# 
# js <- '[{},[]]'
# from_json( js )
# 
# js <- '[[[1,1],3,[1,2]],[1,3],{},[{},{}]]'
# res <- from_json( js ); res
# 
# js <- '{{}}'
# from_json( js )
# 
# js <- '[{{}}]'
# from_json( js )
# 
# js <- '[{"id":{"val":1}}]'
# from_json( js )
# 
# ## df with matrix column
# df <- structure(list(fill_colour = structure(c(68, 49, 53, 253, 1,
# 104, 183, 231, 84, 142, 121, 37, 255, 255, 255, 255), .Dim = c(4L,
# 4L)), geometry = c(1, 2, -5, 0.3), lat = 1:4, lon = c(1, 2, -5,
# 0.3)), class = "data.frame", row.names = c(NA, 4L))
# 
# js <- to_json( df, by = "row" )
# 
# str( df )
# str( from_json( js ) )
# str( jsonlite::fromJSON( js ) )
# 
# 
# df$geometry <- NULL
# df$lat <- NULL
# df$lon <- NULL 
# 
# js <- to_json( df )
# from_json( js )
# 
# 
# x <- rnorm(1e3)
# y <- rnorm(1e3)
# z <- rnorm(1e3)
# m <- matrix(rnorm(1e3), ncol = 2)
# l <- list(x, m, list(y, list(z)))
# #l <- list(m)
# js <- to_json( l )
# 
# jfy <- from_json( js )
# jlt <- jsonlite::fromJSON( js )
# 
# str( jfy )
# str( jlt )
# 
# library(microbenchmark)
# 
# microbenchmark(
#   jfy = {
#     jsonify::from_json( js )
#   },
#   jlt = {
#     jsonlite::fromJSON( js )
#   },
#   times = 2
# )
