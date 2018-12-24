context("digits")

test_that("digits argument in R handled", {
  
  expect_equal( jsonify:::handle_digits( NULL ), -1 )
  expect_equal( jsonify:::handle_digits( 1 ), 1 )
  expect_equal( jsonify:::handle_digits( 1L ), 1 )
  expect_equal( jsonify:::handle_digits( 1.001 ), 1 )
  expect_equal( jsonify:::handle_digits( 1.99 ), 1 )
  
})

test_that("results rounded",{
  
  ## VECTORS
  x <- 1.234567
  js <- to_json( x, digits = 1 )
  expect_true( validate_json( js ) )
  expect_equal( as.character(js), '[1.2]')
  
  js <- to_json( x, digits = 2 )
  expect_true( validate_json( js ) )
  expect_equal( as.character(js), '[1.23]')
  
  js <- to_json( x, digits = 20 )
  expect_true( validate_json( js ) )
  expect_equal( as.character(js), '[1.234567]')
  
  ## MATRIX
  m <- matrix(1.234567)
  js <- to_json( m, unbox = T, digits = 1)
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), '[1.2]')
  
  js <- to_json( m, unbox = T, digits = 2)
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), '[1.23]')
  
  js <- to_json( m, unbox = T, digits = NULL)
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), '[1.234567]')
  
  ## DataFrame
  df <- data.frame(x = c("a","b"), y = c(1.23456789, 9.87654321), stringsAsFactors = FALSE)
  js <- to_json( df, digits = 1 )
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), '[{"x":"a","y":1.2},{"x":"b","y":9.9}]')
  
  
  ## List
  lst <- list(
    x = c(1.23456798,1.987654321)
    , y = list(x = 1.23456789, z = c(9.87654321, 1000.1))
    , m = matrix(1000.123456789)
    , i = 1L
    )
  js <- to_json( lst, unbox = TRUE, digits = 0)
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), '{"x":[1.0,2.0],"y":{"x":1.0,"z":[10.0,1000.0]},"m":[1000.0],"i":1}')
  
  js <- to_json( lst, unbox = TRUE, digits = 1)
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), '{"x":[1.2,2.0],"y":{"x":1.2,"z":[9.9,1000.1]},"m":[1000.1],"i":1}')
  
  js <- to_json( lst, unbox = TRUE, digits = 4)
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), '{"x":[1.2346,1.9877],"y":{"x":1.2346,"z":[9.8765,1000.1]},"m":[1000.1235],"i":1}')
  
})