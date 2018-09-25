context("dataframes")

test_that("data.frame - all R data types are converted", {
  df <- data.frame(id = 1:2, stringsAsFactors = F)        ## numeric
  expect_equal('[{"id":1},{"id":2}]', to_json(df))
  df <- data.frame(id = 1L:2L, stringsAsFactors = F)      ## integer
  expect_equal('[{"id":1},{"id":2}]', to_json(df))
  df <- data.frame(id = c("a","b"), stringsAsFactors = T) ## factor
  expect_equal('[{"id":1},{"id":2}]', to_json(df))
  df <- data.frame(id = c("a","b"), stringsAsFactors = F) ## character
  expect_equal('[{"id":"a"},{"id":"b"}]', to_json(df))
  df <- data.frame(id = c(T,FALSE))                       ## logical
  expect_equal('[{"id":true},{"id":false}]', to_json(df))
  ## TODO
  # df <- data.frame(id = as.Date("2018-01-01"))            ## Date
  # expect_equal('[{"id":"2018-01-01"}]', to_json(df))
  # df <- data.frame(id = as.POSIXct("2018-01-01 00:00:59"))            ## Posixct
  # expect_equal('[{"id":"2018-01-01 00:00:59"}]', to_json(df))
  # df <- data.frame(id = as.POSIXlt("2018-01-01 00:00:59"))            ## Posixct
  # expect_equal('[{"id":"2018-01-01 00:00:59"}]', to_json(df))
})

test_that("data.frame - complex columns are jsonified", {
  df <- data.frame( id = 1, val = I(list(c(0,0))))
  expect_equal(jsonify::to_json( df ), "[{\"id\":1.0,\"val\":[0.0,0.0]}]")
  
  df <- data.frame( id = 1, val = I(list(letters[1:5])))
  expect_equal(jsonify::to_json( df ), "[{\"id\":1.0,\"val\":[\"a\",\"b\",\"c\",\"d\",\"e\"]}]")
  
  df <- data.frame(
    id = 1:3
    , norm = c(1.1, 2.2, 3.3)
    , letters = c("a","x","B")
    , val = I(list(c(0,0),1,list(c(2,3), myname = c(4,5), c("a"))))
   , stringsAsFactors = F)
  expect_equal(
    jsonify::to_json( df ) , 
    "[{\"id\":1,\"norm\":1.1,\"letters\":\"a\",\"val\":[0.0,0.0]},{\"id\":2,\"norm\":2.2,\"letters\":\"x\",\"val\":[1.0]},{\"id\":3,\"norm\":3.3,\"letters\":\"B\",\"val\":{\"1\":[2.0,3.0],\"myname\":[4.0,5.0],\"3\":[\"a\"]}}]"
  )
  

})
