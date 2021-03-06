# requires minimum cmake version
cmake_minimum_required(VERSION 3.5)

# produce the cmake var PROJECT_NAME
project(assignment_producer-consumer)

# mandatory use of these packages
find_package(YARP 3.1.101 REQUIRED)
find_package(ICUBcontrib REQUIRED)

# extend the current search path used by cmake to load helpers
list(APPEND CMAKE_MODULE_PATH ${ICUBCONTRIB_MODULE_PATH})

# helpers defining certain macros
include(ICUBcontribHelpers)
include(ICUBcontribOptions)

# specify to install in $ICUBcontrib_DIR/bin
icubcontrib_set_default_prefix()

# building client
add_executable(producer ${CMAKE_SOURCE_DIR}/src/producer.cpp)
target_compile_definitions(producer PRIVATE _USE_MATH_DEFINES)
target_link_libraries(producer ${YARP_LIBRARIES})
install(TARGETS producer DESTINATION bin)

# building server
add_executable(consumer ${CMAKE_SOURCE_DIR/}src/consumer.cpp)
target_compile_definitions(consumer PRIVATE _USE_MATH_DEFINES)
target_link_libraries(consumer ${YARP_LIBRARIES})
install(TARGETS consumer DESTINATION bin)

# generate ad-hoc project to perform "make uninstall"
icubcontrib_add_uninstall_target()

file(GLOB scripts ${CMAKE_SOURCE_DIR}/app/scripts/*.xml)
yarp_install(FILES ${scripts} DESTINATION ${ICUBCONTRIB_APPLICATIONS_INSTALL_DIR})
