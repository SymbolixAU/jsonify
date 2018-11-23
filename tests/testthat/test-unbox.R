context("unbox")

test_that("single-vectors unboxed", {
  
  x <- 1L
  expect_equal( as.character( to_json(x, auto_unbox = T) ), "1")
  x <- 1
  expect_equal( as.character( to_json(x, auto_unbox = T) ), "1.0")
  x <- "a"
  expect_equal( as.character( to_json(x, auto_unbox = T) ), "\"a\"")
  x <- as.Date("2018-01-01")
  expect_equal( as.character( to_json(x, auto_unbox = T) ), "17532.0")
  x <- as.Date("2018-01-01")
  expect_equal( as.character( to_json(x, auto_unbox = T, numeric_dates = F ) ), "\"2018-01-01\"")
  x <- as.POSIXct("2018-01-01 00:00:01")
  expect_equal( as.character( to_json(x, auto_unbox = T, numeric_dates = F ) ), "\"2018-01-01 00:00:01\"")
  x <- as.POSIXlt("2018-01-01 01:00:00", tz = "GMT")
  expect_equal( as.character( to_json( x , auto_unbox = T) ), "{\"sec\":0.0,\"min\":0,\"hour\":1,\"mday\":1,\"mon\":0,\"year\":118,\"wday\":1,\"yday\":0,\"isdst\":0}" )
})

test_that("single-vectors in lists are unboxed", {
  
  x <- list(x = 1)
  expect_equal( as.character( to_json( x, auto_unbox = T ) ), '{"x":1.0}' )
  x <- list(x = 1, y = c("a","b"))
  expect_equal( as.character( to_json( x, auto_unbox = T ) ),  '{"x":1.0,"y":["a","b"]}')
  x <- list(x = 1, y = c("a","b"), z = list(x = 1))
  expect_equal( as.character( to_json( x, auto_unbox = T ) ),  '{"x":1.0,"y":["a","b"],"z":{"x":1.0}}')
  
})

test_that("single-column matrices are unboxed", {
  
  m <- matrix(1)
  expect_equal( as.character( to_json( m, auto_unbox = T ) ), "[1.0]" )
  m <- matrix(1:2, ncol = 2)
  expect_equal( as.character( to_json( m, auto_unbox = T ) ), "[[1,2]]" )
  m <- matrix(1:2, ncol = 1)
  expect_equal( as.character( to_json( m, auto_unbox = T ) ), "[1,2]" )
  m <- matrix(1:4, ncol = 2)
  expect_equal( as.character( to_json( m, auto_unbox = T ) ), "[[1,3],[2,4]]" )
})


