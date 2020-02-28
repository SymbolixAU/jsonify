context("dataframes")

test_that("data.frame - all R data types are converted", {
  
  df <- data.frame(id = 1:2, stringsAsFactors = F)        ## numeric
  js <- to_json( df )
  expect_true( validate_json( js ) ) 
  expect_equal('[{"id":1},{"id":2}]', as.character( js ) )
  
  df <- data.frame(id = 1L:2L, stringsAsFactors = F)      ## integer
  js <- to_json( df )
  expect_true( validate_json( js ) ) 
  expect_equal('[{"id":1},{"id":2}]', as.character( js ) ) 
  
  df <- data.frame(id = c("a","b"), stringsAsFactors = T) ## factor
  js <- to_json( df, factors_as_string = FALSE )
  expect_true( validate_json( js ) ) 
  expect_equal('[{"id":1},{"id":2}]', as.character( js ) )
  
  df <- data.frame(id = c("a","b"), stringsAsFactors = F) ## character
  js <- to_json( df )
  expect_true( validate_json( js ) ) 
  expect_equal('[{"id":"a"},{"id":"b"}]', as.character( js ) )
  
  df <- data.frame(id = c(T,FALSE), stringsAsFactors = TRUE)                       ## logical
  js <- to_json( df )
  expect_true( validate_json( js ) ) 
  expect_equal('[{"id":true},{"id":false}]', as.character( js ) )
  
  df <- data.frame(id = as.Date("2018-01-01"), stringsAsFactors = TRUE)            ## Date
  js <- to_json( df, numeric_dates = FALSE )
  expect_true( validate_json( js ) ) 
  expect_equal('[{"id":"2018-01-01"}]', as.character( js ) )
  
  df <- data.frame(id = as.POSIXct("2018-01-01 00:00:59", tz = "Australia/Melbourne"), stringsAsFactors = TRUE)            ## Posixct
  js <- to_json( df, numeric_dates = FALSE )
  expect_true( validate_json( js ) ) 
  expect_equal('[{"id":"2017-12-31T13:00:59"}]', as.character( js ) )
  
  df <- data.frame(id = as.POSIXlt("2018-01-01 00:00:59", tz = "Australia/Melbourne"), stringsAsFactors = TRUE)            ## Posixct
  js <- to_json( df, numeric_dates = FALSE )
  expect_true( validate_json( js ) ) 
  expect_equal('[{"id":"2017-12-31T13:00:59"}]', as.character( js ) )
})

test_that("data.frame - complex columns are jsonified", {
  df <- data.frame( id = 1, val = I(list(c(0))), stringsAsFactors = TRUE)
  js <- to_json( df )
  expect_equal( as.character( js ), "[{\"id\":1.0,\"val\":[0.0]}]")
  expect_true( jsonify::validate_json( js ) ) 
  
  df <- data.frame( id = 1, val = I(list(c(0))), stringsAsFactors = TRUE)
  js <- to_json( df, unbox = TRUE  )
  expect_equal( as.character( js ), "[{\"id\":1.0,\"val\":0.0}]")
  expect_true( jsonify::validate_json( js ) ) 
  
  df <- data.frame( id = 1, val = I(list(c(0,0))), stringsAsFactors = TRUE)
  js <- to_json( df )
  expect_equal( as.character( js ), "[{\"id\":1.0,\"val\":[0.0,0.0]}]")
  expect_true( jsonify::validate_json( js ) ) 
  
  df <- data.frame( id = 1, val = I(list(letters[1:5])), stringsAsFactors = TRUE)
  js <- to_json( df )
  expect_equal( as.character( js ), "[{\"id\":1.0,\"val\":[\"a\",\"b\",\"c\",\"d\",\"e\"]}]")
  expect_true( jsonify::validate_json( js ) ) 
  
  df <- data.frame(
    id = 1:3
    , norm = c(1.1, 2.2, 3.3)
    , letters = c("a","x","B")
    , val = I(list(c(0,0),1,list(c(2,3), myname = c(4,5), c("a"))))
   , stringsAsFactors = F
   )
  
  js <- to_json( df ) 
  
  expect_equal(
    as.character( js ) , 
    '[{"id":1,"norm":1.1,"letters":"a","val":[0.0,0.0]},{"id":2,"norm":2.2,"letters":"x","val":[1.0]},{"id":3,"norm":3.3,"letters":"B","val":{"":[2.0,3.0],"myname":[4.0,5.0],"":["a"]}}]'
  )
  expect_true( jsonify::validate_json( js ) ) 
  
  
  df <- data.frame(
    id = 1:3
    , norm = c(1.1, 2.2, 3.3)
    , letters = c("a","x","B")
    , val = I(list(c(0),1,list(c(2), myname = c(4,5), c("a"))))
    , stringsAsFactors = F
    )
  
  js <- to_json( df, unbox = TRUE )
  expect_equal(
    as.character( js ), 
    '[{"id":1,"norm":1.1,"letters":"a","val":0.0},{"id":2,"norm":2.2,"letters":"x","val":1.0},{"id":3,"norm":3.3,"letters":"B","val":{"":2.0,"myname":[4.0,5.0],"":"a"}}]'
  )
  expect_true( validate_json( js ) )
  
  df <- data.frame(
    id = 1:2
    , val = I(list( x = 1:2, y = 3:6 ) )
    , val2 = I(list( a = "a", b = c("b","c") ) )
    , val3 = I(list( l = list( 1:3, l2 = c("a","b")), 1))
    , stringsAsFactors = TRUE
    )
  
  js <- to_json( df )
  
  expect_equal(
    as.character( js ),
    '[{"id":1,"val":{"x":[1,2]},"val2":{"a":["a"]},"val3":{"l":{"":[1,2,3],"l2":["a","b"]}}},{"id":2,"val":{"y":[3,4,5,6]},"val2":{"b":["b","c"]},"val3":{"":[1.0]}}]'
  )
  expect_true( validate_json( js ) )
})

test_that("column factors return levels and characters", {
  
  df <- data.frame( id = 1:5, val = letters[1:5], stringsAsFactors = TRUE )
  js <- to_json( df, by = "column", factors_as_string = FALSE )
  expect_equal( as.character( js ), '{"id":[1,2,3,4,5],"val":[1,2,3,4,5]}')
  expect_true( validate_json( js ) )
  
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

test_that("roundtrips work", {
  df <- data.frame(x = c("a", NA_character_), stringsAsFactors = F)
  expect_equal( from_json( to_json( df ) ), df )
  df <- data.frame(x = c(1, NA_integer_), stringsAsFactors = F)
  expect_equal( from_json( to_json( df ) ), df )
  df <- data.frame(x = c(1.0, NA_real_), stringsAsFactors = F)
  expect_equal( from_json( to_json( df ) ), df )
  df <- data.frame(x = c("a", NULL), stringsAsFactors = F)
  expect_equal( from_json( to_json( df ) ), df )
  df <- data.frame(x = c(T, NA), stringsAsFactors = F)
  expect_equal( from_json( to_json( df ) ), df )
  # df <- data.frame(x = c(1, Inf), stringsAsFactors = F)
  # expect_equal( from_json( to_json( df ) ), df )
  # df <- data.frame(x = c(1, -Inf), stringsAsFactors = F)
  # expect_equal( from_json( to_json( df ) ), df )
})


test_that("ISSUE 38 - data.frames inside data.frames works", {
  
  df <- structure(list(`_id` = 1, details = structure(list(
    val1 = "a", 
    val2 = "b", 
    val3 = "c"
  ), 
  class = "data.frame", row.names = 1L)), class = "data.frame", row.names = 1L)
  
  js <- to_json( df )
  expect_true( validate_json( js ) )
  expect_equal( as.character(js), '[{"_id":1.0,"details":{"val1":"a","val2":"b","val3":"c"}}]' )
  
  details <- data.frame(
    val1 = c("a","b")
    , val2 = c("b","c")
    , val3 = c("c","d")
    , stringsAsFactors = FALSE
  )
  
  df <- data.frame(
    id = 1
    , details = I(details)
    , stringsAsFactors = TRUE
  )
  
  js <- to_json( df )
  expect_true( validate_json( js ) )
  expect_equal(as.character(js), '[{"id":1.0,"details":{"val1":"a","val2":"b","val3":"c"}},{"id":1.0,"details":{"val1":"b","val2":"c","val3":"d"}}]')
  
  df <- data.frame(
    id = 1
    , details = details
    , stringsAsFactors = TRUE
  )
  
  js <- to_json( df )
  expect_true( validate_json( js ) )
  expect_equal( as.character(js) , '[{"id":1.0,"details.val1":"a","details.val2":"b","details.val3":"c"},{"id":1.0,"details.val1":"b","details.val2":"c","details.val3":"d"}]')
  
})

test_that("data.frame with a matrix-column works", {
  
  df <- data.frame( id = 1:2, mat = I(matrix(1:4, ncol = 2)), stringsAsFactors = TRUE)
  js <- to_json( df )
  expect_equal( as.character(js), '[{"id":1,"mat":[1,3]},{"id":2,"mat":[2,4]}]')
  expect_true( validate_json( js ) )
  
  js <- to_json( df, by = "col" )
  expect_equal( as.character( js ), '{"id":[1,2],"mat":[[1,2],[3,4]]}')

  ## Issue 42
  df <- structure(list(fill_colour = structure(c(68, 49, 53, 253, 1, 
  104, 183, 231, 84, 142, 121, 37, 255, 255, 255, 255), .Dim = c(4L, 
  4L)), geometry = c(1, 2, -5, 0.3), lat = 1:4, lon = c(1, 2, -5, 
  0.3)), class = "data.frame", row.names = c(NA, 4L))
  
  js <- to_json( df, by = "row" )
  expect_equal( as.character( js ), '[{"fill_colour":[68.0,1.0,84.0,255.0],"geometry":1.0,"lat":1,"lon":1.0},{"fill_colour":[49.0,104.0,142.0,255.0],"geometry":2.0,"lat":2,"lon":2.0},{"fill_colour":[53.0,183.0,121.0,255.0],"geometry":-5.0,"lat":3,"lon":-5.0},{"fill_colour":[253.0,231.0,37.0,255.0],"geometry":0.3,"lat":4,"lon":0.3}]')

  
})

test_that("issue 55 is solved",{
  
  df <- data.frame(string = c("first", NA), stringsAsFactors = TRUE)
  js <- to_json( df )
  expect_equal( as.character( js ), '[{"string":"first"},{"string":null}]')
  
})

