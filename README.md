
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
#  jsonlite 2.017081 2.063732 2.540350 2.110383 2.801984 3.493585     3
#   jsonify 1.186239 1.202719 1.514067 1.219198 1.677981 2.136763     3

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
#  jsonlite 18.52028 18.82241 19.32112 18.99683 19.18103 21.08508     5
#   jsonify 17.72060 18.19092 19.58308 19.52457 21.14687 21.33241     5
   
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
#          expr       min        lq      mean    median        uq       max neval
#  jsonlite_row 20.533642 20.717894 27.294220 21.122860 21.426250 52.670456     5
#  jsonlite_col 13.691643 13.812459 15.683795 14.293177 15.655705 20.965993     5
#   jsonify_row 17.506507 17.951948 20.929641 19.827791 21.161389 28.200572     5
#   jsonify_col  7.262305  7.382238  7.409085  7.434759  7.435476  7.530645     5
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
#       expr       min        lq     mean    median        uq       max neval
#   jsonify1  57.61869  60.75988  63.8870  63.90106  67.02115  70.14124     3
#   jsonify2 308.03909 314.36571 317.2089 320.69232 321.79381 322.89529     3
#   jsonlite 692.80273 698.84349 714.4054 704.88426 725.20679 745.52933     3
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
