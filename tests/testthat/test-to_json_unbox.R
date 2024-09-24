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
  
})

test_that("Posixlt unboxed", {
  
  testthat::skip_on_cran()
  
  x <- as.POSIXlt("2018-01-01 01:00:00", tz = "GMT")
  js <- to_json(x, unbox = T)
  expect_equal( as.character( js ), "{\"sec\":0.0,\"min\":0,\"hour\":1,\"mday\":1,\"mon\":0,\"year\":118,\"wday\":1,\"yday\":0,\"isdst\":0,\"zone\":\"GMT\",\"gmtoff\":0}" )
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

test_that("AsIs boxing", {

  prepend_class <- function(x, cls) {
    structure(x, class = unique(c(cls, class(x))))
  }

  ## baseline: no AsIs
  x <- 1L
  js <- to_json(x, unbox = FALSE)
  expect_equal(as.character(js), "[1]")
  expect_true(validate_json(js))
  js <- to_json(x, unbox = TRUE)
  expect_equal(as.character(js), "1" )
  expect_true(validate_json(js))

  ## now with AsIs (in the non-first class() location):
  x <- prepend_class(I(1L), "foo")
  expect_identical(class(x), c("foo", "AsIs"))
  ## AsIs should not affect when not unboxing:
  js <- to_json(x, unbox = FALSE)
  expect_equal(as.character(js), "[1]")
  expect_true(validate_json(js))
  ## AsIs should now protect the length-1 vector:
  js <- to_json(x, unbox = TRUE)
  expect_equal(as.character(js), "[1]")
  expect_true(validate_json(js))

  ## verify that length-zero vectors are unaffected by I():
  x <- I(integer(0))
  js <- to_json(x, unbox = FALSE)
  expect_equal(as.character(js), "[]")
  expect_true(validate_json(js))
  js <- to_json(x, unbox = TRUE)
  expect_equal(as.character(js), "[]")
  expect_true(validate_json(js))

  ## verify that length > 1 vectors are unaffected by I():
  x <- 1:3
  js <- to_json(x, unbox = FALSE)
  expect_equal(as.character(js), "[1,2,3]")
  expect_true(validate_json(js))
  js <- to_json(x, unbox = TRUE)
  expect_equal(as.character(js), "[1,2,3]")
  expect_true(validate_json(js))
  
})
