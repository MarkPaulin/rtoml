
<!-- README.md is generated from README.Rmd. Please edit that file -->

# rtoml

<!-- badges: start -->

[![R-CMD-check](https://github.com/MarkPaulin/rtoml/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/MarkPaulin/rtoml/actions/workflows/R-CMD-check.yaml)
[![Codecov test
coverage](https://codecov.io/gh/MarkPaulin/rtoml/graph/badge.svg)](https://app.codecov.io/gh/MarkPaulin/rtoml)
<!-- badges: end -->

The goal of rtoml is to …

## Installation

You can install the development version of rtoml from
[GitHub](https://github.com/) with:

``` r
# install.packages("pak")
pak::pak("MarkPaulin/rtoml")
```

## Example

This is a basic example which shows you how to solve a common problem:

``` r
library(rtoml)
## basic example code
```

What is special about using `README.Rmd` instead of just `README.md`?
You can include R chunks like so:

``` r
summary(cars)
#>      speed           dist       
#>  Min.   : 4.0   Min.   :  2.00  
#>  1st Qu.:12.0   1st Qu.: 26.00  
#>  Median :15.0   Median : 36.00  
#>  Mean   :15.4   Mean   : 42.98  
#>  3rd Qu.:19.0   3rd Qu.: 56.00  
#>  Max.   :25.0   Max.   :120.00
```

You’ll still need to render `README.Rmd` regularly, to keep `README.md`
up-to-date. `devtools::build_readme()` is handy for this.

You can also embed plots, for example:

<img src="man/figures/README-pressure-1.png" width="100%" />

In that case, don’t forget to commit and push the resulting figure
files, so they display on GitHub and CRAN.
