project(rdfs)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl -lssl -lcrypto")

set(CMAKE_CXX_FLAGS "-Wall -DELPP_FEATURE_PERFORMANCE_TRACKING" )
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Protobuf REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(Zookeeper REQUIRED)
find_package(ISAL REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

include_directories(/usr/include/asio)

add_subdirectory(proto)
include_directories(${ProtobufIncludePath})

add_subdirectory(utility)
add_subdirectory(isal)
add_subdirectory(rpcserver)
add_subdirectory(rice-namenode)
add_subdirectory(zookeeper)
add_subdirectory(zkwrapper)
add_subdirectory(rice-datanode)
add_subdirectory(native-filesystem)
add_subdirectory(test)
add_subdirectory(storage)
add_subdirectory(web-rdfs)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/hdfs-default.xml
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/utility/config-reader/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/nn-log-conf.conf
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/rice-namenode/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/dn-log-conf.conf
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/rice-datanode/)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/hdfs-default.xml
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/rice-namenode/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/hdfs-default.xml
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/rice-datanode/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/hdfs-default.xml
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test/)
