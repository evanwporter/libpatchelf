set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PAGESIZE 4096)

add_library(libpatchelf STATIC patchelf.cc elf.h patchelf.h libpatchelf.h)
set_target_properties(libpatchelf PROPERTIES OUTPUT_NAME patchelf)
target_include_directories(libpatchelf PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

add_executable(patchelf patchelf-main.cc)
target_link_libraries(patchelf PRIVATE libpatchelf)

target_compile_definitions(
  libpatchelf PRIVATE PAGESIZE=${PAGESIZE}
                   PACKAGE_STRING="patchelf ${VERSION_STRING}")

install(TARGETS patchelf libpatchelf
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES libpatchelf.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
