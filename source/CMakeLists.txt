add_library(${project_name}_lib STATIC
    ip_address.h
    ip_address.cpp
    ip_filter.h
    ip_filter.cpp
)

# Настраиваем флаги компиляции для библиотеки
target_link_libraries(${project_name}_lib PRIVATE compiler_flags)

# PUBLIC гарантирует, что любой, кто линкуется с ip_filter_lib,
# автоматически получит доступ к заголовочным файлам из этой папки
target_include_directories(${project_name}_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Создаем исполняемый файл приложения
add_executable(${project_name}
    main.cpp
)

# Просто линкуем наше приложение с созданной библиотекой
target_link_libraries(${project_name} PRIVATE ${project_name}_lib compiler_flags)

# Установка
install(TARGETS ${project_name} RUNTIME DESTINATION bin)
