cmake_minimum_required(VERSION 3.14)

# set the project name
set(project_name "ip_filter")

# Формирование версии проекта
set(PATCH_VERSION "1" CACHE STRING "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

#----------------------------------------------------------------------------
# Опции сборки
# option(WITH_BOOST_TEST "Build BoostTest tests" ON)
option(WITH_GOOGLE_TEST "Build GoogleTest tests" ON)

if(WITH_BOOST_TEST OR WITH_GOOGLE_TEST)
    enable_testing()
endif()

#----------------------------------------------------------------------------
# Project
project(${project_name} 
        VERSION ${PROJECT_VERSION}
        LANGUAGES C CXX)

# Настройки компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Отключаем compiler-specific расширения для чистой переносимости

# Опции компиляции
add_library(compiler_flags INTERFACE)
if (MSVC)
    target_compile_options(compiler_flags INTERFACE /W4)
else ()
    target_compile_options(compiler_flags INTERFACE -Wall -Wextra -pedantic -Werror)
endif()

#target_link_libraries(ip_filter_cli PRIVATE compiler_flags)
#target_link_libraries(${project_name} PRIVATE compiler_flags)

# Подключаем папки с исходным кодом и тестами
add_subdirectory(source)
add_subdirectory(tests)

#----------------------------------------------------------------------------
# Установка и создание DEB пакета через CPack
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT "example@example.com")
include(CPack)
