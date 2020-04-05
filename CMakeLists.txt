cmake_minimum_required(VERSION 3.5)
project(flashloantest VERSION 1.0.0)

find_package(eosio.cdt)

add_contract(flashloantest flashloantest ${CMAKE_CURRENT_SOURCE_DIR}/src/flashloantest.cpp)
target_include_directories(flashloantest
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../eosio.system/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../eosio.token/include)

set_target_properties(flashloantest
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

#target_ricardian_directory( flashloantest ${CMAKE_CURRENT_SOURCE_DIR}/ricardian )