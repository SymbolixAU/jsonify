## TODO
## - lists
## - POSIXlt
## - POSIXct
## - Date


library(microbenchmark)
library(jsonlite)

n <- 1e6
df <- data.frame(
  id = 1:n
  , value = sample(letters, size = n, replace = T)
  , val2 = rnorm(n = n)
  , log = sample(c(T,F), size = n, replace = T)
  #, dte = sample(seq(as.Date("2018-01-01"), as.Date("2018-01-31"), length.out = n), size = n, replace = T)
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

## numeric vector
n <- 1e7
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
# Unit: seconds
# expr      min       lq     mean   median       uq      max neval
# jsonlite 1.750814 1.894219 2.096893 2.034916 2.250064 2.554454     5
#  jsonify 1.732922 1.746326 1.856985 1.805632 1.820807 2.179239     5

## numeric vector with NAs
n <- 1e6
x <- rnorm(n = n)
x[sample(1:n,size = n/3)] <- NA_real_
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
# expr      min       lq     mean   median       uq      max neval
# jsonlite 142.9747 160.1157 161.9641 163.9898 169.7590 172.9811     5
#  jsonify 127.8681 128.6028 129.6287 130.0981 130.2327 131.3419     5


## mixture of NAs, NULLs, Infs etc
n <- 1e7
df <- data.frame(
  id = 1:n
  , value = sample(letters, size = n, replace = T)
  , val2 = rnorm(n = n)
  , log = sample(c(T,F), size = n, replace = T)
  #, dte = sample(seq(as.Date("2018-01-01"), as.Date("2018-01-31"), length.out = n), size = n, replace = T)
)

df[sample(1:n, size = n / 3), 'id'] <- NA_integer_
df[sample(1:n, size = n / 3), 'val2'] <- NA_real_
df[sample(1:n, size = n / 3), 'log'] <- NA

microbenchmark(
  jsonlite = {
    js <- jsonlite::toJSON( df )
  },
  jsonify = {
    js <- jsonify::to_json( df )
  },
  times = 5
)

# Unit: seconds
#     expr       min        lq     mean   median       uq      max neval
# jsonlite 18.518717 23.558636 24.79996 24.47630 24.77023 32.67592     5
#  jsonify  9.265547  9.407197 13.29851 12.50761 17.08521 18.22700     5