add_library(Engine STATIC
    src/Core/Application.cpp
)

target_include_directories(Engine
    PUBLIC
        include
)