set(STB_IMAGE_LOCATION "${CMAKE_SOURCE_DIR}/../../Dependencies/stb_image")

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE "${STB_IMAGE_LOCATION}/include")