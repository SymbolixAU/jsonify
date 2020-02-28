context("from_ndjson")

test_that("single ndjson objects work in the same way as json",{
  
  expect_equal( from_json("{}"), from_ndjson("{}") )
  expect_equal( from_json("[]"), from_ndjson("[]") )
  expect_equal( from_ndjson('{"abc":123}'), from_json('{"abc":123}') )
  expect_equal( 1:5, from_ndjson("[1,2,3,4,5]") )
  expect_equal( letters[1:5], from_ndjson( '["a","b","c","d","e"]') )
  
})

test_that("new-line separated json is parsed correctly",{
  
  mat <- matrix(1:6, ncol = 2)
  
  expect_equal( mat, from_ndjson( '[1,4]\n[2,5]\n[3,6]') )

  df <- data.frame(
    x = 1:5
    , y = letters[1:5]
    , z = as.numeric( as.Date(seq(18262, 18262 + 4, by = 1 ), origin = "1970-01-01" ) )
    , stringsAsFactors = FALSE
  )
  
  expect_equal( df, from_ndjson("{\"x\":1,\"y\":\"a\",\"z\":18262.0}\n{\"x\":2,\"y\":\"b\",\"z\":18263.0}\n{\"x\":3,\"y\":\"c\",\"z\":18264.0}\n{\"x\":4,\"y\":\"d\",\"z\":18265.0}\n{\"x\":5,\"y\":\"e\",\"z\":18266.0}") )

  
  ## Lists are non-recurisve; only elements `x` and `y` are converted to ndjson
  ## unnamed
  lst <- list(
    1:5
    , list(
      a = letters[1:5]
      , b = data.frame(i = 10:15, j = 20:25, stringsAsFactors = TRUE)
    )
  )
  
  expect_equal( lst, from_ndjson( "[1,2,3,4,5]\n{\"a\":[\"a\",\"b\",\"c\",\"d\",\"e\"],\"b\":[{\"i\":10,\"j\":20},{\"i\":11,\"j\":21},{\"i\":12,\"j\":22},{\"i\":13,\"j\":23},{\"i\":14,\"j\":24},{\"i\":15,\"j\":25}]}" ) )

  ## named
  lst <- list(
    x = 1:5
    , y = list(
      a = letters[1:5]
      , b = data.frame(i = 10:15, j = 20:25, stringsAsFactors = TRUE)
    )
  )
  
  # nd <- "{\"x\":[1,2,3,4,5]}\n{\"y\":{\"a\":[\"a\",\"b\",\"c\",\"d\",\"e\"],\"b\":[{\"i\":10,\"j\":20},{\"i\":11,\"j\":21},{\"i\":12,\"j\":22},{\"i\":13,\"j\":23},{\"i\":14,\"j\":24},{\"i\":15,\"j\":25}]}}"
  # js <- "{\"x\":[1,2,3,4,5]},{\"y\":{\"a\":[\"a\",\"b\",\"c\",\"d\",\"e\"],\"b\":[{\"i\":10,\"j\":20},{\"i\":11,\"j\":21},{\"i\":12,\"j\":22},{\"i\":13,\"j\":23},{\"i\":14,\"j\":24},{\"i\":15,\"j\":25}]}}]"
  # 
  # from_ndjson( nd )
  # from_json( js )
  
  ## See comments in issue 58 as to why this is necessary
  ## https://github.com/SymbolixAU/jsonify/issues/58#issuecomment-585098163
  expect_equal(
    from_ndjson( to_ndjson( lst ) )
    , from_ndjson( "{\"x\":[1,2,3,4,5]}\n{\"y\":{\"a\":[\"a\",\"b\",\"c\",\"d\",\"e\"],\"b\":[{\"i\":10,\"j\":20},{\"i\":11,\"j\":21},{\"i\":12,\"j\":22},{\"i\":13,\"j\":23},{\"i\":14,\"j\":24},{\"i\":15,\"j\":25}]}}" )
    )
  
})

