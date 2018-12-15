context("digits")

test_that("results rounded",{
  
  ## VECTORS
  x <- 1.234567
  js <- to_json( x, digits = 1 )
  expect_equal( as.character(js), '[1.2]')
  
  js <- to_json( x, digits = 2 )
  expect_equal( as.character(js), '[1.23]')
  
  js <- to_json( x, digits = 20 )
  expect_equal( as.character(js), '[1.234567]')
  
  ## MATRIX
  m <- matrix(1.234567)
  js <- to_json( m, unbox = T, digits = 1)
  expect_equal( as.character( js ), '[1.2]')
  
  js <- to_json( m, unbox = T, digits = 2)
  expect_equal( as.character( js ), '[1.23]')
  
  js <- to_json( m, unbox = T, digits = NULL)
  expect_equal( as.character( js ), '[1.234567]')
  
  ## DataFrame
  df <- data.frame(x = c("a","b"), y = c(1.23456789, 9.87654321), stringsAsFactors = FALSE)
  js <- to_json( df, digits = 1 )
  expect_equal( as.character( js ), '[{"x":"a","y":1.2},{"x":"b","y":9.9}]')
  
  
})