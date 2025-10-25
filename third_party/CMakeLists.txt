include(cpm/cpm.cmake)

CPMAddPackage(
    NAME nlohmann_json
    GITHUB_REPOSITORY nlohmann/json
    GIT_TAG master
    OPTIONS "JSON_BuildTests OFF"
)