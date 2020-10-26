add_executable(create_data_bin create_data_bin.cpp)
add_executable(create_dictionary create_dictionary.cpp)
add_executable(create_histograms create_histograms.cpp)
add_executable(find_similar_imgs find_similar_imgs.cpp)

target_link_libraries(create_data_bin utils convert_dataset ${OpenCV_LIBS})
target_link_libraries(create_dictionary utils ${OpenCV_LIBS} convert_dataset bow_dictionary histogram)
target_link_libraries(create_histograms utils ${OpenCV_LIBS} convert_dataset bow_dictionary histogram)
target_link_libraries(find_similar_imgs utils ${OpenCV_LIBS} convert_dataset bow_dictionary histogram image_browser)
