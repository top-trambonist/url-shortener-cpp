cmake_minimum_required(VERSION 3.20)

project(UrlShortener LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(Crow CONFIG REQUIRED)

find_package(unofficial-sqlite3 CONFIG REQUIRED)


add_executable(UrlShortener

    src/UrlShortener.cpp
    src/Shortener.cpp
    src/UrlRepository.cpp

)


target_include_directories(
    UrlShortener
    PRIVATE
    src
)


target_link_libraries(
    UrlShortener
    PRIVATE

    Crow::Crow
    unofficial::sqlite3::sqlite3
)


if(MSVC)

    target_compile_definitions(
        UrlShortener
        PRIVATE
        _CRT_SECURE_NO_WARNINGS
    )

    target_compile_options(
        UrlShortener
        PRIVATE
        /utf-8
    )

    set_property(
        TARGET UrlShortener
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
        "${CMAKE_SOURCE_DIR}"
    )

endif()