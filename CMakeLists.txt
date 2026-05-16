cmake_minimum_required(VERSION 3.14)

# Формирование версии проекта
set(PATCH_VERSION "1" CACHE STRING "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VERSION})

# Опции сборки
option(WITH_BOOST_TEST "Build BoostTest tests" ON)
option(WITH_GOOGLE_TEST "Build GoogleTest tests" ON)

if(WITH_BOOST_TEST OR WITH_GOOGLE_TEST)
    enable_testing()
endif()

# Настройки компилятора
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Генерация header файла с версией
configure_file(version.h.in version.h)

#----------------------------------------------------------------------------
# Основа
add_executable(ip_filter_cli main.cpp)
add_library(ip_filter lib.cpp)

target_include_directories(ip_filter
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(ip_filter_cli PRIVATE
    ip_filter
)

#----------------------------------------------------------------------------
# Опции компиляции
add_library(compiler_flags INTERFACE)
if (MSVC)
    target_compile_options(compiler_flags INTERFACE /W4)
else ()
    target_compile_options(compiler_flags INTERFACE -Wall -Wextra -pedantic -Werror)
endif()

target_link_libraries(ip_filter_cli PRIVATE compiler_flags)
target_link_libraries(ip_filter PRIVATE compiler_flags)

#----------------------------------------------------------------------------
# Boost tests
if(WITH_BOOST_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    
    add_executable(test_version btest_version.cpp)

    target_link_libraries(test_version 
        PRIVATE Boost::unit_test_framework ip_filter compiler_flags
    )

    add_test(NAME test_version COMMAND test_version --log_level=all)
endif()

#----------------------------------------------------------------------------
# Google tests
if(WITH_GOOGLE_TEST)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      # URL !!! тут должна быть сслыка на архив
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG v1.16.0 # последняя версия которая поддерживает С++14
    )
    # Исключение тестов самого GoogleTest
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    include(GoogleTest)
    add_executable(test_gtest gtest_version.cpp)

    # Линкуемся с GTest::gtest_main (автоматически добавляет функцию main())
    target_link_libraries(test_gtest 
        PRIVATE GTest::gtest_main ip_filter compiler_flags
    )
    
    gtest_discover_tests(test_gtest)
endif()

#----------------------------------------------------------------------------
# Установка
install(TARGETS ip_filter_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT "example@example.com")
include(CPack)
