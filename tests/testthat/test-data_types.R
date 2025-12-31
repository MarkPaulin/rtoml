test_that("floats are parsed as doubles", {
  expect_equal(parse_toml("a = 2.2"), list(a = 2.2))
})

test_that("large integers are parsed as doubles", {
  x <- parse_toml("numbers = { too_big = 2147483648, ok = 2147483647 }")
  expect_identical(x$numbers$ok, 2147483647L)
  expect_identical(x$numbers$too_big, 2147483648)
})

test_that("dates are handled correctly", {
  x <- parse_toml(
    '
    time = 17:43:00
    local_date = 2025-12-30
    local_datetime = 2025-12-30T17:43:00
    datetime = 2025-12-30T17:43:00Z
    datetime_in_paris = 2025-12-30T18:43:00+01:00
  '
  )

  expect_identical(
    x$time,
    structure(c(17L, 43L, 0L, 0L), class = "rtoml_time")
  )

  expect_identical(
    x$local_date,
    structure(c(2025L, 12L, 30L), class = "rtoml_local_date")
  )

  expect_identical(
    x$local_datetime,
    structure(
      c(2025L, 12L, 30L, 17L, 43L, 0L, 0L),
      class = "rtoml_local_datetime"
    )
  )

  expect_identical(
    x$datetime,
    structure(
      c(2025L, 12L, 30L, 17L, 43L, 0L, 0L, 0L),
      class = "rtoml_datetime"
    )
  )

  expect_identical(
    x$datetime_in_paris,
    structure(
      c(2025L, 12L, 30L, 18L, 43L, 0L, 0L, 60L),
      class = "rtoml_datetime"
    )
  )
})
