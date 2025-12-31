
<!-- README.md is generated from README.Rmd. Please edit that file -->

# rtoml

<!-- badges: start -->

[![R-CMD-check](https://github.com/MarkPaulin/rtoml/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/MarkPaulin/rtoml/actions/workflows/R-CMD-check.yaml)
[![Codecov test
coverage](https://codecov.io/gh/MarkPaulin/rtoml/graph/badge.svg)](https://app.codecov.io/gh/MarkPaulin/rtoml)
<!-- badges: end -->

rtoml is a wrapper around [tomlc17](https://github.com/cktan/tomlc17), a
TOML parser written in C.

## Installation

You can install the development version of rtoml with:

``` r
# install.packages("pak")
pak::pak("MarkPaulin/rtoml")
```

## Usage

Convert TOML documents into lists:

``` r
library(rtoml)

x <- parse_toml(
  '
title = "rtoml!"
today = 2025-12-30

[package]
status = "In development"

[package.todo]
dates = "Convert to R date types"
arrays = "Flatten them into vectors"
'
)

# or...
# x <- read_toml("file.toml")

str(x)
#> List of 3
#>  $ title  : chr "rtoml!"
#>  $ today  : 'rtoml_local_date' int [1:3] 2025 12 30
#>  $ package:List of 2
#>   ..$ status: chr "In development"
#>   ..$ todo  :List of 2
#>   .. ..$ dates : chr "Convert to R date types"
#>   .. ..$ arrays: chr "Flatten them into vectors"
```

## Limitations

TOML has some features that aren’t completely compatible with R. Some of
these I’m planning on addressing, some of them are just not manageable
at all. On top of those, this package will only read TOML - the
underlying library doesn’t support generating TOML files.

### Integers

TOML supports 64-bit integers, which are bigger than R’s 32-bit
integers. Any integers outside this range are coerced to numerics.

### Dates and times

TOML has four date- and time-related types. Base R doesn’t have a type
for times, but there are classes for dates and date-times. Currently,
rtoml parses and dates, times, and date-times into custom types, but
could be converted into more common types. Times could be optionally
converted to `hms::hms()`.

RcppTOML converts dates into `Date`, and both local date-times and
date-times to `POSIXct`, and times are converted to text. tomledit
leaves all dates and times as text.

## Extra tests

I’m working on testing this against the [standard test
suite](https://github.com/toml-lang/toml-test). Results are [in this
repo](https://github.com/MarkPaulin/rtoml-test).
