context("from_json")

test_that("scalar values handled properly", {
  target <- list(a = 8L, b = 99.5, c = TRUE, d = "cats", e = NA)

  js <- "{\"a\":8, \"b\":99.5, \"c\":true, \"d\":\"cats\", \"e\":null}"
  x <- from_json(js)
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  x <- from_json(js)
  expect_equal(x, target)

  expect_equal(from_json("1"), 1)
  expect_equal(from_json("1.5"), 1.5)
  expect_equal(from_json("\"a\""), "a")
  expect_equal(from_json("true"), TRUE)
})

test_that("vector / array values handled properly", {
  
  target <- list(a = c(1L, 2L, 3L, NA_integer_), b = list(1L, "cats", 3L, NA))
  js <- "{\"a\":[1, 2, 3, null], \"b\":[1, \"cats\", 3, null]}"
  x <- from_json(js, simplify = FALSE)
  expect_equal(x, target)
  
  target <- list(a = c(1L, 2L, 3L, NA_integer_), b = c("1", "cats","3", NA))
  js <- "{\"a\":[1, 2, 3, null], \"b\":[1, \"cats\", 3, null]}"
  x <- from_json(js, simplify = TRUE)
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)
})

test_that("nested JSON handled properly", {
  target <- list(a = 8, b = list(c = 123, d = list(e = 456, f = NA)))

  js <- "{\"a\":8, \"b\":{\"c\":123, \"d\":{\"e\":456, \"f\":null}}}"
  x <- from_json( js )
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  x <- from_json( js )
  expect_equal(x, target)
})

test_that("nested JSON within an array handled properly", {
  
  target <- list(a = 8L, b = list(list(c = 123L, d = 456L), list(e = data.frame(f = "cats", stringsAsFactors = F))))

  js <- "{\"a\":8, \"b\":[{\"c\":123, \"d\":456}, {\"e\":[{\"f\":\"cats\"}]}]}"
  x <- from_json(js)
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)
})

test_that("JSON missing keys handled properly", {
  # vector
  target <- c(1L, 2L, 3L, NA_integer_)

  js <- "[1, 2, 3, null]"
  x <- from_json(js)
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  x <- from_json(js)
  expect_equal(x, target)

  # list
  target <- list(1L, "cats", 3L, NA)

  js <- "[1, \"cats\", 3, null]"
  x <- from_json(js, simplify = FALSE )
  expect_equal(x, target)
  
  target <- c("1","cats","3",NA)
  x <- from_json(js, simplify = TRUE )
  expect_equal(x, target)

})

test_that("data frame returned properly", {
  # Return data frame in which the values in each name are of the same data type.
  target <- data.frame("id" = c(1L, 2L), "val" = c("a", "b"), stringsAsFactors = FALSE)

  js <- '[{"id":1,"val":"a"},{"id":2,"val":"b"}]'
  x <- from_json( js )
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)
  
  js <- '[{"id":1,"val":"a","val2":1},{"id":2,"val":"b"}]'
  x <- from_json( js )
  expect_equal(x, target)
  
  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)
  
  ## two entries with same key
  js <- '[{"id":1,"val":"a","val":1},{"id":2,"val":"b"}]'
  x <- from_json( js )
  expect_equal(x, target)
  
  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)

  # Return data frame in which the values in each name are NOT of the same data type.
  target <- data.frame("id" = c("cats", 2L), "val" = c("a", "b"), stringsAsFactors = FALSE)

  js <- '[{"id":"cats","val":"a"},{"id":2,"val":"b"}]'
  x <- from_json( js )
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)

  # Return data frame in which the names do not align across JSON objects.
  target <- data.frame("id" = c(1L, 2L), "val" = c("a", NA), "blah" = c(NA, "b"), stringsAsFactors = FALSE)

  js <- '[{"id":"1","val":"a"},{"id":"2","blah":"b"}]'
  x <- from_json(js)
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)
  
  
  
  js <- '[{"id":"1","val":"a"},{"id":"2","blah":[1,2]}]'
  x <- from_json(js)
  expect_equal(x, target)
  
  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)
  
  
  ## now loads of complex stuff
  df1 <- data.frame(
    x = 1:2
    , y = 3:4
  )
  df2 <- data.frame(
    z = c("a","b")
    , stringsAsFactors = F
  )
  
  df1$z <- df2
  
  js <- to_json( df1 )
  from_json( js )
  
  # 
  # # Return data frame in which the lengths of the input values are different.
  # target <- data.frame(matrix(nrow = 2, ncol = 2), stringsAsFactors = FALSE)
  # colnames(target) <- c("id", "val")
  # target[["id"]] <- list("a", NA)
  # target[["val"]] <- list(NA, c(1L, 2L, 3L, 4L))
  # 
  # js <- '[{"id":"a"},{"val":[1,2,3,4]}]'
  # expect_equal(from_json(js), target)
  # 
  # js <- jsonify::to_json(target)
  # expect_equal(from_json(js), target)
})
# 
# test_that("empty inputs return NULL", {
#   expect_null(from_json("{}"))
#   expect_null(from_json("[]"))
# })
# 
# test_that("round trips work", {
#   
#   f1 <- function( x ) from_json( to_json( x ) )
#   f2 <- function( x ) from_json( to_json( x, unbox = TRUE ) )
#   
#   x <- 1L
#   expect_identical( f1(x), x )
#   expect_identical( f2(x), x )
#   
#   x <- 1
#   expect_identical( f1(x), x )
#   expect_identical( f2(x), x )
#   
#   x <- 1L:5L
#   expect_identical( f1(x), x )
#   expect_identical( f2(x), x )
#   
#   x <- 1:5
#   expect_identical( f1(x), x )
#   expect_identical( f2(x), x )
#   
#   x <- c(1L, 2L, 3L, NA_integer_)
#   expect_identical( f1(x), x )
#   expect_identical( f2(x), x )
#   
#   x <- list(x = 1)
#   expect_identical( f1(x), x )
#   expect_identical( f2(x), x )
#   
#   x <- list(x = 1:5)
#   expect_identical( f1(x), x )
#   expect_identical( f2(x), x )
#   
#   x <- list(x = 1, y = letters)
#   expect_identical( f1(x), x )
#   expect_identical( f2(x), x )
#   
#   x <- list(1L, "cats", 3L, NA)
#   expect_identical( f1(x), x )
#   expect_identical( f2(x), x )
#   
#   x <- data.frame(
#     "col1" = c(1, 2), 
#     "col2" = c(1L, 2L), 
#     "col3" = c(TRUE, FALSE), 
#     "col4" = c("cats", "dogs"), 
#     stringsAsFactors = FALSE
#   )
#   expect_identical( f1(x), x )
#   
#   
# })






