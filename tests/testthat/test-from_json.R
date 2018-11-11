context("from_json")

test_that("scalar values handled properly", {
  target <- list(a = 8L, b = 99.5, c = TRUE, d = "cats")
  
  json_str <- "{\"a\":8, \"b\":99.5, \"c\":true, \"d\":\"cats\"}"
  expect_equal(from_json(json_str), target)
  
  json_str <- jsonify::to_json(target)
  expect_equal(from_json(json_str), target)
})

test_that("vector / array values handled properly", {
  target <- list(a = c(1L, 2L, 3L), b = list(1L, "cats", 3L))
  
  json_str <- "{\"a\":[1, 2, 3], \"b\":[1, \"cats\", 3]}"
  expect_equal(from_json(json_str), target)
  
  json_str <- jsonify::to_json(target)
  expect_equal(from_json(json_str), target)
})

test_that("nested JSON handled properly", {
  target <- list(a = 8, b = list(c = 123, d = list(e = 456)))
  
  json_str <- "{\"a\":8, \"b\":{\"c\":123, \"d\":{\"e\":456}}}"
  expect_equal(from_json(json_str), target)
  
  json_str <- jsonify::to_json(target)
  expect_equal(from_json(json_str), target)
})

test_that("JSON missing keys handled properly", {
  # vector
  target <- c(1L, 2L, 3L)
  
  json_str <- "[1, 2, 3]"
  expect_equal(from_json(json_str), target)
  
  json_str <- jsonify::to_json(target)
  expect_equal(from_json(json_str), target)
  
  # list
  target <- list(1L, "cats", 3L)
  
  json_str <- "[1, \"cats\", 3]"
  expect_equal(from_json(json_str), target)
  
  json_str <- jsonify::to_json(target)
  expect_equal(from_json(json_str), target)
})
