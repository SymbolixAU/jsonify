context("validate")

test_that("validate works", {
  
  expect_true(validate_json('[]'))
  expect_true(validate_json('{}'))
  expect_true(validate_json('[{}]'))
  expect_true(validate_json('{"x":1}'))
  expect_true(validate_json('{"x":"a"}'))
  expect_true(validate_json('[{"x":1},{"y":[1,2,3,4]}]'))
  
  expect_false(validate_json(''))
  expect_false(validate_json('{[]}'))
  expect_false(validate_json('{"x"}'))
  expect_false(validate_json('{"x":1]'))
  expect_false(validate_json('[{"x":1},{"y":[1,2,3,4}]'))
})

