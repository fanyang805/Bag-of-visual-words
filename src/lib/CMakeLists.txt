add_library(convert_dataset serialize.cpp convert_dataset.cpp)
add_library(utils utils.cpp)
add_library(bow_dictionary bowdictionary.cpp kMeans.cpp)
add_library(histogram histogram.cpp)
add_library(image_browser html_writer.cpp image_browser.cpp)

target_link_libraries(convert_dataset ${OpenCV_LIBS})
target_link_libraries(utils ${OpenCV_LIBS} convert_dataset  image_browser)
target_link_libraries(bow_dictionary ${OpenCV_LIBS} utils)
target_link_libraries(histogram bow_dictionary)

