fixture = shared_library('patchelf-fixture', '../libpatchelf-fixture.c', soversion : '1')
api_test = executable('libpatchelf-api-test', '../libpatchelf-api.cc',
  include_directories : include_directories('../../src'),
  link_with : patchelf_lib)
test('libpatchelf API', api_test, args : [fixture, 'libpatchelf-fixture.so.1'])
