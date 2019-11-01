# 
# 
# library(Rcpp)
# 
# cppFunction('Rcpp::List na_list(){
# 
#   Rcpp::List lst(3);
#   
#   for( int i = 0; i < 3; i++ ) {
#     lst[i] = NA_LOGICAL;
#   }
#   
#   return lst;
# 
# }')
# 
# na_list()
# 
# # [[1]]
# # [1] NA
# # 
# # [[2]]
# # [1] NA
# # 
# # [[3]]
# # [1] NA
# 
# 
# 
# 
# 
# js <- '[{"x":1,"y":1},{"y":"hello"},{"z":true}]'
# res <- from_json( js, fill_na = TRUE )
# res
# #    x     y    z
# # 1  1     1   NA
# # 2 NA hello   NA
# # 3 NA  <NA> TRUE
# 
# str( res )
# # 'data.frame':	3 obs. of  3 variables:
# # $ x: int  1 NA NA
# # $ y: chr  "1" "hello" NA
# # $ z: logi  NA NA TRUE
# 
# js <- '[{"x":1,"y":[1,2]},{"x":1,"y":[1,3,3]},{"x":3}]'
# res <- from_json( js, fill_na = TRUE )
# res
# #   x       y
# # 1 1    1, 2
# # 2 1 1, 3, 3
# # 3 3      NA
# 
# 
# str( res )
# # 'data.frame':	2 obs. of  2 variables:
# # $ x: int  1 1
# # $ y:List of 2
# # ..$ : int  1 2
# # ..$ : int  1 3 3
# # ..$ : int NA
# 
# js <- '[{"x":1,"y":[1,2]},{"":1,"y":[1,3,3]},{"x":3}]'
# res <- from_json( js, fill_na = TRUE )
# res
# 
# js <- '[{"x":1,"y":[1,2]},{"x":{},"y":[1,3,3]},{"x":3},{"z":{}}]'
# res <- from_json( js, fill_na = T )
# res
# #from_json( to_json( res, unbox = T ) )
# 
# js <- '[{"x":1,"y":[1,2]},{"x":{},"y":[1,3,3],"z":3},{"x":3},{"z":{}}]'
# res <- from_json( js, fill_na = T )
# res
# to_json( res, unbox = T )
# 
# 
# 
# js <- '[{"x":1,"y":[1,2,3]},{"x":{},"y":[1,3,3],"z":3},{"x":3},{"z":{}},{"x":1,"y":[1,3,3],"z":"yo"}]'
# res <- from_json( js, fill_na = T )
# res
# to_json( res, unbox = T )
# 
# 
# js <- '[{"x":1,"y":[1,2,3]},{"x":{},"y":[1,3,3]},{"x":1}]'
# res <- from_json( js, fill_na = T )
# res
# ## 'y' shouldn't be a matrix column because the last row is missing
# ## but instead it's filled with overflowed stuff
# to_json( res, unbox = T )
# 
# js <- '[{"x":1,"y":[1,2,3]},{"x":{},"y":[1,3,3]},{"x":1}]'
# res <- from_json( js, fill_na = F )
# res
# to_json( res, unbox = T )
# 
# js <- '[{"x":1,"y":[1,2,3]},{"x":{},"y":[1,3,3]}]'
# res <- from_json( js, fill_na = F )
# res; str(res)
# to_json( res, unbox = T )
# 
# js <- '[{"x":1,"y":[1,2,3]},{"x":{},"y":[1,3,3]}]'
# res <- from_json( js, fill_na = T )
# res
# 
# str( res )
# # 'data.frame':	2 obs. of  2 variables:
# #   $ x:List of 2
# # ..$ : int 1
# # ..$ : NULL
# # $ y:List of 2
# # ..$ : int  1 2 3
# # ..$ : int  1 3 3
# 
# 
# to_json( res, unbox = T )
# 
# 
# 
