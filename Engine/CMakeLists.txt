add_library(Engine STATIC

    src/Core/Application.cpp
    src/Core/Window.cpp
    src/Core/Input.cpp
    src/Core/Time.cpp
    src/Core/Layer.cpp
    src/Core/LayerStack.cpp

    src/Renderer/Renderer.cpp
    src/Renderer/RenderCommand.cpp
    src/Renderer/Shader.cpp
    src/Renderer/VertexArray.cpp
    src/Renderer/VertexBuffer.cpp
    src/Renderer/IndexBuffer.cpp
    src/Renderer/Mesh.cpp
    src/Renderer/BufferLayout.cpp
    src/Renderer/MeshFactory.cpp
    src/Renderer/Camera.cpp
    src/Renderer/Framebuffer.cpp
    src/Renderer/Texture.cpp
    src/Renderer/Model.cpp
    src/Renderer/ShadowMap.cpp

    src/Math/Matrix4.cpp
    src/Math/Transform.cpp

    src/Controllers/CameraController.cpp

    src/Scene/Entity.cpp
    src/Scene/Scene.cpp
    src/Scene/Component.cpp

    src/Systems/RenderSystem.cpp

    src/Assets/AssetManager.cpp
    src/Assets/ModelImporter.cpp

    src/Serialization/SceneSerializer.cpp

    src/Gameplay/GameLayer.cpp

    src/Editor/EditorLayer.cpp
    src/Editor/EditorCamera.cpp
    src/Editor/AssetBrowser.cpp

    src/ImGui/ImGuiLayer.cpp
    src/ImGuizmo/ImGuizmoLayer.cpp
)

target_include_directories(Engine
    PUBLIC
        include
)

target_link_libraries(Engine
    PUBLIC
        glfw
        glad
        imgui
        imguizmo
        stb
        assimp
)