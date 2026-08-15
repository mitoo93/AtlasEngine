Roadmap definitivo de AtlasEngine


FASE I — Núcleo del motor

Hito 1 — Arquitectura inicial
Estructura del proyecto.
CMake.
Sandbox.
Separación Engine / Sandbox.

Hito 2 — Ventana y contexto gráfico
GLFW.
GLAD.
Window.
Contexto OpenGL.
Main Loop.

Hito 3 — Renderer básico
Frame Loop.
Limpieza de pantalla.
Primer triángulo.
Primer cuadrado.

Hito 4 — Sistema de Shaders
Shader.
Compilación.
Uniforms.
Pipeline básico.

Hito 5 — Sistema Mesh y Buffers
VAO.
VBO.
EBO.
Mesh.
MeshFactory.
BufferLayout.

Hito 6 — Librería matemática
Vector2.
Vector3.
Matrix4.
Transform.
Traslación.
Rotación.
Escala.
Operaciones matriciales.

Hito 7 — Cámara básica
View Matrix.
Projection Matrix.
Perspectiva.
Forward / Right / Up.

Hito 8 — Sistema de tiempo
Time.
DeltaTime.

Hito 9 — Sistema Input
Teclado.
Ratón.
Pressed.
Down.
Released.
Refactorización del Input.

Hito 10 — CameraController
Movimiento libre.
Rotación con ratón.
Navegación FPS.

Hito 11 — Scene Graph
Transform jerárquico.
Parent / Child.
Matriz local.
Matriz mundial.



FASE II — ECS y arquitectura del motor

Hito 12 — ECS básico
Scene.
Entity.
Component.
EntityID.
Transform integrado.
Cámara integrada.

Hito 13 — ECS completo
System.
RenderSystem.
MeshRendererComponent.
MaterialComponent.
CameraComponent.
Cámara primaria.
Actualización de sistemas.

Hito 14 — Sistema de Assets
AssetManager.
Mesh como recurso.
Shader como recurso.
Material como recurso.
Recursos compartidos.

Hito 15 — Serialización
SceneSerializer.
Guardado.
Carga.
Persistencia de entidades.
Transform.
Render components.
Cámara.

Hito 16 — Arquitectura basada en Layers 
Layer.
LayerStack.
GameLayer.
EditorLayer.
Separación Engine / Gameplay.
Separación Engine / Editor.
Eventos.
Render desacoplado de Application.

Hito 17 — Limpieza arquitectónica 
Ownership de LayerStack.
OnAttach.
OnDetach.
Application reducida al núcleo.
Desacoplamiento de sistemas.
Refactorización arquitectónica.



FASE III — Editor

Hito 18 — Dear ImGui
Backend GLFW.
Backend OpenGL.
ImGuiLayer.
Primera interfaz.

Hito 19 — Editor de escenas
Scene Hierarchy.
Inspector.
Selección desde Hierarchy.
Transform editable.
Modos Edit / Play.

Hito 20 — Gizmos
Translate.
Rotate.
Scale.
Local / World.
Manipulación directa de Transform.

Hito 21 — Viewport del editor
Viewport ImGui.
Framebuffer.
Render embebido.
Resize.
Aspect ratio.
Coordenadas del viewport.

Hito 22 — ImGuizmo funcional
Integración de ImGuizmo.
ImGuizmoLayer.
Translate / Rotate / Scale.
Local / World.
IsOver.
IsUsing.
Sincronización con Transform.

Hito 23 — Cámara de editor
EditorCamera independiente.
RMB.
W/A/S/D.
Q/E.
Rotación con ratón.
Pitch limitado.
View / Projection.
Resize según viewport.
Focus.

Hito 24 — Integración Cámara ↔ Gizmo
Cámara completamente inerte sin RMB.
Navegación con RMB.
Cámara bloqueada sobre gizmo.
Gizmo bloquea correctamente la cámara.
Gestión del estado del ratón.
Input arbitrado correctamente.

Hito 25 — Primera estabilización del editor
Correcciones de interacción cámara/gizmo.
Correcciones de focus.
Render del viewport con EditorCamera en Edit.
CameraComponent para Play.
Separación Editor Camera / Runtime Camera.

Hito 26 — Cámara de editor funcional
Movimiento.
Rotación.
Focus.
Integración completa con viewport.
Flujo Edit / Play.

Hito 27 — Estabilización y refactorización
Eliminación de código muerto.
Eliminación de API no utilizada.
Simplificación de Input.
Simplificación de EditorCamera.
Limpieza de Component.
Limpieza de Transform.
Limpieza de Application.
Eliminación del MiniGizmo.
Eliminación de estados innecesarios.
Revisión de includes y dependencias.
Verificación de comportamiento.

Hito 28 — Picking de entidades 
Screen → viewport coordinates.
Screen → NDC.
Ray desde EditorCamera.
Matrix4::Inverse.
Matrix4::TransformPoint.
Ray / AABB.
Selección de entidad por click.
Selección de la entidad más cercana.
Cambio de selección.
Deselección haciendo click en vacío.
Integración con Inspector y Gizmo.

Hito 29 — Asset Browser
Exploración de Assets/.
Carpetas.
Navegación.
Breadcrumb.
Detección de extensiones.
Identificación del tipo de asset.
Drag & Drop.
Payload ASSET_PATH.
Recepción del payload en el viewport.
Panel integrado en el layout del editor.



FASE IV — Recursos y contenido

Hito 30 — Sistema de texturas 
Texture.
Carga de imágenes.
Texturas OpenGL.
Samplers.
UV.
Materiales con textura.
Integración con AssetManager.

Hito 31 — Importación de modelos 
OBJ.
glTF.
GLB.
Importador.
Creación de Mesh.
Materiales importados.
Integración con AssetManager.
Drag & Drop de modelos desde Asset Browser.

Hito 32 — Iluminación 
Normales.
Luz direccional.
Luz puntual.
Luz ambiental.
Materiales iluminados.
Sombras básicas si el alcance lo permite.



FASE V — Renderer avanzado

Hito 33 — Renderer avanzado 
Materiales PBR.
Skybox.
HDR.
Framebuffers avanzados.
Postprocesado.

Hito 34 — Optimización del renderer 
Frustum culling.
Batching.
Instancing.
Resource caching.
Render queues.
Reducción de cambios de estado.
Optimización de RenderSystem.
Optimización de sistemas ECS.



FASE VI — Sistemas del motor

Hito 35 — Audio 
Audio 2D.
Audio 3D.
Gestión de recursos de audio.
Audio asociado a entidades.

Hito 36 — Físicas 
Colisiones.
Collider.
Rigidbody.
Motor físico.
Sincronización Physics ↔ Transform.

Hito 37 — Animación 
Skeleton.
Animaciones esqueléticas.
Animation clips.
Animator.
Blend Trees básicos.

Hito 38 — Scripting 
Sistema de scripts.
Componentes de comportamiento.
Comunicación con ECS.
Ciclo de vida de scripts.

Hito 39 — Prefabs 
Prefabs.
Instanciación.
Modificación de instancias.
Overrides.
Serialización de prefabs.



FASE VII — Pulido y entrega del TFG

Hito 40 — Pulido final 
Optimización final.
Docking.
Menús.
Atajos.
Herramientas del editor.
Gestión de errores.
Limpieza de logs.
UX.
Revisión de dependencias externas.

Hito 41 — Documentación técnica 
Arquitectura.
Renderer.
ECS.
Editor.
Assets.
Input.
Cámara.
Gizmos.
Picking.
Decisiones de diseño.
Diagramas.
Justificación técnica.

Hito 42 — Build final 
Build limpia.
Configuración Release.
Sandbox funcional.
Assets incluidos.
Verificación de dependencias.
Pruebas finales.
Preparación de entrega.

Hito 43 — Memoria del proyecto 
Introducción.
Objetivos.
Estado del arte.
Arquitectura.
Desarrollo.
Resultados.
Pruebas.
Rendimiento.
Conclusiones.
Trabajo futuro.