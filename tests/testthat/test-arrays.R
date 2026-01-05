test_that("simple arrays are flattened", {
  x <- parse_toml("x = [1, 2, 3]")
  expect_identical(x, list(x = c(1L, 2L, 3L)))

  x <- parse_toml("x = [1.0, 2.0, 3.0]")
  expect_identical(x, list(x = c(1, 2, 3)))

  x <- parse_toml("x = [true, false, true]")
  expect_identical(x, list(x = c(TRUE, FALSE, TRUE)))

  x <- parse_toml("x = ['a', 'b', 'cde']")
  expect_identical(x, list(x = c("a", "b", "cde")))

  # is this the right thing to do?
  x <- parse_toml("x = [1, 1.0, 1]")
  expect_identical(x, list(x = list(1L, 1, 1L)))
})

test_that("flattening can be disabled", {
  x <- parse_toml("x = [1, 2, 3]", flatten = FALSE)
  expect_identical(x, list(x = list(1L, 2L, 3L)))
  
  x <- parse_toml("x = [1.0, 2.0, 3.0]", flatten = FALSE)
  expect_identical(x, list(x = list(1, 2, 3)))
  
  x <- parse_toml("x = [true, false, true]", flatten = FALSE)
  expect_identical(x, list(x = list(TRUE, FALSE, TRUE)))
  
  x <- parse_toml("x = ['a', 'b', 'cde']", flatten = FALSE)
  expect_identical(x, list(x = list("a", "b", "cde")))
})
