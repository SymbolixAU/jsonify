context("matrix")

test_that("matrix converted correctly",{
  expect_equal(as.character(to_json(matrix(letters[1:6], ncol = 3))), "[[\"a\",\"c\",\"e\"],[\"b\",\"d\",\"f\"]]")
  expect_equal(as.character(to_json(matrix(1L:6L,ncol=3))), "[[1,3,5],[2,4,6]]")
  expect_equal(as.character(to_json(matrix(c(1.1,2.1,3.1,4.1,5.1,6.1),ncol=3))), "[[1.1,3.1,5.1],[2.1,4.1,6.1]]")
  
  m <- matrix(c(T,F,T,F))
  expect_equal(as.character(to_json(m, unbox = TRUE)), '[true,false,true,false]')
  expect_equal(as.character(to_json(m, unbox = FALSE)), '[[true],[false],[true],[false]]')
  m <- matrix(c(T,F,T,F),ncol= 2)
  expect_equal(as.character(to_json(m)), '[[true,true],[false,false]]')
  m <- matrix(c(T,F,T,F),ncol= 2,byrow = T)
  expect_equal(as.character(to_json(m)), '[[true,false],[true,false]]')
})
