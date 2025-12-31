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
  file <- withr::local_tempfile(
    lines = c(
      'title = "test"',
      '',
      '[details]',
      'type = "package"',
      'array = [{"a" = 1}, {"b" = 2}]',
      'valid = true'
    )
  )

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
