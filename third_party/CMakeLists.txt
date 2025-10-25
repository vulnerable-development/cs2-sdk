include(cpm/cpm.cmake)

CPMAddPackage(
    NAME json
    GITHUB_REPOSITORY nlohmann/json
    GIT_TAG master
    OPTIONS "JSON_BuildTests OFF"
)

CPMAddPackage(
    NAME minhook
    GITHUB_REPOSITORY TsudaKageyu/minhook
    GIT_TAG master
)

CPMAddPackage(
    NAME imgui
    GITHUB_REPOSITORY ocornut/imgui
    GIT_TAG v1.92.4
)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0)

CPMAddPackage(
    NAME protobuf
    GITHUB_REPOSITORY protocolbuffers/protobuf
    GIT_TAG v3.21.8
    OPTIONS
    "protobuf_BUILD_TESTS OFF"
    "protobuf_BUILD_SHARED_LIBS OFF"
    "protobuf_MSVC_STATIC_RUNTIME OFF"
    "protobuf_WITH_ZLIB OFF"
    "protobuf_DISABLE_RTTI ON"
    "protobuf_USE_STATIC_LIBS ON"
    "protobuf_HAVE_BUILTIN_ATOMICS ON"
)

set(PROTOBUF_DIR ${CMAKE_SOURCE_DIR}/third_party/protocolbuffers/proto)
file(GLOB_RECURSE PROTOBUF_FILES ${PROTOBUF_DIR}/*.proto)

set(PROTOBUF_OUTDIR ${CMAKE_SOURCE_DIR}/third_party/protocolbuffers/generated)
file(MAKE_DIRECTORY ${PROTOBUF_OUTDIR})

foreach(FILE ${PROTOBUF_FILES})
    get_filename_component(FILENAME ${FILE} NAME_WE)

    set(GEN_SRC "${PROTOBUF_OUTDIR}/${FILENAME}.pb.cc")
    set(GEN_HDR "${PROTOBUF_OUTDIR}/${FILENAME}.pb.h")

    add_custom_command(
        OUTPUT ${GEN_SRC} ${GEN_HDR}
        COMMAND $<TARGET_FILE:protobuf::protoc> --cpp_out=lite:${PROTOBUF_OUTDIR} --proto_path=${PROTOBUF_DIR} --proto_path=${protobuf_SOURCE_DIR}/src ${FILE}
        DEPENDS ${FILE}
    )

    list(APPEND GENERATED_PB_FILES ${GEN_SRC} ${GEN_HDR})
endforeach()

add_custom_target(protobuf_generate ALL DEPENDS ${GENERATED_PB_FILES})

add_library(protobuf STATIC ${GENERATED_PB_FILES})
add_dependencies(protobuf protobuf_generate)

target_link_libraries(protobuf PUBLIC protobuf::libprotobuf-lite)
target_include_directories(protobuf PUBLIC ${PROTOBUF_OUTDIR} ${PROTOBUF_DIR})