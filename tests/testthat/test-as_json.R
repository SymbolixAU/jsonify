context("as_json")

test_that("json class set", {
  
  js <- "{x:1}"  ## Invalid
  expect_error( as.json(js), "Invalid JSON" )
  
  js <- '{"x":1}'
  js <- as.json(js)
  expect_equal( attr(js, "class"), "json" )
  
  js <- '{"x":[1,2,3]}'
  js <- pretty_json(js)
  expect_equal( attr(js, "class"), "json" )
})

test_that("non-characters coerced to JSON", {
  
  # df <- data.frame(
  #   x = 1:5
  #   , y = letters[1:5]
  #   , stringsAsFactors = F
  # )
  # 
  # expect_true( to_json( df ) == as.json( df ) )
  
})
