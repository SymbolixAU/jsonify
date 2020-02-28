context("vectors")

test_that("different vector types work", {
  
  expect_equal(as.character(to_json(c(1L:5L))), "[1,2,3,4,5]")                         # integer
  expect_equal(as.character(to_json(c(1.0,2.1,3.2))), "[1.0,2.1,3.2]")                 # numeric
  expect_equal(as.character(to_json(letters[1:5])), "[\"a\",\"b\",\"c\",\"d\",\"e\"]") # character
  expect_equal(as.character(to_json(c(T,F,T,F))), "[true,false,true,false]")
  expect_equal(as.character(to_json(as.Date("2018-01-01"))), "[17532.0]")
  expect_equal(as.character(to_json(as.Date("2018-01-01"), numeric_dates = F)), "[\"2018-01-01\"]")
  expect_equal(as.character(to_json(as.POSIXct("2018-01-01 01:00:00", tz = "GMT"))), "[1514768400.0]")
  expect_equal(as.character(to_json(as.POSIXct("2018-01-01 01:00:00", tz = "GMT"), numeric_dates = F)), "[\"2018-01-01T01:00:00\"]")
  expect_equal(as.character(to_json(as.POSIXlt("2018-01-01 01:00:00", tz = "GMT"))), "{\"sec\":[0.0],\"min\":[0],\"hour\":[1],\"mday\":[1],\"mon\":[0],\"year\":[118],\"wday\":[1],\"yday\":[0],\"isdst\":[0]}")
  expect_equal(as.character(to_json(as.POSIXlt("2018-01-01 01:00:00", tz = "GMT"), numeric_dates = F)), '{"sec":[0.0],"min":[0],"hour":[1],"mday":[1],"mon":[0],"year":[118],"wday":[1],"yday":[0],"isdst":[0]}')
  expect_equal(as.character(to_json(complex(1))),"[\"0+0i\"]")
  expect_equal(as.character(to_json( list(x = as.Date("2018-01-01") ), numeric_dates = F)), '{"x":[\"2018-01-01\"]}')
  expect_equal(as.character(to_json( list(x = as.Date("2018-01-01") ), numeric_dates = T)), '{"x":[17532.0]}')
  
  expect_equal(as.character(to_json( as.factor(letters[1:5]))), '["a","b","c","d","e"]')
  expect_equal(as.character(to_json( as.factor(letters[1:5]), factors_as_string = FALSE )),'[1,2,3,4,5]')
  
})

test_that("NAs, NULLS and Infs work", {
  expect_equal(as.character(to_json( NA )), "[null]")
  expect_equal(as.character(to_json( NA_character_ )), "[null]")
  expect_equal(as.character(to_json( NA_complex_ )), "[null]")
  expect_equal(as.character(to_json( NA_integer_ )), "[null]")
  expect_equal(as.character(to_json( NA_real_ )), "[null]")
  expect_equal(as.character(to_json( NaN )), "[null]")
  expect_equal(as.character(to_json( Inf )), "[\"Inf\"]")
  expect_equal(as.character(to_json( -Inf )), "[\"-Inf\"]")
  expect_equal(as.character(to_json(NULL)), "{}")
  expect_equal(as.character(to_json(list(a="a",b=NULL))),"{\"a\":[\"a\"],\"b\":{}}")
  expect_equal(as.character(to_json(data.frame())), "[]")
})

test_that("NAs, NULLS and Infs work and unboxed", {
  expect_equal(as.character(to_json( NA, unbox = T )), "null")
  expect_equal(as.character(to_json( NA_character_, unbox = T )), "null")
  expect_equal(as.character(to_json( NA_complex_, unbox = T )), "null")
  expect_equal(as.character(to_json( NA_integer_, unbox = T )), "null")
  expect_equal(as.character(to_json( NA_real_, unbox = T )), "null")
  expect_equal(as.character(to_json( NaN, unbox = T )), "null")
  expect_equal(as.character(to_json( Inf, unbox = T )), "\"Inf\"")
  expect_equal(as.character(to_json( -Inf, unbox = T )), "\"-Inf\"")
  expect_equal(as.character(to_json(NULL, unbox = T)), "{}")
  expect_equal(as.character(to_json(list(a="a",b=NULL), unbox = T)),"{\"a\":\"a\",\"b\":{}}")
  expect_equal(as.character(to_json(data.frame(), unbox = T)), "[]")
})

test_that("round trips work", {
  x <- c(1L, NA_integer_)
  expect_equal( from_json( to_json( x ) ), x)
  x <- c(1.0, NA_real_)
  expect_equal( from_json( to_json( x ) ), x)
  x <- c("1", NA_character_)
  expect_equal( from_json( to_json( x ) ), x)
  x <- c(T,F, NA)
  expect_equal( from_json( to_json( x ) ), x)
  # x <- c(1, Inf, -Inf)
  # expect_equal( from_json( to_json( x ) ), x)
  x <- list()
  expect_equal( from_json( to_json( x ) ), x)
})

test_that("some randome thoughts I had work", {

  lst <- list(x = as.Date("2018-01-01"), y = list(as.POSIXct("2018-01-01 10:00:00", tz = "UTC")), z = NA)
  js <- to_json( lst, numeric_dates = FALSE, unbox = TRUE )
  expect_true( validate_json( js ) ) 
  expect_equal( as.character(js), '{"x":"2018-01-01","y":["2018-01-01T10:00:00"],"z":null}')
  
})

test_that("factors work as expected", {
  
  expect_equal( as.character( to_json( factor() ) ), "[]" )
  
  expect_equal( as.character( to_json( factor( c("a","a") ) ) ), '["a","a"]')
  expect_equal( as.character( to_json( factor( c("a","a") ), factors_as_string = FALSE ) ), '[1,1]')
  
  x <- factor(c("a", NA_character_, "B"))
  expect_equal( as.character( to_json( x ) ), '["a",null,"B"]' )
  
  # expect_equal( to_json( factor() ), to_json( numeric() ) )
  
  expect_equal( as.character( to_json( factor( NA_integer_ ) ) ), "null" )
  
  df_factors <- data.frame( vals = sample(letters, size = 100, replace = TRUE), stringsAsFactors = TRUE)
  df_strings <- df_factors
  df_strings$vals <- as.character( df_factors$vals )
  
  expect_equal( to_json( df_factors ), to_json( df_strings ) )
  expect_equal( to_json( df_factors[1, ] ), to_json( df_strings[1, ] ) )
  
  df <- data.frame( x = c("a","a","a"), stringsAsFactors = TRUE )
  expect_equal( as.character( to_json( df ) ), '[{"x":"a"},{"x":"a"},{"x":"a"}]' )
  expect_equal( as.character( to_json( df , factors_as_string = FALSE ) ), '[{"x":1},{"x":1},{"x":1}]' )
  
})