add_executable(disbeleaf
    Application.cpp Shader.cpp Texture.cpp ResourceManager.cpp Camera.cpp Player.cpp Alga.cpp Fish.cpp SkyboxRenderer.cpp GameStructure.cpp Model.cpp Mesh.cpp Renderer.cpp Lighting.cpp ../vendor/glad.c
)

target_include_directories(disbeleaf PRIVATE ${VENDORED_INCLUDE_DIR})

target_link_libraries(disbeleaf PRIVATE imgui glfw assimp::assimp OpenGL::GL ${CMAKE_DL_LIBS})

add_dependencies(disbeleaf copy_assets)

# when launched from Visual Studio, run from bin/ so "../assets/..." resolves
set_target_properties(disbeleaf PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${OUTPUT_DIR})
