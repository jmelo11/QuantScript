cmake_minimum_required(VERSION 3.10)
project(QuantScript LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Look for Boost headers (only needed for brent_find_minima)
find_package(Boost)

# Source files
set(QUANTSCRIPT_SRC
    QuantScript/aad/aad.cpp
    QuantScript/main/QuantScript.cpp
    QuantScript/models/models.cpp
    QuantScript/nodes/nodes.cpp
    QuantScript/parser/parser.cpp
    QuantScript/product/product.cpp
    QuantScript/visitors/debugger.cpp
    QuantScript/visitors/definitionindexer.cpp
    QuantScript/visitors/evaluator.cpp
    QuantScript/visitors/solverevaluator.cpp
    QuantScript/visitors/varindexer.cpp
    QuantScript/visitors/visitor.cpp
)

add_executable(QuantScript ${QUANTSCRIPT_SRC})

target_include_directories(QuantScript PUBLIC
    ${PROJECT_SOURCE_DIR}/QuantScript
)

if(Boost_FOUND)
    target_include_directories(QuantScript PUBLIC ${Boost_INCLUDE_DIRS})
else()
    message(WARNING "Boost not found. Build will fail if required headers are missing.")
endif()

# Link the updated automatic differentiation library
find_package(automatic REQUIRED)
target_include_directories(QuantScript PUBLIC ${automatic_INCLUDE_DIRS})
target_link_libraries(QuantScript PUBLIC ${automatic_LIBRARIES})
