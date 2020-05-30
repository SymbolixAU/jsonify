context("from_json")

test_that("scalar values handled properly", {
  target <- list(a = 8L, b = 99.5, c = TRUE, d = "cats", e = NA)

  js <- "{\"a\":8, \"b\":99.5, \"c\":true, \"d\":\"cats\", \"e\":null}"
  x <- from_json(js)
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  x <- from_json(js)
  expect_equal(x, target)

  expect_equal(from_json("1"), 1)
  expect_equal(from_json("1.5"), 1.5)
  expect_equal(from_json("\"a\""), "a")
  expect_equal(from_json("true"), TRUE)
})

test_that("vector / array values handled properly", {

  target <- list(a = list(1L, 2L, 3L, NA), b = list(1L, "cats", 3L, NA))
  js <- "{\"a\":[1, 2, 3, null], \"b\":[1, \"cats\", 3, null]}"
  x <- from_json(js, simplify = FALSE)
  expect_equal(x, target)
  
  js <- jsonify::to_json(target, unbox = T)
  x <- from_json(js, simplify = FALSE)
  expect_equal(x, target)

  target <- list(a = c(1L, 2L, 3L, NA_integer_), b = c("1", "cats","3", NA))
  js <- "{\"a\":[1, 2, 3, null], \"b\":[1, \"cats\", 3, null]}"
  x <- from_json(js, simplify = TRUE)
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)
})

test_that("nested JSON handled properly", {
  target <- list(a = 8, b = list(c = 123, d = list(e = 456, f = NA)))

  js <- "{\"a\":8, \"b\":{\"c\":123, \"d\":{\"e\":456, \"f\":null}}}"
  x <- from_json( js )
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  x <- from_json( js )
  expect_equal(x, target)
})

test_that("nested JSON within an array handled properly", {

  js <- '[{"f":"cats"}]'
  target <- data.frame(f = "cats", stringsAsFactors = F)
  x <- from_json( js )
  expect_equal( x, target )
  
  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)

  js <- '{"e":[{"f":"cats"}]}'
  target <- list( e = data.frame(f = "cats", stringsAsFactors = F) )
  x <- from_json( js )
  expect_equal(x, target)
  
  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)

  # ## not valid for jsonify
  # js <- '[{"c":123,"d":456},{"f":"cats"}]'
  # target <- data.frame( c = c(123,NA), d = c(456, NA), f = c(NA, "cats"), stringsAsFactors = F)
  # x <- from_json( js )
  # df <- jsonlite::fromJSON( js )
  # to_json( df ) ## produced different js
  # '[{"c":123,"d":456,"f":null},{"c":null,"d":null,"f":"cats"}]'
  # ## because 'c' and 'd' dont' exist in the 2nd object; I don't want to introduce extra key/values
  # ## to replicate the js
  # to_json( list( list( c = 123, d = 456), list( e = data.frame(f = "cats") ) ), unbox = T)
  # ## and


  js <- '[{"c":123,"d":456},{"e":[{"f":"cats"}]}]'
  target <- list(list(c = c(123), d = c(456)), list( e = data.frame(f = "cats", stringsAsFactors = F) ) )
  x <- from_json( js )
  expect_equal( x, target )
  
  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)

  target <- list(a = 8L, b = list(list(c = 123L, d = 456L), list(e = data.frame(f = "cats", stringsAsFactors = F))))
  js <- '{"a":8, "b":[{"c":123,"d":456},{"e":[{"f":"cats"}]}]}'
  x <- from_json(js)
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)
})

test_that("JSON missing keys handled properly", {
  # vector
  target <- c(1L, 2L, 3L, NA_integer_)

  js <- "[1, 2, 3, null]"
  x <- from_json(js)
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)

  # list
  target <- list(1L, "cats", 3L, NA)

  js <- "[1, \"cats\", 3, null]"
  x <- from_json(js, simplify = FALSE )
  expect_equal(x, target)

  js <- jsonify::to_json(target, unbox = T)
  expect_equal(from_json(js, simplify = FALSE), target)
  
  target <- c("1","cats","3",NA)
  x <- from_json(js, simplify = TRUE )
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)
  
})

test_that("data frame returned properly", {
  # Return data frame in which the values in each name are of the same data type.
  target <- data.frame("id" = c(1L, 2L), "val" = c("a", "b"), stringsAsFactors = FALSE)

  js <- '[{"id":1,"val":"a"},{"id":2,"val":"b"}]'
  x <- from_json( js )
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)

  ## missing val2 in 2nd objet
  target <- list(list(id = c(1), val = c("a"), val2 = c(1)), list(id = c(2), val = c("b")))
  js <- '[{"id":1,"val":"a","val2":1},{"id":2,"val":"b"}]'
  x <- from_json( js )
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)

  ## fill_na
  target <- data.frame(id = c(1,2), val = c("a","b"), val2 = c(1,NA), stringsAsFactors = FALSE)
  x <- from_json( js, fill_na = TRUE )
  expect_equal(x, target)
  
  ## two entries with same key
  target <- list( list(id = 1, val = "a", val = 1), list(id = 2, val = "b"))
  js <- '[{"id":1,"val":"a","val":1},{"id":2,"val":"b"}]'
  x <- from_json( js )
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)
  
  ## fill_na (on duplicate keys)
  target <- data.frame(id = c(1,2), val = c("a","b"), stringsAsFactors = FALSE)
  x <- from_json( js, fill_na = TRUE )
  expect_equal(x, target)

  # Return data frame in which the values in each name are NOT of the same data type.
  target <- data.frame("id" = c("cats", 2L), val = c("a", "b"), stringsAsFactors = FALSE)
  js <- '[{"id":"cats","val":"a"},{"id":2,"val":"b"}]'
  x <- from_json( js )
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)

  # Return data frame in which the names do not align across JSON objects.
  target <- list( list(id = c(1L), val = c("a")), list(id = c(2L), blah = c("b")) )
  ## - don't simplify to a data.frame if the names in objects after the first one
  ## are different.
  js <- '[{"id":1,"val":"a"},{"id":2,"blah":"b"}]'
  x <- from_json(js)
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)
  
  ## fill_na 
  target <- data.frame(id = c(1,2), val = c("a",NA), blah = c(NA, "b"), stringsAsFactors = F)
  x <- from_json( js, fill_na = TRUE )
  expect_equal( x, target )
  
  target <- list( list(id = c(1L), val = c("a")), list(id = c(2L), blah = c(1L,2L)) )
  js <- '[{"id":1,"val":"a"},{"id":2,"blah":[1,2]}]'
  x <- from_json(js)
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)

  ## filL_na
  target <- data.frame(id = 1:2, val = c("a",NA), stringsAsFactors = FALSE)
  target$blah <- list(NA_integer_, 1:2)
  x <- from_json(js, fill_na = TRUE )
  expect_equal( x, target )
  
  ## 'val' changes type and length
  target <- data.frame(id = c("1","2"), stringsAsFactors = F)
  l <- list("a", 1:2)
  target$val <- l
  js <- '[{"id":"1","val":"a"},{"id":"2","val":[1,2]}]'
  x <- from_json(js)
  expect_equal(x, target)

  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)

  target <- data.frame(id = c("1","2"), stringsAsFactors = F)
  l <- list("a", matrix(1:2, ncol = 2) )
  target$val <- l
  js <- '[{"id":"1","val":["a"]},{"id":"2","val":[[1,2]]}]'
  x <- from_json(js)
  expect_equal(x, target)
  
  js <- jsonify::to_json(target)
  expect_equal(from_json(js), target)
  
  js <- '[{"val":["a"]},{"val":[1,2]}]'
  x <- from_json( js )
  expect_equal( as.character( to_json(x) ), js )

  js <- '[{"val":"a"},{"val":[[1,2]]}]'
  df <- from_json( js )
  expect_equal( as.character( to_json(df, unbox = T) ), js )

  ## matrices
  js <- '[{"val":[[1,2]]},{"val":[[1,2]]}]'
  x <- from_json( js )
  expect_equal( as.character( to_json(x, unbox = T) ), js )

  ## complex df columns
  js <- '[{"val":[[1,{"a":1,"b":2}]]},{"val":[[1,2]]}]'
  x <- from_json( js )
  expect_equal( as.character( to_json(x, unbox = T) ), js )


  ## different type
  js <- '[{"val":[[1,2]]},{"val":[["a","b"]]}]'
  x <- from_json( js )
  expect_equal( as.character( to_json(x, unbox = T) ), js )

  js <- '[{"val":[[1,2],[3,4]]},{"val":[["a","b"]]}]'
  x <- from_json( js )
  expect_equal( as.character( to_json(x, unbox = T) ), js )


  ## something more complex
  js <- '[{"val":{"inner_val":[[1,2],[3,4]]}},{"val":[["a","b"]]}]'
  x <- from_json( js )
  expect_equal( as.character( to_json(x, unbox = T) ), js )

  js <- '[{"val":{"inner_val":[[1,2],[3,4]]}},{"val":{"inner_val":[["a","b"]]}}]'
  x <- from_json( js )
  expect_equal( as.character( to_json(x, unbox = T) ), js )

  ## other complex stuff
  df1 <- data.frame(
    x = 1:2
    , y = 3:4
    , stringsAsFactors = T
  )
  df2 <- data.frame(
    z = c("a","b")
    , m = c("c","d")
    , stringsAsFactors = F
  )

  l <- list( df1, df2 )
  js <- to_json( l )
  x <- from_json( js )
  expect_equal(x, l)

  df1$z <- df2

  js <- to_json( df1 )
  x <- from_json( js )
  expect_equal(x, df1)

  ## shouldn't be a data.frame
  js <- '{"id":1,"val":2}'
  x <- from_json( js )
  expect_equal( as.character( to_json(x, unbox = T) ), js )
  
  ## should be a data.frame
  js <- '[{"id":1,"val":2}]'
  x <- from_json( js )
  expect_equal( as.character( to_json(x, unbox = T) ), js )

  ## should be a list
  l <- list(1,2,df)
  js <- '[1,2,{"id":1,"val":2}]'
  x <- from_json( js )
  expect_equal( as.character( to_json(x, unbox = T) ), js )
  
  ## should be a list with a data.frame element
  js <- '[1,2,[{"id":1,"val":2}]]'
  x <- from_json( js )
  expect_equal( as.character( to_json(x, unbox = T) ), js )

  df <- data.frame( id = 1:2, mat = I(matrix(1:4, ncol = 2)), stringsAsFactors = TRUE )
  js <- to_json( df )
  x <- from_json( js )
  ## I can't recreate 'AsIs' columns
  
  expect_equal(x$id, df$id)
  expect_equal(x$mat, unclass( df$mat) )
  expect_equal( to_json(x, unbox = T), js )
  
  ## Issue 42
  df <- structure(list(fill_colour = structure(c(68, 49, 53, 253, 1,
  104, 183, 231, 84, 142, 121, 37, 255, 255, 255, 255), .Dim = c(4L,
  4L)), geometry = c(1, 2, -5, 0.3), lat = 1:4, lon = c(1, 2, -5,
  0.3)), class = "data.frame", row.names = c(NA, 4L))

  js <- to_json( df )
  res <- from_json( js )
  expect_equal( res, df )
  
  
  # # Return data frame in which the lengths of the input values are different.
  # target <- data.frame(matrix(nrow = 2, ncol = 2), stringsAsFactors = FALSE)
  # colnames(target) <- c("id", "val")
  # target[["id"]] <- list("a", NA)
  # target[["val"]] <- list(NA, c(1L, 2L, 3L, 4L))
  # 
  # js <- '[{"id":"a"},{"val":[1,2,3,4]}]'
  # expect_equal(from_json(js), target)
  # 
  # js <- jsonify::to_json(target)
  # expect_equal(from_json(js), target)
})

test_that("empty object returns NULL", {
  expect_null(from_json("{}"))
})
test_that("empty array returns list",{
  expect_true(is.list( from_json("[]")) )
})

test_that("empty elements in an array return correct structures",{
  
  ## issue 51
  js <- '[{"test":[]}]'
  res <- from_json( js )
  expect_true( is.data.frame( res ) )
  expect_true( is.list( res$test ) )
  expect_true( length( res$test ) == 1 )
  expect_true( length( res$test[[1]] ) == 0 )
  expect_equal( as.character( to_json( res ) ), js )
  
  js <- '[{"test":{}}]'
  res <- from_json( js )
  expect_equal( as.character( to_json( res ) ), js )
  
  expect_true( is.list( res$test ) )
  expect_true( is.null( res$test[[1]] ))
  
})

test_that("round trips work", {

  f1 <- function( x ) from_json( to_json( x ) )
  f2 <- function( x ) from_json( to_json( x, unbox = TRUE ) )

  x <- 1L
  expect_identical( f1(x), x )
  expect_identical( f2(x), x )

  x <- 1
  expect_identical( f1(x), x )
  expect_identical( f2(x), x )

  x <- 1L:5L
  expect_identical( f1(x), x )
  expect_identical( f2(x), x )

  x <- 1:5
  expect_identical( f1(x), x )
  expect_identical( f2(x), x )

  x <- c(1L, 2L, 3L, NA_integer_)
  expect_identical( f1(x), x )
  expect_identical( f2(x), x )

  x <- list(x = 1)
  expect_identical( f1(x), x )
  expect_identical( f2(x), x )

  x <- list(x = 1:5)
  expect_identical( f1(x), x )
  expect_identical( f2(x), x )

  x <- list(x = 1, y = letters)
  expect_identical( f1(x), x )
  expect_identical( f2(x), x )
  
  x <- data.frame(
    "col1" = c(1, 2),
    "col2" = c(1L, 2L),
    "col3" = c(TRUE, FALSE),
    "col4" = c("cats", "dogs"),
    stringsAsFactors = FALSE
  )
  expect_identical( f1(x), x )

  
  x <- list(1L, "cats", 3L, NA)
  f2 <- function( x ) from_json( to_json( x, unbox = TRUE ), simplify = FALSE )
  expect_identical( f2(x), x )
  
})

test_that("UTF-8 encoding is not mangled", {
  
  testthat::skip(message = "Tests fail on windows")
  
  test_list <- list(
    id = c("回", "站"),
    val = list("收", c("غ ظ ض ذ خ ث ت ش ر ق ص ف ع س ن" ,
                      "م ل ك ي ط ح ز و ه د ج ب أ"))
  )
  
  test_df <- structure(test_list,
                       row.names = 1:2,
                       class = "data.frame")
  
  test_json_df <- '[{"id":"回","val":"收"},{"id":"站","val":["غ ظ ض ذ خ ث ت ش ر ق ص ف ع س ن","م ل ك ي ط ح ز و ه د ج ب أ"]}]'
  
  
  expect_identical(
    from_json(to_json(test_list)),
    test_list
  )
  
  expect_identical(
    from_json(test_json_df),
    test_df
  )
  
  expect_identical(
    from_json(to_json(test_df)),
    test_df
  )
})
