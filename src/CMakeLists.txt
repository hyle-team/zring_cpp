file(GLOB CRYPTO crypto/*.*)

source_group(crypto FILES ${CRYPTO})

add_library(crypto ${CRYPTO})

set_property(TARGET crypto PROPERTY FOLDER "libs")

if(MSVC)
  set_source_files_properties("crypto/chacha8_stream.c" PROPERTIES COMPILE_FLAGS "/wd4018")
else()
  set_source_files_properties("crypto/chacha8_stream.c" PROPERTIES COMPILE_FLAGS "-Wno-sign-compare -Wno-strict-prototypes")
endif()

if(APPLE)
  set(SIMPLE_BUNDLE 1)
endif()

if(SIMPLE_BUNDLE)
  set(INSTALL_DIR "${CMAKE_BINARY_DIR}/hp-${VERSION}")

  install(FILES ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} DESTINATION "${INSTALL_DIR}/lib")

  if(APPLE)
    set(FIXUP_COMMAND ${CMAKE_SOURCE_DIR}/utils/macosx_fixup.sh " " ${INSTALL_DIR})
    install(CODE "execute_process(COMMAND ${FIXUP_COMMAND})")
  endif()
endif()
