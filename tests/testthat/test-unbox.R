context("unbox")

test_that("single-vectors unboxed", {
  
  x <- 1L
  js <- to_json(x, unbox = T)
  expect_equal( as.character( js ), "1")
  expect_true( validate_json( js ) )
  x <- 1
  js <- to_json(x, unbox = T)
  expect_equal( as.character( js ), "1.0")
  expect_true( validate_json( js ) )
  x <- "a"
  js <- to_json(x, unbox = T)
  expect_equal( as.character( js ), "\"a\"")
  expect_true( validate_json( js ) )
  x <- as.Date("2018-01-01")
  js <- to_json(x, unbox = T)
  expect_equal( as.character( js ), "17532.0")
  expect_true( validate_json( js ) )
  x <- as.Date("2018-01-01")
  js <- to_json(x, unbox = T, numeric_dates = F )
  expect_equal( as.character( js ), "\"2018-01-01\"")
  expect_true( validate_json( js ) )
  x <- as.POSIXct("2018-01-01 00:00:01", tz = "Australia/Melbourne")
  js <- to_json(x, unbox = T, numeric_dates = F )
  expect_equal( as.character( js ), "\"2017-12-31T13:00:01\"")
  expect_true( validate_json( js ) )
  x <- as.POSIXlt("2018-01-01 01:00:00", tz = "GMT")
  js <- to_json(x, unbox = T)
  expect_equal( as.character( js ), "{\"sec\":0.0,\"min\":0,\"hour\":1,\"mday\":1,\"mon\":0,\"year\":118,\"wday\":1,\"yday\":0,\"isdst\":0}" )
  expect_true( validate_json( js ) )
})

test_that("single-vectors in lists are unboxed", {
  
  x <- list(x = 1)
  js <- to_json(x, unbox = T)
  expect_equal( as.character( js ), '{"x":1.0}' )
  expect_true( validate_json( js ) )
  x <- list(x = 1, y = c("a","b"))
  js <- to_json(x, unbox = T)
  expect_equal( as.character( js ),  '{"x":1.0,"y":["a","b"]}')
  expect_true( validate_json( js ) )
  x <- list(x = 1, y = c("a","b"), z = list(x = 1))
  js <- to_json(x, unbox = T)
  expect_equal( as.character( js ),  '{"x":1.0,"y":["a","b"],"z":{"x":1.0}}')
  expect_true( validate_json( js ) )
})

test_that("single-column matrices are unboxed", {
  
  x <- matrix(1)
  js <- to_json(x, unbox = T)
  expect_equal( as.character( js ), "[1.0]" )
  expect_true( validate_json( js ) )
  x <- matrix(1:2, ncol = 2)
  js <- to_json(x, unbox = T)
  expect_equal( as.character( js ), "[[1,2]]" )
  expect_true( validate_json( js ) )
  x <- matrix(1:2, ncol = 1)
  js <- to_json(x, unbox = T)
  expect_equal( as.character( js ), "[1,2]" )
  expect_true( validate_json( js ) )
  x <- matrix(1:4, ncol = 2)
  js <- to_json(x, unbox = T)
  expect_equal( as.character( js ), "[[1,3],[2,4]]" )
  expect_true( validate_json( js ) )
})


