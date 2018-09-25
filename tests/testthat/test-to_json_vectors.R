context("vectors")

test_that("different vector types work", {
  
  expect_equal(to_json(c(1L:5L)), "[1,2,3,4,5]")                         # integer
  expect_equal(to_json(c(1.0,2.1,3.2)), "[1.0,2.1,3.2]")                 # numeric
  expect_equal(to_json(letters[1:5]), "[\"a\",\"b\",\"c\",\"d\",\"e\"]") # character
  expect_equal(to_json(c(T,F,T,F)), "[true,false,true,false]")
  expect_equal(to_json(as.Date("2018-01-01")), "[17532.0]")
  expect_equal(to_json(as.Date("2018-01-01"), numeric_dates = F), "[\"2018-01-01\"]")
  expect_equal(to_json(as.POSIXct("2018-01-01 01:00:00", tz = "GMT")), "[1514768400.0]")
  expect_equal(to_json(as.POSIXct("2018-01-01 01:00:00", tz = "GMT"), numeric_dates = F), "[\"2018-01-01 01:00:00\"]")
  expect_equal(to_json(as.POSIXlt("2018-01-01 01:00:00", tz = "GMT")), "{\"sec\":[0.0],\"min\":[0],\"hour\":[1],\"mday\":[1],\"mon\":[0],\"year\":[118],\"wday\":[1],\"yday\":[0],\"isdst\":[0]}")
  expect_equal(to_json(as.POSIXct("2018-01-01 01:00:00", tz = "GMT"), numeric_dates = F), "[\"2018-01-01 01:00:00\"]")
  
  expect_error(to_json(complex(1)), "this type is not supported")
  
})


