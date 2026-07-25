add_library(Engine STATIC

    src/Core/Application.cpp
    src/Core/Window.cpp
    src/Core/Input.cpp
    src/Core/Time.cpp
    src/Core/Log.cpp

)

target_include_directories(Engine
    PUBLIC
        include
)

target_link_libraries(Engine
    PUBLIC
        glfw
        glad
)