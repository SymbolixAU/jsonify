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

# to_json
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

# from_json
js <- jsonify::to_json( x )
microbenchmark(
  jsonlite = {
    r_val <- jsonlite::fromJSON( js )
  }, 
  jsonify = {
    r_val <- jsonify::from_json( js )
  }, 
  times = 5
)
# Unit: milliseconds
#     expr       min         lq       mean     median         uq        max neval
# jsonlite 9962.3905 10075.0774 10157.5579 10076.4016 10308.3503 10365.5699     5
#  jsonify  822.6131   831.9874   883.8569   845.1197   943.5952   975.9688     5

## numeric vector with NAs
n <- 1e6
x <- rnorm(n = n)
x[sample(1:n,size = n/3)] <- NA_real_

# to_json
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

# from_json
js <- jsonify::to_json( x )
microbenchmark(
  jsonlite = {
    r_val <- jsonlite::fromJSON( js )
  },
  jsonify = {
    r_val <- jsonify::from_json( js )
  },
  times = 5
)
# Unit: milliseconds
#     expr      min        lq     mean    median        uq       max neval
# jsonlite 749.2141 754.33312 775.3778 757.38981 757.92618 858.02579     5
#  jsonify  73.4771  74.24309  75.7850  76.84927  77.06383  77.29172     5

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

# to_json
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

# from_json
js <- jsonify::to_json(df)
microbenchmark(
  jsonlite = {
    r_val <- jsonlite::fromJSON( js )
  },
  jsonify = {
    r_val <- jsonify::from_json( js )
  },
  times = 5
)
# Unit: seconds
#     expr       min        lq      mean    median       uq       max neval
# jsonlite 104.84647 109.82142 120.44753 115.25022 126.4798 145.83968     5
#  jsonify  12.13313  17.33051  25.97161  17.44538  27.2728  55.67621     5


## forcing 

library(microbenchmark)
library(jsonlite)

n <- 1e7
df <- data.frame(
  id = 1:n
  , value = sample(letters, size = n, replace = T)
  , val2 = rnorm(n = n)
  , log = sample(c(T,F), size = n, replace = T)
  #, dte = sample(seq(as.Date("2018-01-01"), as.Date("2018-01-31"), length.out = n), size = n, replace = T)
)

microbenchmark(
  force = {
    js <- jsonify::to_json( df, force = TRUE )
  },
  dontforce = {
    js <- jsonify::to_json( df, force = FALSE )
  },
  times = 5
)
# Unit: seconds
#       expr      min       lq     mean   median       uq      max neval
#     force 9.889725 10.03111 10.23549 10.05032 10.38913 10.81715     5
# dontforce 9.981074 10.02546 10.16957 10.12311 10.25727 10.46095     5

