cmake_minimum_required(VERSION 3.2)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(vixen-helper)
include("${VITASDK}/share/vita.cmake" REQUIRED)

add_executable(${PROJECT_NAME}
  src/main.c
  src/controller.c
)

target_link_libraries(${PROJECT_NAME}
  SceDebugForDriver_stub
  SceSblAIMgrForDriver_stub
  SceIofilemgrForDriver_stub
  SceCtrlForDriver_stub
  SceKernelSuspendForDriver_stub
  SceSysclibForDriver_stub
  SceSysmemForDriver_stub
  SceThreadmgrForDriver_stub
  SceUsbdForDriver_stub
  SceUsbServForDriver_stub
  taihenForKernel_stub
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES LINK_FLAGS "-nostdlib"
)

vita_create_self(${PROJECT_NAME}.skprx ${PROJECT_NAME}
  CONFIG vixen-helper.yml
  UNSAFE
)

vita_create_stubs(stubs vixen-helper ${CMAKE_CURRENT_SOURCE_DIR}/vixen-helper.yml KERNEL)

add_custom_target(vixen-helper_all
  ALL
  DEPENDS libvixen-helper_stub.a
  DEPENDS libvixen-helper_stub_weak.a
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/stubs/
  DESTINATION lib
  FILES_MATCHING PATTERN "*.a"
)

install(FILES vixen-helper.h
  DESTINATION include
)
