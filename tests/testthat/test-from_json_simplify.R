context("simplify")

test_that("R Lists simplified to vectors",{
  
  l <- list(1,2,3)
  res <- jsonify:::rcpp_simplify_vector( l, 13, 1)
  expect_equal( res, 1:3 )
  
  l <- list(1,2,3:4)
  # expect_error(
  #   jsonify:::rcpp_simplify_vector( l, 13, 2)
  #   , "jsonify - list elements different sizes"
  # )
  
  l <- list( 1:2, 3:4, 5:6 )
  res <- jsonify:::rcpp_simplify_vector( l, 13, 2 )
  expect_equal( res, 1:6 )
  
  l <- list( 1.1, 2.2, 3.3 )
  res <- jsonify:::rcpp_simplify_vector( l, 14, 1 )
  expect_equal( res, unlist( l ) )
  
  
  l <- list( letters, letters, letters )
  res <- jsonify:::rcpp_simplify_vector( l, 16, 26 )
  expect_equal( res, rep( letters, 3 ) )
  
  ## mix of types
  l <- list(1,2,"a")
  res <- jsonify:::rcpp_simplify_vector( l, 16, 1)
  expect_equal( res, c("1","2","a") )
  
  
})


test_that("simplify types",{
  
  x <- 1:4
  js <- to_json( x )
  #js
  types <- jsonify:::rcpp_get_dtypes( js )
  expect_equal( types, 8 )
  ## type 8, length 4
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
  
  x <- data.frame(x = 1:2, y = 3:4, stringsAsFactors = TRUE)
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
  
  x <- data.frame(x = 1:2, y = I(matrix(1:4, ncol = 2)), stringsAsFactors = TRUE)
  js <- to_json( x )
  types <- jsonify:::rcpp_get_dtypes( js )
  expect_equal( types, 3 )
  ## type 3, length 2
  ## - object == data.frame( row = 2, col = ?? )
  
  ## won't work because it's not an array
  x <- data.frame(x = 1:2, y = I(matrix(1:4, ncol = 2)), stringsAsFactors = TRUE)
  js <- to_json( x, by = "col" )
  types <- jsonify:::rcpp_get_dtypes( js )
  expect_equal( types, integer() );

  
})


test_that("arrays to vectors",{
  
  round_trip <- function(x) {
    testthat::expect_equal( from_json( to_json( x ) ), x )
  }
  round_trip( 1L )
  round_trip( 1L:3L )
  round_trip( 1.1 )
  round_trip( c(1.1, 1,3, 3,3 ) )
  round_trip( letters )
})

test_that("arrays of mixed types to highest type" ,{
  
  js <- '[1,2,"a"]'
  x <- from_json( js )
  expect_equal(c(1,2,"a"), x)
  
})

test_that("arrays of arrays of same length go to matrix",{
  
  js <- '[[1,2],[3,4],[5,6]]'
  x <- from_json( js )
  expect_equal(x, matrix(1:6, ncol = 2, byrow = T ) )
  
})

test_that("array of arrays of different lenghts go to list",{
  
  ## issue 67
  js <- '[1,[2]]'
  x <- from_json( js )
  expect_equal(x, list(1,2))
  
  js <- '{"test":[1,[2,[3]]]}'
  x <- from_json( js )
  expect_equal(x, list(test = list(1,list(2,3))))
  
  x <- from_json( js, simplify = FALSE )
  expect_equal(x, list(test = list(1,list(2,list(3)))))
  
  js <- '[[1,2],[1,2,3]]'
  x <- from_json( js )
  expect_equal( x, list(1:2, 1:3))

  js <- '[[5,[6,7]]]'
  x <- from_json( js )
  expect_equal( x, list( list( 5, 6:7 )  ) ) 
  
  js <- '[[5,[6,"a"]]]'
  x <- from_json( js )
  expect_equal( x, list( list( 5, c("6","a") )  ) )
  
  js <- '[[1,2],[3,4],[5,[6,7]]]'
  x <- from_json( js )
  expect_equal( x, list( c(1,2), c(3,4), list(5, 6:7) ) )
})

test_that("array of various types converted to matrices",{
  
  js <- '[[1,2],[3,4]]'
  x <- from_json( js )
  expect_equal(x, matrix(1L:4L, ncol = 2, byrow = T ) )
  
  js <- '[[1.1,2],[3,4]]'
  x <- from_json( js )
  expect_equal(x, matrix(c(1.1,2,3,4), ncol = 2, byrow = T ) )
})

