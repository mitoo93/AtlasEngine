add_library(Engine STATIC

    src/Core/Application.cpp
    src/Core/Window.cpp
    src/Core/Input.cpp
    src/Core/Time.cpp
    src/Core/Log.cpp

    src/Renderer/Renderer.cpp
    src/Renderer/RenderCommand.cpp
    src/Renderer/Shader.cpp
    src/Renderer/VertexArray.cpp
    src/Renderer/VertexBuffer.cpp
    src/Renderer/IndexBuffer.cpp
    src/Renderer/Mesh.cpp
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