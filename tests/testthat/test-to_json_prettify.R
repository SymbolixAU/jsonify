context("prettify")

test_that("indentations added", {
  
  df <- data.frame(id = 1:3, val = letters[1:3], stringsAsFactors = TRUE)
  js <- jsonify::to_json( df )
  js <- jsonify::pretty_json( js )
  expected <- "[\n    {\n        \"id\": 1,\n        \"val\": \"a\"\n    },\n    {\n        \"id\": 2,\n        \"val\": \"b\"\n    },\n    {\n        \"id\": 3,\n        \"val\": \"c\"\n    }\n]"
  expect_equal( as.character( js ), expected )
  
  ## directly on R object
  js <- jsonify::pretty_json( df )
  expect_equal( as.character( js ), expected )
  
})

test_that("indentations removed", {
  
  df <- data.frame(id = 1:3, val = letters[1:3], stringsAsFactors = TRUE)
  js <- jsonify::pretty_json( df )
  js <- jsonify::minify_json( js )
  expected <- "[{\"id\":1,\"val\":\"a\"},{\"id\":2,\"val\":\"b\"},{\"id\":3,\"val\":\"c\"}]"
  expect_equal( as.character( js ), expected )
  
  ## directly on R object
  js <- jsonify::minify_json( df )
  expect_equal( as.character( js ), expected )
  
})



