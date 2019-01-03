
[![CRAN\_Status\_Badge](http://www.r-pkg.org/badges/version/jsonify)](https://CRAN.R-project.org/package=jsonify)
![downloads](http://cranlogs.r-pkg.org/badges/grand-total/jsonify)
[![CRAN RStudio mirror
downloads](http://cranlogs.r-pkg.org/badges/jsonify)](https://CRAN.R-project.org/package=jsonify)
[![Travis build
status](https://travis-ci.org/SymbolixAU/jsonify.svg?branch=master)](https://travis-ci.org/SymbolixAU/jsonify)
[![Coverage
status](https://codecov.io/gh/SymbolixAU/jsonify/branch/master/graph/badge.svg)](https://codecov.io/github/SymbolixAU/jsonify?branch=master)

# jsonify

jsonify converts R objects to JSON.

### There are already JSON converters, why did you build this one?

Because I wanted it available at the source ( C++ ) level for
integrating into other packages.

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
    js <- jsonlite::toJSON( df )
  },
  jsonify = {
    js <- jsonify::to_json( df )
  },
  times = 3
)

# Unit: seconds
#      expr      min       lq     mean   median       uq      max neval
#  jsonlite 4.204652 4.327804 5.367306 4.450957 5.948634 7.446310     3
#   jsonify 2.437399 3.256403 4.723197 4.075407 5.866097 7.656786     3

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
 
# Unit: milliseconds
#      expr      min       lq     mean   median       uq      max neval
#  jsonlite 34.51573 37.85838 39.06570 39.86556 40.15261 42.93620     5
#   jsonify 33.27312 35.57159 36.40071 36.80128 37.76846 38.58911     5
   
```

### I thought you had an example of it being MUCH quicker than `jsonlite` ?

Yeah, but I realised it was comparing two different methods. When
`jsonify` was parsing nested lists, it was parsing data.frames
column-wise, whereas jsonlite was row-wise. Which is a slower operation

### Oh right. So it wasn’t a fair test then.

Correct.

Here’s a more suitable comparison

``` r
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
  jsonlite_row = {
    js <- jsonlite::toJSON( x )
  },
  jsonlite_col = {
    js <- jsonlite::toJSON( x, dataframe = "columns" )
  },
  jsonify_row = {
    js <- jsonify::to_json( x )
  },
  jsonify_col = {
    js <- jsonify::to_json( x, by = "column" )
  },
  times = 5
)

# Unit: milliseconds
#          expr      min       lq     mean   median       uq      max neval
#  jsonlite_row 34.93630 35.17458 41.44464 36.74871 47.34143 53.02217     5
#  jsonlite_col 24.93064 27.78434 30.88678 28.55122 30.07938 43.08830     5
#   jsonify_row 33.62929 35.90877 36.78444 36.43218 38.16549 39.78647     5
#   jsonify_col 12.91980 14.91945 16.20517 17.16689 17.28481 18.73488     5
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

### What about lists?

~~The purpose of this library is speed. A lot of overhead is incurred
iterating over a list to find and convert objects from one type to
another.~~

For v0.2.0 I’ve managed to get the date handling at the c++ level, so
there’s no penalty for recursing through the list to coerce to
character.

Therefore, lists will work too

``` r
l <- list(
  dte = as.Date("2018-01-01")
  , psx = seq(as.POSIXct("2018-01-01 13:00:00"), as.POSIXct("2018-01-05 13:00:00"), length.out = 5)
  , df = data.frame(psx = seq(as.POSIXct("2018-01-01 13:00:00"), as.POSIXct("2018-01-05 13:00:00"), length.out = 5))
)
jsonify::to_json( l )
#  {"dte":[17532.0],"psx":[1514772000,1514858400,1514944800,1515031200,1515117600],"df":[{"psx":1514772000},{"psx":1514858400},{"psx":1514944800},{"psx":1515031200},{"psx":1515117600}]}
```

``` r
js <- jsonify::to_json( l, numeric_dates = FALSE )
jsonify::pretty_json( js )
#  {
#      "dte": [
#          "2018-01-01"
#      ],
#      "psx": [
#          "2018-01-01T02:00:00",
#          "2018-01-02T02:00:00",
#          "2018-01-03T02:00:00",
#          "2018-01-04T02:00:00",
#          "2018-01-05T02:00:00"
#      ],
#      "df": [
#          {
#              "psx": "2018-01-01T02:00:00"
#          },
#          {
#              "psx": "2018-01-02T02:00:00"
#          },
#          {
#              "psx": "2018-01-03T02:00:00"
#          },
#          {
#              "psx": "2018-01-04T02:00:00"
#          },
#          {
#              "psx": "2018-01-05T02:00:00"
#          }
#      ]
#  }
```

And it’s still fast because of the design choice to coerce dates to UTC.
All the date handling is done at the C++ leve, not R. So it’s
faster.

``` r
dtes <- seq(as.Date("2018-01-01"), as.Date("2019-01-01"), length.out = 365)
psx <- seq(as.POSIXct("2018-01-01"), as.POSIXct("2019-01-01"), length.out = 365)
n <- 1e5

lst <- list(
  x = sample(dtes, size = n, replace = T)
  , y = list(
    ya = sample(dtes, size = n, replace = TRUE)
    , yb = rnorm(n = n)
    , yx = list( sample(dtes, size = n, replace = T ) )
  )
  , p = psx
)

library( microbenchmark )

microbenchmark(
  jsonify1 = {
    jsonify::to_json( lst, numeric_dates = TRUE )
  },
  jsonify2 = {
    jsonify::to_json( lst, numeric_dates = FALSE )
  },
  jsonlite = {
    jsonlite::toJSON( lst )
  },
  times = 3
)
#  Unit: milliseconds
#       expr       min        lq      mean    median        uq       max
#   jsonify1  136.0230  142.6240  159.1319  149.2250  170.6864  192.1478
#   jsonify2  910.7382  912.7783  970.9419  914.8184 1001.0437 1087.2691
#   jsonlite 1758.9345 1951.1584 2027.1144 2143.3824 2161.2044 2179.0264
#   neval
#       3
#       3
#       3
```

### That output looks nice, is that `pretty_json()` function new?

Yep, it’s a new feature in v0.2.0

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
#  [{"id":1,"val":1},{"id":2,"val":2},{"id":3,"val":3}]
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
