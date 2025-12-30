test_that("toml is parsed correctly from a string", {
  toml_str <- '# This is a TOML document

title = "TOML Example"

[owner]
name = "Tom Preston-Werner"
dob = 1979-05-27T07:32:00-08:00

[database]
enabled = true
ports = [ 8000, 8001, 8002 ]
data = [ ["delta", "phi"], [3.14] ]
temp_targets = { cpu = 79.5, case = 72.0 }

[servers]

[servers.alpha]
ip = "10.0.0.1"
role = "frontend"

[servers.beta]
ip = "10.0.0.2"
role = "backend"  
'

  out <- parse_toml(toml_str)
  expect_equal(out[["title"]], "TOML Example")
  expect_equal(names(out), c("title", "owner", "database", "servers"))
  expect_equal(names(out[["servers"]]), c("alpha", "beta"))
  expect_equal(
    out[["owner"]],
    list(
      name = "Tom Preston-Werner",
      dob = structure(
        c(1979, 5, 27, 7, 32, 0, 0, -480),
        class = "rtoml_datetime"
      )
    )
  )
})

test_that("invalid toml is not parsed", {
  expect_error(parse_toml("this is not valid"))
  expect_error(parse_toml("wrong = 2025-2"))
})

test_that("toml can be parsed from files", {
  file <- withr::local_tempfile(lines = c(
    'title = "test"',
    '',
    '[details]',
    'type = "package"',
    'array = [{"a" = 1}, {"b" = 2}]',
    'valid = true'
  ))
  
  x <- read_toml(file)

  y <- list(
    title = "test",
    details = list(
      type = "package",
      array = list(list(a = 1), list(b = 2)),
      valid = TRUE
    )
  )
  
  expect_equal(x, y)
})

test_that("large integers are parsed as doubles", {
  x <- parse_toml("numbers = { too_big = 2147483648, ok = 2147483647 }")
  expect_identical(x$numbers$ok, 2147483647L)
  expect_identical(x$numbers$too_big, 2147483648)
})

test_that("dates are handled correctly", {
  x <- parse_toml('
    time = 17:43:00
    local_date = 2025-12-30
    local_datetime = 2025-12-30T17:43:00
    datetime = 2025-12-30T17:43:00Z
    datetime_in_paris = 2025-12-30T18:43:00+01:00
  ')
  
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
    structure(c(2025L, 12L, 30L, 17L, 43L, 0L, 0L), class = "rtoml_local_datetime")
  )
  
  expect_identical(
    x$datetime, 
    structure(c(2025L, 12L, 30L, 17L, 43L, 0L, 0L, 0L), class = "rtoml_datetime")
  )
  
  expect_identical(
    x$datetime_in_paris,
    structure(c(2025L, 12L, 30L, 18L, 43L, 0L, 0L, 60L), class = "rtoml_datetime")
  )
})
