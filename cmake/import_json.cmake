
include(FetchContent)

FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    URL_HASH MD5=c23a33f04786d85c29fda8d16b5f0efd
)
FetchContent_MakeAvailable(json)
