context("simplify")

test_that("simplify types",{
  
  ## tests function only works for arrays
  
  ## mapping
  # 0 - null
  # 1 - false
  # 2 - true
  # 3 - object
  # 4 - array
  # 5 - string
  # 6 - number 
  #   - 8 - double
  #   - 9 - int
  
  x <- 1:4
  js <- to_json( x )
  #js
  types <- jsonify:::rcpp_get_dtypes( js )
  expect_equal( types, 9 )
  ## type 9, length 4
  ## - int(4)
  
  x <- matrix(1:9, ncol = 3)
  js <- to_json( x )
  types <- jsonify:::rcpp_get_dtypes( js )
  expect_equal( types, 4 )
  ## type 4, length 3
  ## - array == matrix( row = 3, col = ?? )
  
  x <- matrix(1:12, ncol = 3)
  js <- to_json( x )
  types <- jsonify:::rcpp_get_dtypes( js )
  expect_equal( types, 4 )
  ## type 4, length 4
  ## - array == matrix( row = 4, col = ?? )
  
  x <- data.frame(x = 1:2, y = 3:4)
  js <- to_json( x )
  #js
  types <- jsonify:::rcpp_get_dtypes( js )
  expect_equal( types, 3 )
  ## type 3, length 4
  ## - object == data.frame( row = 4 , col = ?? )
  
  x <- list(1:4)
  js <- to_json( x )
  #js
  types <- jsonify:::rcpp_get_dtypes( js )
  expect_equal( types, 4 )
  ## type 4, length 1
  ## - array == vector, matrix or list??
  
  x <- data.frame(x = 1:2, y = I(matrix(1:4, ncol = 2)))
  js <- to_json( x )
  types <- jsonify:::rcpp_get_dtypes( js )
  expect_equal( types, 3 )
  ## type 3, length 2
  ## - object == data.frame( row = 2, col = ?? )
  
  ## won't work because it's not an array
  x <- data.frame(x = 1:2, y = I(matrix(1:4, ncol = 2)))
  js <- to_json( x, by = "col" )
  types <- jsonify:::rcpp_get_dtypes( js )
  expect_equal( types, integer() );

  
})