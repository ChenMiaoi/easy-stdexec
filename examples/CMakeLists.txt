#enable_testing()

file(GLOB example.scheduler schedulers/*.hpp)

add_library(example INTERFACE ${example.scheduler})

set_target_properties(example PROPERTIES
    VERSION "${STDEXEC_VERSION}"
)

target_include_directories(example INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/examples/>
)

target_compile_features(example INTERFACE cxx_std_20)

function(def_example target source_file)
    add_executable(${target} ${source_file})
    target_link_libraries(${target} stdexec example)
endfunction()

def_example(example.hello_world hello_world.cc)
