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
  
  x <- as.POSIXct("2018-01-01", tz = "Australia/Melbourne")
  expect_equal( as.character( to_json( x, numeric_dates = FALSE ) ), '["2017-12-31T13:00:00"]' )

  x <- as.POSIXlt("2018-01-01", tz = "GMT")
  expect_equal( as.character( to_json( x , numeric_dates = FALSE) ), '{"sec":[0.0],"min":[0],"hour":[0],"mday":[1],"mon":[0],"year":[118],"wday":[1],"yday":[0],"isdst":[0]}')
  
})

test_that("Different R objects with dates are handled", {
  
  ## DATE
  ## vector
  x <- seq( as.Date("2018-01-01"), as.Date("2018-01-06"), length.out = 6)
  js <- to_json( x, numeric_dates = FALSE )
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), '["2018-01-01","2018-01-02","2018-01-03","2018-01-04","2018-01-05","2018-01-06"]')
  
  ## data.frame
  df <- data.frame( dte = x, stringsAsFactors = TRUE )
  js <- to_json( df, numeric_dates = FALSE )
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), '[{"dte":"2018-01-01"},{"dte":"2018-01-02"},{"dte":"2018-01-03"},{"dte":"2018-01-04"},{"dte":"2018-01-05"},{"dte":"2018-01-06"}]')
  
  ## POSIXct
  ## vector
  x <- seq( as.POSIXct("2018-01-01", tz = "Australia/Melbourne"), as.POSIXct("2018-01-06", tz = "Australia/Melbourne"), length.out = 6)
  js <- to_json( x, numeric_dates = FALSE )
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), '["2017-12-31T13:00:00","2018-01-01T13:00:00","2018-01-02T13:00:00","2018-01-03T13:00:00","2018-01-04T13:00:00","2018-01-05T13:00:00"]')
  
  ## data.frame
  df <- data.frame( dte = x, stringsAsFactors = TRUE )
  js <- to_json( df, numeric_dates = FALSE )
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), '[{"dte":"2017-12-31T13:00:00"},{"dte":"2018-01-01T13:00:00"},{"dte":"2018-01-02T13:00:00"},{"dte":"2018-01-03T13:00:00"},{"dte":"2018-01-04T13:00:00"},{"dte":"2018-01-05T13:00:00"}]')
 
  ## vector
  x <- seq( as.POSIXct("2018-01-01 00:00:01", tz = "Australia/Melbourne"), as.POSIXct("2018-01-06 00:00:01", tz = "Australia/Melbourne"), length.out = 6)
  js <- to_json( x, numeric_dates = FALSE )
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), '["2017-12-31T13:00:01","2018-01-01T13:00:01","2018-01-02T13:00:01","2018-01-03T13:00:01","2018-01-04T13:00:01","2018-01-05T13:00:01"]')
  
  ## data.frame
  df <- data.frame( dte = x, stringsAsFactors = TRUE )
  js <- to_json( df, numeric_dates = FALSE )
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), '[{"dte":"2017-12-31T13:00:01"},{"dte":"2018-01-01T13:00:01"},{"dte":"2018-01-02T13:00:01"},{"dte":"2018-01-03T13:00:01"},{"dte":"2018-01-04T13:00:01"},{"dte":"2018-01-05T13:00:01"}]')
  
   
  ## POSIXlt
  ## vector
  x <- seq( as.POSIXlt("2018-01-01", tz = "Australia/Melbourne"), as.POSIXlt("2018-01-06", tz = "Australia/Melbourne"), length.out = 6)
  js <- to_json( x, numeric_dates = FALSE )
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), '["2017-12-31T13:00:00","2018-01-01T13:00:00","2018-01-02T13:00:00","2018-01-03T13:00:00","2018-01-04T13:00:00","2018-01-05T13:00:00"]')
  
  ## data.frame
  df <- data.frame( dte = x, stringsAsFactors = TRUE )
  js <- to_json( df, numeric_dates = FALSE )
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), '[{"dte":"2017-12-31T13:00:00"},{"dte":"2018-01-01T13:00:00"},{"dte":"2018-01-02T13:00:00"},{"dte":"2018-01-03T13:00:00"},{"dte":"2018-01-04T13:00:00"},{"dte":"2018-01-05T13:00:00"}]')
  
  ## vector
  x <- seq( as.POSIXlt("2018-01-01 00:00:01", tz = "Australia/Melbourne"), as.POSIXlt("2018-01-06 00:00:01", tz = "Australia/Melbourne"), length.out = 6)
  js <- to_json( x, numeric_dates = FALSE )
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), '["2017-12-31T13:00:01","2018-01-01T13:00:01","2018-01-02T13:00:01","2018-01-03T13:00:01","2018-01-04T13:00:01","2018-01-05T13:00:01"]')
  
  ## data.frame
  ## same as POSIXct, because data.frames coerce POSIXlt to POSIXct
  df <- data.frame( dte = x, stringsAsFactors = TRUE )
  js <- to_json( df, numeric_dates = FALSE )
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), '[{"dte":"2017-12-31T13:00:01"},{"dte":"2018-01-01T13:00:01"},{"dte":"2018-01-02T13:00:01"},{"dte":"2018-01-03T13:00:01"},{"dte":"2018-01-04T13:00:01"},{"dte":"2018-01-05T13:00:01"}]')
  
})


test_that("posixlt not converted to numeric",{
  
  p <- as.POSIXlt("2019-01-01 00:00:00", tz = "GMT")  ## so travis works
  res <- to_json(p, numeric_dates = F)
  expect_equal( as.character( res ), '{"sec":[0.0],"min":[0],"hour":[0],"mday":[1],"mon":[0],"year":[119],"wday":[2],"yday":[0],"isdst":[0]}')
})