
[![Travis build
status](https://travis-ci.org/SymbolixAU/jsonify.svg?branch=master)](https://travis-ci.org/SymbolixAU/jsonify)
[![Coverage
status](https://codecov.io/gh/SymbolixAU/jsonify/branch/master/graph/badge.svg)](https://codecov.io/github/SymbolixAU/jsonify?branch=master)

# jsonify

jsonify converts a data.frame to JSON.

### Aren’t there already data.frame to JSON converters?

Yep.

### So why did you build this one?

Because I wanted it available at the source ( C++ ) level for
integrating into other packages.

### Is it fast

It does alright

``` r

library(microbenchmark)
library(jsonlite)

n <- 1e6
df <- data.frame(
  id = 1:n
  , value = sample(letters, size = n, replace = T)
  , val2 = rnorm(n = n)
  , log = sample(c(T,F), size = n, replace = T)
)

microbenchmark(
  jsonlite = {
    js <- jsonlite::toJSON( df )
  },
  jsonify = {
    js <- jsonify::to_json( df )
  },
  times = 5
)
#  Unit: seconds
#       expr      min       lq     mean   median       uq      max neval
#   jsonlite 3.784345 3.901516 4.928768 4.787375 5.038220 7.132385     5
#    jsonify 1.962421 2.004886 2.889840 3.009741 3.682872 3.789279     5

n <- 1e6
x <- rnorm(n = n)
microbenchmark(
  jsonlite = {
    js <- jsonlite::toJSON( x )
  },
  jsonify = {
    js <- jsonify::to_json( x )
  },
  times = 5
)
#  Unit: milliseconds
#       expr      min       lq     mean   median       uq      max neval
#   jsonlite 337.7843 357.4031 360.4379 358.4674 371.8704 376.6641     5
#    jsonify 287.1586 293.5292 300.2759 298.1050 300.5953 321.9915     5

n <- 1e4
x <- list(
  x = rnorm(n = n)
  , y = list(x = rnorm(n = n))
  , z = list( list( x = rnorm(n = n)))
  , xx = rnorm(n = n)
  , yy = data.frame(
      id = 1:n
      , value = sample(letters, size = n, replace = T)
      , val2 = rnorm(n = n)
      , log = sample(c(T,F), size = n, replace = T)
    )
)

microbenchmark(
  jsonlite = {
    js <- jsonlite::toJSON( x )
  },
  jsonify = {
    js <- jsonify::to_json( x )
  },
  times = 5
)
#  Unit: milliseconds
#       expr      min       lq     mean   median       uq      max neval
#   jsonlite 36.74542 42.13514 47.74987 46.15347 50.55273 63.16261     5
#    jsonify 13.85580 14.09951 14.89491 15.28431 15.28559 15.94935     5
```

### There’s no ‘Date’ type in JSON, how have you handled this?

At its core `Dates` in R are numeric, so they’re treated as numbers when
converted to JSON. However, I’ve given the option to the user to coerce
to character through the `numeric_dates` argument.

**This argument only works for data.frames and vectors, not lists**

``` r
df <- data.frame(dte = as.Date("2018-01-01"))
jsonify::to_json( df )
#  [{"dte":17532.0}]

df <- data.frame(dte = as.Date("2018-01-01"))
jsonify::to_json( df, numeric_dates = FALSE )
#  [{"dte":"2018-01-01"}]

df <- data.frame(dte = as.POSIXct("2018-01-01 01:01:01"))
jsonify::to_json( df )
#  [{"dte":1514728861.0}]

df <- data.frame(dte = as.POSIXct("2018-01-01 01:01:01"))
jsonify::to_json( df, numeric_dates = FALSE )
#  [{"dte":"2018-01-01 01:01:01"}]
```

### And `POSIXct` and `POSIXlt`?

`POSIXct` work the same as `Date`

``` r
df <- data.frame(dte = as.POSIXct("2018-01-01 10:00:00"))
jsonify::to_json( df )
#  [{"dte":1514761200.0}]
jsonify::to_json( df, numeric_dates = FALSE)
#  [{"dte":"2018-01-01 10:00:00"}]
```

`POSIXlt` will return each component of the date-time

``` r
x <- as.POSIXlt("2018-01-01 01:00:00", tz = "GMT")
jsonify::to_json( x )
#  {"sec":[0.0],"min":[0],"hour":[1],"mday":[1],"mon":[0],"year":[118],"wday":[1],"yday":[0],"isdst":[0]}
jsonify::to_json( x, numeric_dates = FALSE)
#  ["2018-01-01 01:00:00"]
```

### What do you mean by ‘available at the source’ ?

I want to be able to call the C++ code from another package, without
going to & from R. Therefore, the C++ code is imlemented in headers, so
you can `LinkTo` it in your own package.

For example, the LinkingTo section in DESCRIPTION will look something
like

``` r
LinkingTo: 
    Rcpp,
    jsonify
```

And in a c++ source file so you can `#include` the header and use the
available functions

``` cpp
#include "jsonify.hpp"
// [[Rcpp::depends(jsonify)]]

Rcpp::StringVector my_json( Rcpp::DataFrame df ) {
  return jsonify::to_json( df );
}
```

### Can I call it from R if I want to?

Yes.

``` r
df <- data.frame(
  id = 1:3
  , val = letters[1:3]
  , stringsAsFactors = FALSE 
  )
jsonify::to_json( df )
#  [{"id":1,"val":"a"},{"id":2,"val":"b"},{"id":3,"val":"c"}]
```

### How do I install it?

Install the development version from [GitHub](https://github.com/) with:

``` r
# install.packages("devtools")
devtools::install_github("SymbolixAU/jsonify")
```
