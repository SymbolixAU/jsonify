context("Dates")

test_that("Dates are handled", {
  
  x <- as.Date("2018-01-01")
  expect_equal( as.character( to_json( x, numeric_dates = FALSE ) ), '["2018-01-01"]' )
  
  x <- as.Date("2018-01-01", tz = "Australia/Melbourne")
  expect_equal( as.character( to_json( x, numeric_dates = FALSE ) ), '["2018-01-01"]' )
  
  x <- as.Date("2018-01-01", tz = "GMT")
  expect_equal( as.character( to_json( x, numeric_dates = FALSE ) ), '["2018-01-01"]' )

  x <- as.POSIXct("2018-01-01", tz = "GMT")
  expect_equal( as.character( to_json( x, numeric_dates = FALSE ) ), '["2018-01-01T00:00:00"]' )
  
  x <- as.POSIXct("2018-01-01") ## defaults to system
  expect_equal( as.character( to_json( x, numeric_dates = FALSE ) ), '["2017-12-31T13:00:00"]' )
    
  x <- as.POSIXct("2018-01-01", tz = "Australia/Melbourne")
  expect_equal( as.character( to_json( x, numeric_dates = FALSE ) ), '["2017-12-31T13:00:00"]' )
  
})