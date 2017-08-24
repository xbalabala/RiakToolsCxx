file(GLOB SOURCES "*.cpp")
add_executable(riakcompact ${SOURCES})

target_link_libraries(riakcompact libleveldb
  -lpthread -lz
  ${Boost_LIBRARIES}
)


set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

install(TARGETS riakcompact DESTINATION "/bin")

SET(CPACK_GENERATOR "DEB")
 
SET(CPACK_PACKAGE_DESCRIPTION "C++ leveldb Riak tools")
SET(CPACK_PACKAGE_VENDOR "Helm & Walter IT-Solitions GbR")
SET(CPACK_PACKAGE_CONTACT "daniel.walter@helmundwalter.de")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
#dependencies for this service menu
SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
 
INCLUDE(CPack)
