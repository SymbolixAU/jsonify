context("dataframes")

test_that("data.frame - all R data types are converted", {
  df <- data.frame(id = 1:2, stringsAsFactors = F)        ## numeric
  expect_equal('[{"id":1},{"id":2}]', as.character(to_json(df)))
  df <- data.frame(id = 1L:2L, stringsAsFactors = F)      ## integer
  expect_equal('[{"id":1},{"id":2}]', as.character(to_json(df)))
  df <- data.frame(id = c("a","b"), stringsAsFactors = T) ## factor
  expect_equal('[{"id":1},{"id":2}]', as.character(to_json(df)))
  df <- data.frame(id = c("a","b"), stringsAsFactors = F) ## character
  expect_equal('[{"id":"a"},{"id":"b"}]', as.character(to_json(df)))
  df <- data.frame(id = c(T,FALSE))                       ## logical
  expect_equal('[{"id":true},{"id":false}]', as.character(to_json(df)))
  df <- data.frame(id = as.Date("2018-01-01"))            ## Date
  expect_equal('[{"id":"2018-01-01"}]', as.character(to_json(df, numeric_dates = F)))
  df <- data.frame(id = as.POSIXct("2018-01-01 00:00:59"))            ## Posixct
  expect_equal('[{"id":"2018-01-01 00:00:59"}]', as.character(to_json(df, numeric_dates = F)))
  df <- data.frame(id = as.POSIXlt("2018-01-01 00:00:59"))            ## Posixct
  expect_equal('[{"id":"2018-01-01 00:00:59"}]', as.character(to_json(df, numeric_dates = F)))
})

test_that("data.frame - complex columns are jsonified", {
  df <- data.frame( id = 1, val = I(list(c(0))))
  expect_equal( as.character( to_json( df ) ), "[{\"id\":1.0,\"val\":[0.0]}]")
  
  df <- data.frame( id = 1, val = I(list(c(0))))
  expect_equal( as.character( to_json( df, unbox = T ) ), "[{\"id\":1.0,\"val\":0.0}]")
  
  df <- data.frame( id = 1, val = I(list(c(0,0))))
  expect_equal( as.character( to_json( df ) ), "[{\"id\":1.0,\"val\":[0.0,0.0]}]")
  
  df <- data.frame( id = 1, val = I(list(letters[1:5])))
  expect_equal( as.character( to_json( df ) ), "[{\"id\":1.0,\"val\":[\"a\",\"b\",\"c\",\"d\",\"e\"]}]")
  expect_true( jsonify::validate_json( to_json( df ) ) ) 
  
  df <- data.frame(
    id = 1:3
    , norm = c(1.1, 2.2, 3.3)
    , letters = c("a","x","B")
    , val = I(list(c(0,0),1,list(c(2,3), myname = c(4,5), c("a"))))
   , stringsAsFactors = F)
  
  expect_equal(
    as.character( to_json( df ) ) , 
    "[{\"id\":1,\"norm\":1.1,\"letters\":\"a\",\"val\":[0.0,0.0]},{\"id\":2,\"norm\":2.2,\"letters\":\"x\",\"val\":[1.0]},{\"id\":3,\"norm\":3.3,\"letters\":\"B\",\"val\":{\"1\":[2.0,3.0],\"myname\":[4.0,5.0],\"3\":[\"a\"]}}]"
  )
  expect_true(jsonify::validate_json( to_json( df ) ) )
  
  
  df <- data.frame(
    id = 1:3
    , norm = c(1.1, 2.2, 3.3)
    , letters = c("a","x","B")
    , val = I(list(c(0),1,list(c(2), myname = c(4,5), c("a"))))
    , stringsAsFactors = F)
  
  expect_equal(
    as.character( to_json( df, unbox = T ) ) , 
    "[{\"id\":1,\"norm\":1.1,\"letters\":\"a\",\"val\":0.0},{\"id\":2,\"norm\":2.2,\"letters\":\"x\",\"val\":1.0},{\"id\":3,\"norm\":3.3,\"letters\":\"B\",\"val\":{\"1\":2.0,\"myname\":[4.0,5.0],\"3\":\"a\"}}]"
  )
  expect_true(jsonify::validate_json( to_json( df, unbox = T ) ) )
})

test_that("Nulls, NAs and Infs work",{
  expect_equal(as.character(to_json(data.frame(x = NA))), "[{\"x\":null}]" )
  expect_equal(as.character(to_json(data.frame(x = NA_character_))), "[{\"x\":null}]"  )
  expect_equal(as.character(to_json(data.frame(x = NA_real_))), "[{\"x\":null}]"  )
  expect_equal(as.character(to_json(data.frame(x = NA_integer_))), "[{\"x\":null}]"  )
  expect_equal(as.character(to_json(data.frame(x = NA_complex_))), "[{\"x\":null}]"  )
  expect_equal(as.character(to_json(data.frame(x = NaN))), "[{\"x\":null}]"  )
  
  expect_equal(as.character(to_json(data.frame(x = Inf))), '[{"x":"Inf"}]' )  
  expect_equal(as.character(to_json(data.frame(x = -Inf))), '[{"x":"-Inf"}]') 
  
  expect_equal(as.character(to_json(data.frame(x = NULL))), "[]")
  expect_equal(to_json(data.frame(x = c(1,2,NULL,4))), to_json(data.frame(x = c(1,2,4))))
})

test_that("roundtrips with jsonlite work", {
  df <- data.frame(x = c("a", NA_character_), stringsAsFactors = F)
  expect_equal( jsonlite::fromJSON( to_json( df ) ), df )
  df <- data.frame(x = c(1, NA_integer_), stringsAsFactors = F)
  expect_equal( jsonlite::fromJSON( to_json( df ) ), df )
  df <- data.frame(x = c(1.0, NA_real_), stringsAsFactors = F)
  expect_equal( jsonlite::fromJSON( to_json( df ) ), df )
  df <- data.frame(x = c("a", NULL), stringsAsFactors = F)
  expect_equal( jsonlite::fromJSON( to_json( df ) ), df )
  df <- data.frame(x = c(T, NA), stringsAsFactors = F)
  expect_equal( jsonlite::fromJSON( to_json( df ) ), df )
  df <- data.frame(x = c(1, Inf), stringsAsFactors = F)
  expect_equal( jsonlite::fromJSON( to_json( df ) ), df )
  df <- data.frame(x = c(1, -Inf), stringsAsFactors = F)
  expect_equal( jsonlite::fromJSON( to_json( df ) ), df )
})
