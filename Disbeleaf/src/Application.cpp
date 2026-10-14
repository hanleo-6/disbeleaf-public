#pragma once
// DISBELEAF. Application file
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// matrix operations
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include <glm/gtx/string_cast.hpp>

// UI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <string>

#include "GameObject.h"
#include "Renderer.h"
#include "Player.h"
#include "Camera.h"
#include "Alga.h"
#include "Fish.h"
#include "SkyboxRenderer.h"
#include "GameStructure.h"

using namespace std;

class Application {
    public:
        GameStructure * game_structure = nullptr;
        Player * player = nullptr;
        Camera * camera = nullptr;
        AlgaeManager * algae_manager = nullptr;
        FishManager * fish_manager = nullptr;

        bool Initialise(); 
        bool IsRunning();
        void MainLoop();
        void Terminate();
        // recompute projection from framebuffer size & push to all shaders
        void UpdateProjection(int width, int height);

        // mouse position for calculating camera movement when mouse moves
        float last_x;
        float last_y;
        float x_offset;
        float y_offset;

        // flags used to determine app state
        bool flag_MouseDown = false;
        bool flag_initial_mouse = true; // for when window initially loads and mouse pointer is outside of it, orient view correctly
        bool flag_pause_open = false;
        bool to_close_menu = false;
        bool to_exit_app = false;
        // Dev mode disabled for Release version
        bool flag_dev_mode = false;
        bool flag_cursor_normal = false; // for imgui fish settings menu

        // time tracking for FPS & game logic processing
        float cur_time;
        float last_time;
        float dt;
        const float FPS_LIM = 60.0f;

    private:
        void DefineUI();
        
        GLFWwindow* window;
        int window_width, window_height;

        // GAME SETTINGS
        const int NUM_ALGAE = 500;
        const int NUM_FISH = 100;
        
        // Model & skybox renderers
        ModelRenderer * playerRenderer = nullptr;
        ModelRenderer * fishRenderer = nullptr;
        ModelRenderer * algaRenderer = nullptr;
        ModelRenderer * sunRenderer = nullptr;
        SkyboxRenderer * skyboxRenderer = nullptr;

        // Lighting
        LightingEngine * global_lighting = nullptr;

        // file paths
        const char* model_vert_shader = "../assets/shaders/model_vert.glsl";
        const char* model_frag_shader = "../assets/shaders/model_frag.glsl";
        const char*  skybox_vert_shader = "../assets/shaders/skybox_vert.glsl";
        const char* skybox_frag_shader = "../assets/shaders/skybox_frag.glsl";

        // const string player_model_scene = "../assets/textures/seacreature2/scene.gltf";
        // const string player_model_scene = "../assets/textures/paintslug/paintslug_test.glb";
        const string player_model_scene = "../assets/textures/paintslug/elysia_medres.glb";
        // const string player_model_scene = "../assets/textures/paintslug/slug400k.glb";

        const string alga_model_scene = "../assets/textures/alga/scene.gltf";
        const string fish_model_scene = "../assets/textures/fish/scene.gltf";
        const string sun_model_scene = "../assets/textures/sun/sun.glb";

        ImFont * menu_font = nullptr;
        ImFont * score_font = nullptr;
        const char* font_path = "../assets/textures/Stepalange.otf";
};

//////////////////////////////////////////////////////////////////
// MAIN DRIVER CODE
//////////////////////////////////////////////////////////////////
Application app;
int main(int, char **) {
    // Initialise application, checking if it succeeded
    if (!app.Initialise()) {
        cout << "App failed during initialisation" << "\n";
        return -1;
    }
    // Run Application's main loop until it is stopped for any reason
    while (app.IsRunning()) {
        app.MainLoop();
    }
    // Clean up app and terminate program
    app.Terminate();
    return 0;
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

// callback initialisations
static void glfw_error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_move_callback(GLFWwindow* window, double x_pos, double y_pos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

bool Application::Initialise() {
    // SETUP DEPENDENCIES
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        cout << "GLFW did not initialise properly" << "\n";
        return false;
    }
    // using GL 3.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    int window_width = 1920;
    int window_height = 1080;
    window = glfwCreateWindow(window_width, window_height, "Disbeleaf", nullptr, nullptr);
    if (window == nullptr)
    {
        cout << "Failed to create window" << "\n";
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1); // enable vsync
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << "\n";
        return false;
    }
    // Set options and callbacks for window input
    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetMouseButtonCallback(window, mouse_click_callback);
    glfwSetKeyCallback(window, key_callback);
    last_x = window_width / 2.0f; // cursor origin is centre of window
    last_y = window_height / 2.0f;

    // Configure OpenGL view
    glViewport(0, 0, window_width, window_height);
    glEnable(GL_DEPTH_TEST);

    // Setup ImGui context to link with GLFW OpenGL + load fonts to use
    const char* glsl_version = "#version 130"; 
    IMGUI_CHECKVERSION();
    ImGui::CreateContext(); 
    // ImGUI will give its own error message if failed to initialise so no need for manual checking
    ImGuiIO &io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version); // using GLSL 1.3
    menu_font = io.Fonts->AddFontFromFileTTF(font_path, 60);
    score_font = io.Fonts->AddFontFromFileTTF(font_path, 75);
    
    // SETUP GAME SCENE
    playerRenderer = new ModelRenderer();
    algaRenderer = new ModelRenderer();
    fishRenderer = new ModelRenderer();
    sunRenderer = new ModelRenderer();
    skyboxRenderer = new SkyboxRenderer();
    // common projection matrix used across all shaders
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window_width / (float)window_height, 0.1f, 10000.0f);
    // Renderers:
    // player
    playerRenderer->Initialise(
        model_vert_shader, model_frag_shader, "playerShader", 
        player_model_scene, projection
    );
    // algae
    algaRenderer->Initialise(
        model_vert_shader, model_frag_shader, "algaeShader", 
        alga_model_scene, projection
    );
    // fish
    fishRenderer->Initialise(
        model_vert_shader, model_frag_shader, "fishShader", 
        fish_model_scene, projection
    );
    // sun model
    sunRenderer->Initialise(
        model_vert_shader, model_frag_shader, "sunShader",
        sun_model_scene, projection
    );
    // skybox cubemap
    skyboxRenderer->Initialise(
        skybox_vert_shader, skybox_frag_shader, "skyboxShader", 
        "", projection
    );
    // window manager may not have granted the requested size, so match projection to the actual framebuffer
    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    UpdateProjection(fb_width, fb_height);

    // Lighting:
    global_lighting = new LightingEngine();
    global_lighting->Initialise();
    
    // player, camera, algae generation
    game_structure = new GameStructure();
    player = new Player();
    camera = new Camera(player->mPos);
    algae_manager = new AlgaeManager(NUM_ALGAE); // 500 algae
    algae_manager->InitialGeneration();
    fish_manager = new FishManager(NUM_FISH); // 100 fish
    fish_manager->InitialGeneration();
    // load game save file
    game_structure->Save(true);

    // Start time tracking
    cur_time = 0.0f;
    dt = 0.0f;
    last_time = glfwGetTime();

    cout << "App initialised successfully" << "\n";
    return true;
}

void Application::MainLoop() {
    // timing for consistent fps across machines + smooth rotation of player
    last_time = cur_time;
    cur_time = glfwGetTime();
    dt += (cur_time - last_time);
    // APP LOOP
    if (dt >= (1.0f / FPS_LIM)) {
        // GAME LOGIC
        fish_manager->UpdateAllFishPos(dt, player->mPos); // fish don't get paused
        // INPUT HANDLING (based on variables set by input callbacks just before this frame)
        // handle inputs and player logic if game is in-run and not paused
        if (game_structure->InRun() && !flag_pause_open) {
            if (flag_MouseDown) {
                player->AccelForward();
            }
            // (if not in dev mode) Update camera position since we are in-run
            if (!flag_cursor_normal) {
                camera->ProcessMouseLooking(x_offset, y_offset, player->mPos, player->mYaw);
                // reset the offsets so camera doesn't continue turning when mouse stops moving
                x_offset = 0.0f;
                y_offset = 0.0f;
            }
            // process algae collisions with player
            int algae_collected = algae_manager->DetectCollisions(player->mPos, player->GetPlayerRadius());
            player->Update(algae_collected);
            game_structure->UpdateScore(player->GetAlgaeCount());

            // Reduce timer by time passed and reset time counter dt
            // (potential flaw is if machine lags then timer will still tick by lagged time)?
            game_structure->TickTime(dt);
        }
        
        // Lighting update
        global_lighting->UpdateSun(dt);
        dt = 0.0f;

        ///////////////////////////////////////////////////
        // RENDER PIPELINE
        glEnable(GL_CULL_FACE);
        glClear(GL_DEPTH_BUFFER_BIT); 
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        view = camera->GetViewMatrix(player->mPos);
        glm::vec3 vert_offset = {0.0f, -0.2f, 0.0f}; // offset camera slightly above player, applied to every game object

        // render sun
        model = glm::translate(glm::mat4(1.0f), vert_offset);
        sunRenderer->SetNextProperties(global_lighting->Sun.position, 0.0f, 0.0f, global_lighting->Sun.scale, view, model, camera->mPos);
        sunRenderer->Render(*global_lighting);

        // render player
        glCullFace(GL_FRONT);
        // (yaw offset by 90 degrees due to imported model being default rotated)
        model = glm::translate(glm::mat4(1.0f), vert_offset);
        playerRenderer->SetNextProperties(player->mPos, player->mYaw+(+90.0f), -player->mPitch+(45.0f), player->mScale, view, model, camera->mPos);
        playerRenderer->Render(*global_lighting);
        glCullFace(GL_BACK);

        // render fish
        // (pitch offset by -90 degrees due to imported model being default pointing downwards)
        for (int i = 0; i < fish_manager->GetNumFish(); i++) {
            model = glm::translate(glm::mat4(1.0f), vert_offset);
            Fish &cur_fish = fish_manager->GetFish()[i];
            fishRenderer->SetNextProperties(cur_fish.mPos, cur_fish.mYaw, cur_fish.mPitch+(-90.0f), cur_fish.mScale, view, model, camera->mPos);
            fishRenderer->Render(*global_lighting);
        }
        
        // render alga
        for (int i = 0; i < algae_manager->mNumAlgae; i++) {
            model = glm::translate(glm::mat4(1.0f), vert_offset);
            Alga &cur_alga = algae_manager->GetAlgae()[i];
            algaRenderer->SetNextProperties(cur_alga.mPos, cur_alga.mYaw, cur_alga.mPitch, cur_alga.mScale, view, model, camera->mPos);
            algaRenderer->Render(*global_lighting);
        }

        // render skybox
        model = glm::translate(glm::mat4(1.0f), vert_offset);
        skyboxRenderer->SetNextProperties(view, model);
        skyboxRenderer->Render(*global_lighting); // global lighting not actually used for skybox


        ///////////////////////////////////////////////////
        // USER INTERFACE
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // Logic to display start menu
        if (!game_structure->InRun()) {
            ImGui::OpenPopup("DISBELEAF");
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            flag_pause_open = true;
        }
        // Logic to open pause menu with esc key (can only pause when game state is in-run)
        if (player->KeysDown["ESC"] == true && game_structure->InRun()) {
            if (flag_pause_open == false) {
                ImGui::OpenPopup("DISBELEAF");
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                flag_pause_open = true;
            }
            else if (flag_pause_open == true) {
                to_close_menu = true;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                flag_pause_open = false;
            }
            player->KeysDown["ESC"] = false;
        }
        DefineUI();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        // GLFW FRAME UPDATE
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::DefineUI() {
    ImVec2 display_size = ImGui::GetIO().DisplaySize; // not constant as window may resize
    ImVec2 window_centre = ImGui::GetMainViewport()->GetCenter();
    ImVec2 window_pos;
    ImGui::PushFont(score_font);
    // TIMER + SCORE text
    if (game_structure->InRun()) {
        // These coordinates result in text being in desired position
        window_pos = {window_centre.x-(display_size.x / 26.0f), window_centre.y-(display_size.y / 2.8f)};
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        // update score + time left text
        string s1 = "Score: " + to_string(player->GetAlgaeCount()) + "        ";
        const char* score = s1.c_str();
        string s2 = "Time left: " + to_string(game_structure->GetTime());
        const char* time = s2.c_str();
        // display text
        ImGui::Begin("Score", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
        ImGui::Text(score);
        ImGui::SameLine();
        ImGui::Text(time);
        ImGui::End();
    }
    // GAMEOVER menu
    // same format as timer + score text
    if (game_structure->IsGameOver()) {
        // position is offset right, vertically centred
        window_pos = {window_centre.x+(display_size.x / 4.5f), window_centre.y};
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        string s1 = "Highscore: " + to_string(game_structure->GetHighscore());
        const char* highscore = s1.c_str();
        string s2 = "Run Score: " + to_string(game_structure->GetScore());
        const char* score = s2.c_str();
        ImGui::Begin("Highscore", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
        ImGui::Text("GAME OVER!");
        ImGui::Text(score);
        ImGui::Text(highscore);
        ImGui::End();
    }
    ImGui::PopFont();
    // PAUSE menu
    // position is offset left, vertically centred
    window_pos = {window_centre.x-(display_size.x / 4.5f), window_centre.y};
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(display_size.x / 6.0f, display_size.y / 3.0f));
    float button_size = display_size.x / 6.0f - 10.0f;
    ImGui::PushFont(menu_font);
    // using imgui modal popup, which fades background when open
    if (ImGui::BeginPopupModal("DISBELEAF", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar)) {
        ImGui::Separator();
        if (game_structure->InRun()) {
            if (ImGui::Button("RESUME", ImVec2(button_size, 0))) { 
                ImGui::CloseCurrentPopup();
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                flag_pause_open = false;
                flag_initial_mouse = true;
            }
            if (ImGui::Button("RESTART", ImVec2(button_size, 0))) {
                ImGui::CloseCurrentPopup();
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                flag_pause_open = false;
                flag_initial_mouse = true;
                game_structure->StartNewRun(player, algae_manager, camera);
            }
        }
        else {
            if (ImGui::Button("NEW RUN", ImVec2(button_size, 0))) { 
                ImGui::CloseCurrentPopup();
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                flag_pause_open = false;
                flag_initial_mouse = true;
                game_structure->StartNewRun(player, algae_manager, camera);
            }
        }
        if (ImGui::Button("SAVE", ImVec2(button_size, 0))) {
            game_structure->Save(false);
        }

        if (ImGui::Button("EXIT", ImVec2(button_size, 0))) { 
            to_exit_app = true;
        }
        ImGui::Separator();
        // Dev mode disabled for Release version
        // ImGui::Checkbox("Dev mode", &flag_dev_mode);

        ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);

        if (to_close_menu) {
            ImGui::CloseCurrentPopup();
            to_close_menu = false;
        }
        ImGui::EndPopup();
    }
    ImGui::PopFont();
    // (Disabled for Release version)
    // SETTINGS MENU WINDOW (open when dev mode enabled)
    // if (flag_dev_mode) {
    //     ImGui::Begin("Settings");
    //     ImGui::SetWindowSize(ImVec2(display_size.x / 3.0f, display_size.y / 2.0f));
    //     // speed slider
    //     ImGui::Text("Speed");
    //     ImGui::SameLine();
    //     ImGui::SliderFloat("##", &fish_manager->SPEED_SCALAR, 0.5f, 30.0f);

    //     // rule scale sliders
    //     ImGui::Text("COHESION  ");
    //     ImGui::SameLine();
    //     ImGui::SliderFloat("#1", &fish_manager->SCALE_COHESION, 0.1f, 100.0f);
    //     ImGui::Text("SEPARATION");
    //     ImGui::SameLine();
    //     ImGui::SliderFloat("#2", &fish_manager->SCALE_SEPARATION, 0.1f, 100.0f);
    //     ImGui::Text("ALIGNMENT ");
    //     ImGui::SameLine();
    //     ImGui::SliderFloat("#3", &fish_manager->SCALE_ALIGNMENT, 0.1f, 30.0f);
    //     ImGui::Text("TURN FACTOR");
    //     ImGui::SameLine();
    //     ImGui::SliderFloat("#4", &fish_manager->TURN_FACTOR_FLAT, 1.0f, 12.0f);
    //     ImGui::Text("VIEW RANGE");
    //     ImGui::SameLine();
    //     ImGui::SliderFloat("#5", &fish_manager->VIEW_RANGE, 1.0f, 100.0f);
    //     ImGui::End();
    // }
}

void mouse_move_callback(GLFWwindow* window, double x_pos, double y_pos) {
    if (app.game_structure->InRun()) {
        if (app.flag_initial_mouse) {
            // to smooth mouse position on first mouse move when exiting any menu, as mouse is likely offset from screen centre
            app.last_x = x_pos;
            app.last_y = y_pos;
            app.flag_initial_mouse = false;
        }
        app.x_offset = x_pos-app.last_x;
        app.y_offset = app.last_y-y_pos;
        app.last_x = x_pos;
        app.last_y = y_pos;
    }
}
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods) {
    // update whether right mouse click is down or not
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        app.flag_MouseDown = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        app.flag_MouseDown = false;
    }
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // update current key(s) down
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        app.player->KeysDown["W"] = true;
    }
    else if (key == GLFW_KEY_W && action == GLFW_RELEASE && app.player->KeysDown["W"] == true) {
        app.player->KeysDown["W"] = false;
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        app.player->KeysDown["A"] = true;
    }
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE && app.player->KeysDown["A"] == true) {
        app.player->KeysDown["A"] = false;
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        app.player->KeysDown["S"] = true;
    }
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE && app.player->KeysDown["S"] == true) {
        app.player->KeysDown["S"] = false;
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        app.player->KeysDown["D"] = true;
    }
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE && app.player->KeysDown["D"] == true) {
        app.player->KeysDown["D"] = false;
    }
    else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS && app.player->KeysDown["ESC"] != true && app.game_structure->InRun()) {
        app.player->KeysDown["ESC"] = true;
    }
    // ESC key release is not handled, as pause menu wants to be --toggled-- open on Esc press
    
    // Dev mode disabled for Release version
    // Q toggles cursor mode, for debugging purposes when boids settings menu is enabled
    // else if (key == GLFW_KEY_Q && action == GLFW_RELEASE) { 
    //     
    //     if (app.flag_cursor_normal) {
    //         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //         app.flag_cursor_normal = false;
    //     }
    //     else {
    //         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //         app.flag_cursor_normal = true;
    //     }
    // }
}

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the GL viewport always matches the window dimensions
    glViewport(0, 0, width, height);
    // and keep the projection aspect ratio in sync so objects aren't stretched
    app.UpdateProjection(width, height);
}

void Application::UpdateProjection(int width, int height) {
    if (width <= 0 || height <= 0) return; // minimised window - nothing to do & avoids divide by zero
    window_width = width;
    window_height = height;
    glViewport(0, 0, width, height);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 10000.0f);
    if (playerRenderer) playerRenderer->SetProjection(projection);
    if (algaRenderer) algaRenderer->SetProjection(projection);
    if (fishRenderer) fishRenderer->SetProjection(projection);
    if (sunRenderer) sunRenderer->SetProjection(projection);
    if (skyboxRenderer) skyboxRenderer->SetProjection(projection);
}

bool Application::IsRunning() {
    return (!glfwWindowShouldClose(window)) && !to_exit_app;
}

void Application::Terminate() {
    // save game (load=false)
    game_structure->Save(false);
    // Clear textures, imgui, context & window
    playerRenderer->Cleanup();
    fishRenderer->Cleanup();
    algaRenderer->Cleanup();
    sunRenderer->Cleanup();
    skyboxRenderer->Cleanup();
    ResourceManager::Clear();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    cout << "App terminated" << "\n";
}
