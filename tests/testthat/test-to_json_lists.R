context("lists")

test_that("empty lists are handled", {
  expect_equal( as.character(to_json(list())), "[]")
  expect_equal( as.character(to_json(list(x = 1, y = list()))), "{\"x\":[1.0],\"y\":[]}")
  expect_true( jsonlite::validate( to_json( list(x = 1, y = list()))))
})

test_that("list structures jsonified", {
  lst <- list(x = 1, y = c("a","b","c"), z = list("hello", b = "world"))
  expect_equal( as.character(to_json( lst )), "{\"x\":[1.0],\"y\":[\"a\",\"b\",\"c\"],\"z\":{\"1\":[\"hello\"],\"b\":[\"world\"]}}")
  expect_true( jsonlite::validate( to_json(lst) ) ) 
})


test_that("empty lists are handled and unboxed", {
  expect_equal( as.character(to_json(list(), unbox = T)), "[]")
  expect_equal( as.character(to_json(list(x = 1, y = list()), unbox = T)), "{\"x\":1.0,\"y\":[]}")
  expect_true( jsonlite::validate( to_json( list(x = 1, y = list() ), unbox = T ) ) )
})

test_that("list structures jsonified and unboxed", {
  lst <- list(x = 1, y = c("a","b","c"), z = list("hello", b = "world"))
  expect_equal( as.character(to_json( lst, unbox = T )), "{\"x\":1.0,\"y\":[\"a\",\"b\",\"c\"],\"z\":{\"1\":\"hello\",\"b\":\"world\"}}")
  expect_true( jsonlite::validate( to_json( lst, unbox = T) ) ) 
})