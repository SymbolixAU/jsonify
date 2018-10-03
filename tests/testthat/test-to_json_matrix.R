context("matrix")

test_that("matrix stops",{
  expect_error(to_json(matrix()))
})