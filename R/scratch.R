# 
# library(sf)
# 
# nc <- st_read(system.file("shape/nc.shp", package="sf"))
# 
# library(googlePolylines)
# 
# enc <- encode( nc )
# 
# js <- jsonify::to_json( enc )
# 
# library(microbenchmark)
# 
# 
# 
# microbenchmark(
#   jsonlite = { jsonlite::toJSON( enc ) },
#   jsonify = { jsonify::to_json( enc ) },
#   times = 25
# )
# 
# myurl <- "http://eric.clst.org/assets/wiki/uploads/Stuff/gz_2010_us_050_00_500k.json"
# geo <- readLines(myurl)
# geo <- paste0(geo, collapse = "")
# 
# sf <- geojsonsf::geojson_sf( geo )
# enc <- googlePolylines::encode( sf )
# 
# microbenchmark(
#   jsonlite = { jsonlite::toJSON( enc ) },
#   jsonify = { jsonify::to_json( enc ) },
#   times = 25
# )

# df <- data.frame(
#   id = 1:3
#   , norm = c(1.1, 2.2, 3.3)
#   , letters = c("a","x","B")
#   , val = I(list(c(0,0),1,list(c(2,3), myname = c(4,5), c("a"))))
#  , stringsAsFactors = F)
# 
# jsonify::to_json( df )
# 
# jsonlite::toJSON( df )

# df <- data.frame(
#   id = 1:3
#   , norm = c(1.1, 2.2, 3.3)
#   , letters = c("a","x","B")
#   , val = I(list(c(0,0),1,list(c(2,3), c(4,5), c("a"))))
#  , stringsAsFactors = F)
# 
# jsonify::to_json( df )
# 
# jsonlite::toJSON( df )


# lst <- list(x = 1:5, c("a","b"), y = list("z"), z = list(a = list( b = list( d = 20)), ab = 1))
# 
# jsonlite::toJSON( lst )
# 
# to_json( lst )
# 
# 
# jsonlite::toJSON( lst$z )
# jsonify::to_json( lst$z )

# plt <- as.POSIXlt("2018-01-01 01:00:00", tz = "GMT")
# 
# str( plt )
# jsonlite::toJSON( plt, POSIXt = "string") 
# jsonlite::toJSON( plt, POSIXt = "ISO8601") 
# jsonlite::toJSON( plt, POSIXt = "epoch") 
# jsonlite::toJSON( plt, POSIXt = "mongo") 




