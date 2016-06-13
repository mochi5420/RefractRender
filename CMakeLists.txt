cmake_minimum_required(VERSION 3.0.0)
project(RefractRender)

if (MSVC)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

set(DIRECTX11_DIR ${DIRECTX11_DIR} CACHE PATH "DIRECTX11_NOT-FOUND")
if (NOT EXISTS "${DIRECTX11_DIR}/Include/D3D11.h")
  message(FATAL_ERROR "DirectX 11 not found!!")
else()
  set(DIRECTX11_INCLUDE_DIRS "${DIRECTX11_DIR}/Include")
  file(GLOB DIRECTX11_LIBS "${DIRECTX11_DIR}/Lib/x64/*.lib")
endif()

add_subdirectory(sources)
