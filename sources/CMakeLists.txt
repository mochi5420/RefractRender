set(BUILD_TARGET "RefractRenderMain")

configure_file("${CMAKE_CURRENT_LIST_DIR}/common.h.in"
               "${CMAKE_CURRENT_LIST_DIR}/common.h" @ONLY)

set(SOURCES ${SOURCES}
    common.h debug.h init.h
    main.cpp main.h
    mesh.cpp readcsv.cpp)

set(SHADERS ${SHADERS}
    sss.hlsl drawtex.hlsl)

set_source_files_properties(${SHADERS} PROPERTIES VS_SHADER_TYPE Effect
                            VS_SHADER_MODEL 5.0 VX_SHADER_ENTRY_POINT main)

include_directories(${DIRECTX11_INCLUDE_DIRS})
add_executable(${BUILD_TARGET} WIN32 ${SOURCES} ${SHADERS})

target_link_libraries(${BUILD_TARGET} ${DIRECTX11_LIBS})

if (MSVC)
  source_group("Source Files" FILES ${SOURCES})
  source_group("Shader Files" FILES ${SHADERS})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
  set_property(TARGET ${BUILD_TARGET} APPEND PROPERTY
               LINK_FLAS "/DEBUG /PROPERTY")
endif()
