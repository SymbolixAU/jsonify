context("SEXPTYPE")



test_that("SEXPTYPES are convertedt to JSON", {
  
  ## closure & language
  js <- to_json( to_json, unbox = TRUE )
  expect_equal( as.character( js ), '{"x":"","unbox":false,"digits":{},"numeric_dates":true,"factors_as_string":true,"by":"row","":["{",["if",["%in%","col","by"],["<-","by","column"]],["<-","by",{"":"match.arg","":"by","choices":["c","row","column"]}],["<-","digits",["handle_digits","digits"]],["rcpp_to_json","x","unbox","digits","numeric_dates","factors_as_string","by"]]}')
  expect_true( validate_json( js ) ) 
  
  ## builtinsxp
  js <- to_json( c, unbox = TRUE)
  expect_equal( as.character( js ), '[{}]')
  expect_true( validate_json( js ) ) 
  
  ## special
  js <- to_json( quote )
  expect_equal( as.character( js ), '[{}]')
  expect_true( validate_json( js ) ) 
  
  ## clouse & language
  js <- to_json( print, unbox = TRUE )
  expect_equal( as.character( js ), '{"x":"","...":"","":["UseMethod","print"]}')
  expect_true( validate_json( js ) ) 
  
  ## envsxp
  # rm(list=ls());gc()
  # js <- to_json( .GlobalEnv, unbox = TRUE )
  # expect_equal( as.character( js ), '[]')
  # expect_true( validate_json( js ) ) 
  
  
  
})
