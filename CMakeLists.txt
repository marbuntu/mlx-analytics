
include_directories(MLX_LOGGING_HDRS)

file(GLOB
    MLX_ANALYTICS_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/structures/mlx-vector.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/operations/mlx-operators.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/wavelets/mlx-wvt-gauss.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mlx-fft.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/mlx-cwt.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/mlx-sos-filter.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/mlx-gaussian-filter.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/mlx-analytics.cc
)


file(GLOB
    MLX_ANALYTICS_HDR_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}
)


add_library(
    mlx_analytics
    ${MLX_ANALYTICS_SRC_FILES}
    ${MLX_ANALYTICS_HDR_FILES}
)

target_link_libraries(mlx_analytics PRIVATE
    gsl
)


set(MLX_ANALYTICS_HDRS ${MLX_ANALYTICS_HDR_FILES} CACHE INTERNAL "MLX_ANALYTICS_HDRS")




# Build Test 