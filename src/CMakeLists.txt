project(server LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB INCLUDE_FILES
    *.h
    **/*.h
    **/**/*.h
    **/**/**/*.h
    **/**/**/**/*.h
)
file(GLOB SOURCE_FILES
    *.cpp
    **/*.cpp
    **/**/*.cpp
    **/**/**/*.cpp
    **/**/**/**/*.cpp
)
add_executable(${PROJECT_NAME}
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
)

if (MSVC)
    add_definitions(/EHsc)
else ()
    add_definitions(-fexceptions)
endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 11
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    NOMINMAX
    FMT_HEADER_ONLY
    WIN32_LEAN_AND_MEAN 
    SPDLOG_FMT_EXTERNAL
    CPPHTTPLIB_OPENSSL_SUPPORT
)

if (CONAN_EXPORTED)
    include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else ()
    include(${CMAKE_CURRENT_SOURCE_DIR}/conan.cmake)
    conan_cmake_configure(REQUIRES
            fmt/10.2.1
            magic_enum/0.9.6
            mariadb-connector-c/3.3.8
            openssl/3.2.2
            libcurl/8.10.1
	    sqlpp11/0.64
            date/3.0.1
            cpp-httplib/0.18.1
            spdlog/1.14.1
        GENERATORS CMakeDeps CMakeToolchain
        IMPORTS "bin, *.dll -> ./bin"
        OPTIONS
            fmt/*:shared=True
            openssl/*:shared=True
            libcurl/*:shared=True
            mariadb-connector-c/*:shared=True)

    conan_cmake_autodetect(settings)
    conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        REMOTE conancenter)
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}")
set(DEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../bin")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}" ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}" ${CMAKE_PREFIX_PATH})
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

find_package(fmt REQUIRED)
if (fmt_FOUND)
    message(STATUS "Found fmt: ${fmt_VERSION}, at ${fmt_INCLUDE_DIRS}, ${fmt_LIBRARIES}")
    target_include_directories(${PROJECT_NAME} PUBLIC ${fmt_INCLUDE_DIRS})
else ()
    message(FATAL_ERROR "Could not find fmt")
endif ()

find_package(httplib REQUIRED)
if (httplib_FOUND)
    message(STATUS "Found httplib: ${httplib_VERSION}, at ${httplib_INCLUDE_DIRS}, ${httplib_LIBRARIES}")
    target_include_directories(${PROJECT_NAME} PUBLIC ${httplib_INCLUDE_DIRS})
else ()
    message(FATAL_ERROR "Could not find httplib")
endif ()

find_package(mariadb-connector-c REQUIRED)
if (mariadb-connector-c_FOUND)
    message(STATUS "Found mariadb-connector-c: ${mariadb-connector-c_VERSION}, at ${mariadb-connector-c_INCLUDE_DIRS}, ${mariadb-connector-c_LIBRARIES}")
    target_include_directories(${PROJECT_NAME} PUBLIC ${mariadb-connector-c_INCLUDE_DIRS})
else ()
    message(FATAL_ERROR "Could not find mariadb-connector-c")
endif ()

find_package(date REQUIRED)
if (date_FOUND)
    message(STATUS "Found date: ${date_VERSION}, at ${date_INCLUDE_DIRS}, ${date_LIBRARIES}")
    target_include_directories(${PROJECT_NAME} PUBLIC ${date_INCLUDE_DIRS})
else ()
    message(FATAL_ERROR "Could not find date")
endif ()

find_package(Sqlpp11 REQUIRED)
if (Sqlpp11_FOUND)
    message(STATUS "Found Sqlpp11: ${Sqlpp11_VERSION}, at ${Sqlpp11_INCLUDE_DIRS}, ${Sqlpp11_LIBRARIES}")
    target_include_directories(${PROJECT_NAME} PUBLIC ${Sqlpp11_INCLUDE_DIRS})
else ()
    message(FATAL_ERROR "Could not find Sqlpp11")
endif ()

find_package(magic_enum REQUIRED)
if (magic_enum_FOUND)
    message(STATUS "Found magic_enum: ${magic_enum_VERSION}, at ${magic_enum_INCLUDE_DIRS}, ${magic_enum_LIBRARIES}")
    target_include_directories(${PROJECT_NAME} PUBLIC ${magic_enum_INCLUDE_DIRS})
else ()
    message(FATAL_ERROR "Could not find magic_enum")
endif ()

find_package(spdlog REQUIRED)
if (spdlog_FOUND)
    message(STATUS "Found spdlog: ${spdlog_VERSION}, at ${spdlog_INCLUDE_DIRS}, ${spdlog_LIBRARIES}")
    target_include_directories(${PROJECT_NAME} PUBLIC ${spdlog_INCLUDE_DIRS})
else ()
    message(FATAL_ERROR "Could not find spdlog")
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/sigslot/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/enet/include
    ${HTTPLIB_INCLUDE_DIR}
    ${Sqlpp11_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    fmt::fmt
    magic_enum::magic_enum
    spdlog::spdlog
    httplib::httplib
    mariadb-connector-c::mariadb-connector-c
    sqlpp11::sqlpp11
    date::date
    Pal::Sigslot
    enet
)
