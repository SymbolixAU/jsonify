context("vectors")

test_that("different vector types work", {
  
  expect_equal(to_json(c(1L:5L)), "[1,2,3,4,5]")                         # integer
  expect_equal(to_json(c(1.0,2.1,3.2)), "[1.0,2.1,3.2]")                 # numeric
  expect_equal(to_json(letters[1:5]), "[\"a\",\"b\",\"c\",\"d\",\"e\"]") # character
  
  
  expect_error(to_json(complex(1)), "this type is not supported")
  
})