context("lists")

test_that("empty lists are handled", {
  expect_equal( as.character(to_json(list())), "[]")
  expect_equal( as.character(to_json(list(x = 1, y = list()))), "{\"x\":[1.0],\"y\":[]}")
  expect_true( validate_json( to_json( list(x = 1, y = list()))))
})

test_that("list structures jsonified", {
  lst <- list(x = 1, y = c("a","b","c"), z = list("hello", b = "world"))
  expect_equal( as.character(to_json( lst )), "{\"x\":[1.0],\"y\":[\"a\",\"b\",\"c\"],\"z\":{\"\":[\"hello\"],\"b\":[\"world\"]}}")
  expect_true( validate_json( to_json(lst) ) ) 
})


test_that("empty lists are handled and unboxed", {
  expect_equal( as.character(to_json(list(), unbox = T)), "[]")
  expect_equal( as.character(to_json(list(x = 1, y = list()), unbox = T)), "{\"x\":1.0,\"y\":[]}")
  expect_true( validate_json( to_json( list(x = 1, y = list() ), unbox = T ) ) )
})

test_that("list structures jsonified and unboxed", {
  lst <- list(x = 1, y = c("a","b","c"), z = list("hello", b = "world"))
  expect_equal( as.character(to_json( lst, unbox = T )), "{\"x\":1.0,\"y\":[\"a\",\"b\",\"c\"],\"z\":{\"\":\"hello\",\"b\":\"world\"}}")
  expect_true( validate_json( to_json( lst, unbox = T) ) ) 
})


test_that("matrices inside lists are correctly converted", {
  
  m1 <- matrix(1:4, ncol = 2)
  l <- list( x = 1L, y = m1 )
  js <- to_json( l, unbox = F )
  expected <- '{"x":[1],"y":[[1,3],[2,4]]}'
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), expected )
  
  js <- to_json( l, unbox = T )
  expect_true( validate_json( js ) )
  expected <- '{"x":1,"y":[[1,3],[2,4]]}'
  expect_equal( as.character( js ), expected )
  
  m <- matrix(c(T,F,T,F))
  l <- list( m )
  js <- to_json( l )
  expect_true( validate_json( js ) )
  expected <- '[[[true],[false],[true],[false]]]'
  expect_equal( as.character( js ) , expected )
  
  js <- to_json( l, unbox = TRUE )
  expect_true( validate_json( js ) )
  expected <- '[[true,false,true,false]]'
  expect_equal( as.character( js ) , expected )
  
  m1 <- matrix(1:4, ncol = 2)
  m2 <- matrix(c("a","b","c","d","e","f"), ncol = 3)
  l <- list( x = m1, y = m2 )
  js <- to_json( l )
  expect_true( validate_json( js ) )
  expected <- '{"x":[[1,3],[2,4]],"y":[["a","c","e"],["b","d","f"]]}'
  expect_equal( as.character( js ), expected )
  
  m1 <- matrix(1:4, ncol = 2)
  m2 <- matrix(c("a","b","c","d","e","f"), ncol = 3)
  m3 <- matrix(1)
  l <- list( x = m1, y = m2, m3 )
  js <- to_json( l )
  expect_true( validate_json( js ) )
  expected <- '{"x":[[1,3],[2,4]],"y":[["a","c","e"],["b","d","f"]],"":[[1.0]]}'
  expect_equal( as.character( js ), expected )
  
  m1 <- matrix(1:4, ncol = 2)
  m2 <- matrix(c("a","b","c","d","e","f"), ncol = 3)
  m3 <- matrix(1)
  l <- list( x = m1, y = m2, m3 )
  js <- to_json( l, unbox = TRUE )
  expect_true( validate_json( js ) )
  expected <- '{"x":[[1,3],[2,4]],"y":[["a","c","e"],["b","d","f"]],"":[1.0]}'
  expect_equal( as.character( js ), expected )
  
})

test_that("data.frames inside lists are converted correctly", {
  
  df <- data.frame(id = 1:2, val = c("a","b"), stringsAsFactors = F)
  l <- list( x = df )
  js <- to_json( l )
  expected <- '{"x":[{"id":1,"val":"a"},{"id":2,"val":"b"}]}'
  expect_true( validate_json( js ) )
  expect_equal( as.character( js ), expected )
})

test_that("factors in lists become characters", {
  
  l <- list( x = as.factor( letters[1:3] ) )
  l2 <- list( x = letters[1:3] )
  js <- to_json( l )
  expect_true( js == to_json( l2 ) )
  
  js <- to_json( l, factors_as_string = FALSE )
  l2 <- list( x = 1:3 )
  expect_true( js == to_json( l2 ) )
})

