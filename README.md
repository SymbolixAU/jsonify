
[![R build
status](https://github.com/SymbolixAU/jsonify/workflows/R-CMD-check/badge.svg)](https://github.com/SymbolixAU/jsonify/actions)
[![CRAN\_Status\_Badge](http://www.r-pkg.org/badges/version/jsonify)](https://CRAN.R-project.org/package=jsonify)
![downloads](http://cranlogs.r-pkg.org/badges/grand-total/jsonify)
[![CRAN RStudio mirror
downloads](http://cranlogs.r-pkg.org/badges/jsonify)](https://CRAN.R-project.org/package=jsonify)

# jsonify

Converts between R objects and JSON.

``` r
js <- '[{"x":1,"y":"a"},{"x":2,"y":"b"}]'
( df <- from_json( js ) )
#    x y
#  1 1 a
#  2 2 b
( to_json( df ) )
#  [{"x":1,"y":"a"},{"x":2,"y":"b"}]
```

### There are already JSON converters, why did you build this one?

Because I wanted it available at the source ( C++ ) level for
integrating into other packages.

### What do you mean by “available at the source” ?

I want to be able to call the C++ code from another package, without
going to & from R. Therefore, the C++ code is implemented in headers, so
you can “link to” it in your own package.

For example, the `LinkingTo` section in **DESCRIPTION** will look
something like

``` yaml
LinkingTo: 
    Rcpp,
    jsonify
```

And in a c++ source file you can `#include` the header and use the
available functions

``` cpp
// [[Rcpp::depends(jsonify)]]
#include "jsonify/jsonify.hpp"

Rcpp::StringVector my_json( Rcpp::DataFrame df ) {
  return jsonify::api::to_json( df );
}
```

### Can I call it from R if I want to?

Yes. Just like the examples in this readme use `to_json()`

``` r
df <- data.frame(
  id = 1:3
  , val = letters[1:3]
  )
jsonify::to_json( df )
#  [{"id":1,"val":"a"},{"id":2,"val":"b"},{"id":3,"val":"c"}]
```

### Is it fast?

yeah it’s pretty good.

``` r

library(microbenchmark)
library(jsonlite)

n <- 1e6
df <- data.frame(
  id = 1:n
  , value = sample(letters, size = n, replace = T)
  , val2 = rnorm(n = n)
  , log = sample(c(T,F), size = n, replace = T)
  , stringsAsFactors = FALSE
)

microbenchmark(
  jsonlite = {
    jlt <- jsonlite::toJSON( df )
  },
  jsonify = {
    jfy <- jsonify::to_json( df )
  },
  times = 3
)

# Unit: seconds
#      expr      min       lq     mean   median       uq      max neval
#  jsonlite 2.017081 2.063732 2.540350 2.110383 2.801984 3.493585     3
#   jsonify 1.186239 1.202719 1.514067 1.219198 1.677981 2.136763     3


microbenchmark(
  jsonlite = {
    df_jlt <- jsonlite::fromJSON( jlt )
  },
  jsonify = {
    df_jfy <- jsonify::from_json( jfy )
  },
  times = 3
)

# Unit: seconds
#      expr      min       lq     mean   median       uq      max neval
#  jsonlite 5.034888 5.149688 5.229363 5.264489 5.326601 5.388713     3
#   jsonify 4.551434 4.629683 4.678198 4.707932 4.741579 4.775227     3

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
    jlt <- jsonlite::toJSON( x )
  },
  jsonify = {
    jfy <- jsonify::to_json( x )
  },
  times = 5
)
 
# Unit: milliseconds
#      expr      min       lq     mean   median       uq      max neval
#  jsonlite 18.52028 18.82241 19.32112 18.99683 19.18103 21.08508     5
#   jsonify 17.72060 18.19092 19.58308 19.52457 21.14687 21.33241     5
   

microbenchmark(
  jsonlite = {
    df_jlt <- jsonlite::fromJSON( jlt )
  },
  jsonify = {
    df_jfy <- jsonify::from_json( jfy )
  },
  times = 3
)

# Unit: milliseconds
#      expr      min       lq     mean   median       uq      max neval
#  jsonlite 62.53554 62.96435 63.12574 63.39316 63.42084 63.44853     3
#   jsonify 42.47449 42.53826 43.38475 42.60204 43.83988 45.07773     3
```

### There’s no `Date` type in JSON, how have you handled this?

At its core `Dates` in R are numeric, so they are treated as numbers
when converted to JSON. However, the user can coerce to character
through the `numeric_dates` argument.

``` r
df <- data.frame(dte = as.Date("2018-01-01"))
jsonify::to_json( df )
#  [{"dte":17532.0}]

df <- data.frame(dte = as.Date("2018-01-01"))
jsonify::to_json( df, numeric_dates = FALSE )
#  [{"dte":"2018-01-01"}]
```

### And `POSIXct` and `POSIXlt`?

The same

``` r

jsonify::to_json( as.POSIXct("2018-01-01 10:00:00") )
#  [1514761200.0]
jsonify::to_json( as.POSIXct("2018-01-01 10:00:00"), numeric_dates = FALSE)
#  ["2017-12-31T23:00:00"]
```

However, here the **POSIXct** values are returned in UTC timezone. This
is by design.

**POSIXlt** will return each component of the date-time

``` r
x <- as.POSIXlt("2018-01-01 01:00:00", tz = "GMT")
jsonify::to_json( x )
#  {"sec":[0.0],"min":[0],"hour":[1],"mday":[1],"mon":[0],"year":[118],"wday":[1],"yday":[0],"isdst":[0]}

jsonify::to_json( x, numeric_dates = FALSE)
#  {"sec":[0.0],"min":[0],"hour":[1],"mday":[1],"mon":[0],"year":[118],"wday":[1],"yday":[0],"isdst":[0]}
```

### I see factors are converted to strings

Yep. Even though I constructed a `data.frame()` without setting
`stringsAsFactros = FALSE`, jsonify automatically treats factors as
strings.

### Has this changed from v0.1?

Yes. And it’s to keep the data more inline with modern concepts and
design patterns.

If you want factors, use `factors_as_string = FALSE` in the `to_json()`
call

``` r
jsonify::to_json( df, factors_as_string = FALSE )
#  [{"dte":17532.0}]
```

### How do I install it?

Get the latest release version from CRAN

``` r
install.packages("jsonify")
```

Or the development version from [GitHub](https://github.com/) with:

``` r
# install.packages("devtools")
devtools::install_github("SymbolixAU/jsonify")
```
