context("from_json")

test_that("scalar values handled properly", {
  target <- list(a = 8L, b = 99.5, c = TRUE, d = "cats", e = NA_character_)
  
  json_str <- "{\"a\":8, \"b\":99.5, \"c\":true, \"d\":\"cats\", \"e\":null}"
  expect_equal(from_json(json_str), target)
  
  json_str <- jsonify::to_json(target)
  expect_equal(from_json(json_str), target)
  
  expect_equal(from_json("1"), 1)
  expect_equal(from_json("1.5"), 1.5)
  expect_equal(from_json("\"a\""), "a")
  expect_equal(from_json("true"), TRUE)
})

test_that("vector / array values handled properly", {
  target <- list(a = c(1L, 2L, 3L, NA_integer_), b = list(1L, "cats", 3L, NA_character_))
  
  json_str <- "{\"a\":[1, 2, 3, null], \"b\":[1, \"cats\", 3, null]}"
  expect_equal(from_json(json_str), target)
  
  json_str <- jsonify::to_json(target)
  expect_equal(from_json(json_str), target)
})

test_that("nested JSON handled properly", {
  target <- list(a = 8, b = list(c = 123, d = list(e = 456, f = NA_character_)))
  
  json_str <- "{\"a\":8, \"b\":{\"c\":123, \"d\":{\"e\":456, \"f\":null}}}"
  expect_equal(from_json(json_str), target)
  
  json_str <- jsonify::to_json(target)
  expect_equal(from_json(json_str), target)
})

test_that("nested JSON within an array handled properly", {
  target <- list(a = 8L, b = list(list(c = 123L, d = 456L), list(e = list(list(f = "cats")))))
  
  json_str <- "{\"a\":8, \"b\":[{\"c\":123, \"d\":456}, {\"e\":[{\"f\":\"cats\"}]}]}"
  expect_equal(from_json(json_str), target)
  
  json_str <- jsonify::to_json(target)
  expect_equal(from_json(json_str), target)
})

test_that("JSON missing keys handled properly", {
  # vector
  target <- c(1L, 2L, 3L, NA_integer_)
  
  json_str <- "[1, 2, 3, null]"
  expect_equal(from_json(json_str), target)
  
  json_str <- jsonify::to_json(target)
  expect_equal(from_json(json_str), target)
  
  # list
  target <- list(1L, "cats", 3L, NA_character_)
  
  json_str <- "[1, \"cats\", 3, null]"
  expect_equal(from_json(json_str), target)
  
  json_str <- jsonify::to_json(target)
  expect_equal(from_json(json_str), target)
})

test_that("data frame returned properly", {
  target <- data.frame("id" = c(1L, 2L), "val" = c("a", "b"), stringsAsFactors = FALSE)
  
  json_str <- '[{"id":1,"val":"a"},{"id":2,"val":"b"}]'
  expect_equal(from_json(json_str), target)
  
  json_str <- jsonify::to_json(target)
  expect_equal(from_json(json_str), target)
})

test_that("round trips work", {
  
  f1 <- function( x ) from_json( to_json( x ) )
  f2 <- function( x ) from_json( to_json( x, unbox = TRUE ) )
  
  x <- 1L
  expect_identical( f1(x), x )
  expect_identical( f2(x), x )
  
  x <- 1
  expect_identical( f1(x), x )
  expect_identical( f2(x), x )
  
  x <- 1L:5L
  expect_identical( f1(x), x )
  expect_identical( f2(x), x )
  
  x <- 1:5
  expect_identical( f1(x), x )
  expect_identical( f2(x), x )
  
  x <- c(1L, 2L, 3L, NA_integer_)
  expect_identical( f1(x), x )
  expect_identical( f2(x), x )
  
  x <- list(x = 1)
  expect_identical( f1(x), x )
  expect_identical( f2(x), x )
  
  x <- list(x = 1:5)
  expect_identical( f1(x), x )
  expect_identical( f2(x), x )
  
  x <- list(x = 1, y = letters)
  expect_identical( f1(x), x )
  expect_identical( f2(x), x )
  
  x <- list(1L, "cats", 3L, NA_character_)
  expect_identical( f1(x), x )
  expect_identical( f2(x), x )
  
  x <- data.frame(
    "col1" = c(1, 2), 
    "col2" = c(1L, 2L), 
    "col3" = c(TRUE, FALSE), 
    "col4" = c("cats", "dogs"), 
    stringsAsFactors = FALSE
  )
  expect_identical( f1(x), x )
  
  
})






