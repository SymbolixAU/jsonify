# 
# library(jsonlite)
# 
# json <- "~/Desktop/google_timeline/Takeout/Location History/Location History.json"
# 
# js <- jsonlite::fromJSON(json)
# #
# # js2 <- jsonify::from_json(json)
# 
# json_small <- "~/Desktop/google_timeline/Takeout/Location History/Location History Small.json"
# jfy <- from_json( json_small, simplify = T, fill_na = T)
# jlt <- fromJSON( json_small )
# 
# ls <- readLines(json)
# 
# js <- paste0(ls, collapse = "")
# 
# #jsonify::validate_json( js )
# 
# res <- jsonify::from_json( js, fill_na = T )
# 
