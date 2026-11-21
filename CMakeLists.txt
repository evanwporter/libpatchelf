cmake_minimum_required(VERSION 3.5)

project(patchelf)

file(READ version VERSION)
string(STRIP "${VERSION}" VERSION_STRING)

include(GNUInstallDirs)

add_subdirectory(src)
include(CTest)
if(BUILD_TESTING)
  add_library(libpatchelf_fixture SHARED tests/libpatchelf-fixture.c)
  set_target_properties(libpatchelf_fixture PROPERTIES
    OUTPUT_NAME patchelf-fixture SOVERSION 1)
  add_executable(libpatchelf_api_test tests/libpatchelf-api.cc)
  target_link_libraries(libpatchelf_api_test PRIVATE libpatchelf)
  add_test(NAME libpatchelf_api COMMAND libpatchelf_api_test
    $<TARGET_FILE:libpatchelf_fixture> libpatchelf-fixture.so.1)
endif()
# add_subdirectory(tests) # TODO

install(FILES patchelf.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

install(FILES README.md DESTINATION ${CMAKE_INSTALL_DOCDIR})

install(FILES completions/zsh/_patchelf
        DESTINATION ${CMAKE_INSTALL_DATADIR}/zsh/site-functions)
