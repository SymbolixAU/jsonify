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
  
  x <- matrix(1:9, ncol = 3)
  js <- to_json( x )
  types <- jsonify:::rcpp_get_dtypes( js )
  expect_equal( types, 4 )
  
  x <- data.frame(x = 1:2, y = 3:4)
  js <- to_json( x )
  #js
  types <- jsonify:::rcpp_get_dtypes( js )
  expect_equal( types, 3 )
  
  x <- list(1:4)
  js <- to_json( x )
  #js
  types <- jsonify:::rcpp_get_dtypes( js )
  expect_equal( types, 4 )
  
  x <- data.frame(x = 1:2, y = I(matrix(1:4, ncol = 2)))
  js <- to_json( x )
  types <- jsonify:::rcpp_get_dtypes( js )
  expect_equal( types, 3 )
  
  ## won't work because it's not an array
  x <- data.frame(x = 1:2, y = I(matrix(1:4, ncol = 2)))
  js <- to_json( x, by = "col" )
  types <- jsonify:::rcpp_get_dtypes( js )
  expect_equal( types, integer() );

  
})