#----------------------------------------------------------------------------
# Boost tests
#if(WITH_BOOST_TEST)
#    find_package(Boost COMPONENTS unit_test_framework REQUIRED)

#    add_executable(test_version btest_version.cpp)

#    target_link_libraries(test_version
#        PRIVATE Boost::unit_test_framework ip_filter compiler_flags
#    )

#    add_test(NAME test_version COMMAND test_version --log_level=all)
#endif()

#----------------------------------------------------------------------------
# Google tests
if(WITH_GOOGLE_TEST)
    include(FetchContent)

    FetchContent_Declare(
      googletest
      # URL !!! тут должна быть сслыка на архив
      GIT_REPOSITORY https://github.com/google/googletest.git
      # GIT_TAG v1.16.0 # последняя версия которая поддерживает С++14
      GIT_TAG v1.17.0
    )

    # Исключение тестов самого GoogleTest
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    include(GoogleTest)

    add_executable(test_gtest
        gtest_ip_address.cpp
        gtest_ip_filter.cpp
    )

    # Линкуемся с GTest::gtest_main (автоматически добавляет функцию main())
    target_link_libraries(test_gtest
        PRIVATE GTest::gtest_main ${project_name}_lib compiler_flags
    )

    gtest_discover_tests(test_gtest)
 endif()
