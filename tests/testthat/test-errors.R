test_that("bad inputs throw errors", {
  expect_error(read_toml("notafile"))
  expect_error(read_toml(c("too.toml", "many.toml", "files.toml")))
  expect_error(parse_toml(c("a", "b", "c")))
  expect_error(parse_toml("a = 1", flatten = "no"))
})
