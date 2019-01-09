context("source code")

test_that("source tests run", {
  
  expect_silent( jsonify:::source_tests() )
  
})