add_library(ip_filter_lib STATIC
    source/ip_address.cpp
    source/ip_filter.cpp
)

target_link_libraries(ip_filter_lib PRIVATE compiler_flags)

target_include_directories(ip_filter_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
