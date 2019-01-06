context("columns")

test_that("jsonified as columns", {
  
  ## vectors (remain unchanged)
  ## int matrix
  x <- 1L:2L
  js <- to_json( x, by = "column" )
  expect_equal( as.character( js ), '[1,2]')
  expect_true( validate_json( js ) )
  ## num matrix
  x <- c(1.1,2)
  js <- to_json( x, by = "column" )
  expect_equal( as.character( js ), '[1.1,2.0]')
  expect_true( validate_json( js ) )
  ## log matrix
  x <- matrix(c(T,F,T,F), ncol = 2)
  js <- to_json( x, by = "column" )
  expect_equal( as.character( js ), '[[true,false],[true,false]]')
  expect_true( validate_json( js ) )
  ## str matrix
  x <- matrix(letters[1:4], ncol = 2)
  js <- to_json( x, by = "column" )
  expect_equal( as.character( js ), '[["a","b"],["c","d"]]')
  expect_true( validate_json( js ) )
  ## data.frame
  x <- data.frame(id = 1:4, val = letters[1:4], stringsAsFactors = F)
  js <- to_json( x, by = "column" )
  expect_equal( as.character( js ), '{"id":[1,2,3,4],"val":["a","b","c","d"]}' )
  expect_true( validate_json( js ) )
  ## list of stuff
  x <- list(m = matrix(1:4, ncol = 2), df = data.frame(id = 1:4, val = letters[1:4], stringsAsFactors = FALSE))
  js <- to_json( x, by = "column" )
  expect_equal( as.character( js ), '{"m":[[1,2],[3,4]],"df":{"id":[1,2,3,4],"val":["a","b","c","d"]}}')
  expect_true( validate_json( js ) )
})
