context("validate")

test_that("validate works", {
  
  expect_true(validate('[]'))
  expect_true(validate('{}'))
  expect_true(validate('[{}]'))
  expect_true(validate('{"x":1}'))
  expect_true(validate('{"x":"a"}'))
  expect_true(validate('[{"x":1},{"y":[1,2,3,4]}]'))
  
  expect_false(validate(''))
  expect_false(validate('{[]}'))
  expect_false(validate('{"x"}'))
  expect_false(validate('{"x":1]'))
  expect_false(validate('[{"x":1},{"y":[1,2,3,4}]'))
})

