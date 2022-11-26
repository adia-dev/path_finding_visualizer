// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "../Components/Grid.h"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

namespace se
{

    class Window
    {
    public:
        // Private overloads of the constructor and destructor to prevent multiple instances of the class
        Window();
        ~Window();

        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;
        Window(Window &&) = delete;
        Window &operator=(Window &&) = delete;

        void Play();
        void Playground();

        static Window &Get()
        {
            static Window instance;
            return instance;
        }

        static void HelpMarker(const char *desc)
        {
            ImGui::TextDisabled("(?)");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
            {
                ImGui::BeginTooltip();
                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                ImGui::TextUnformatted(desc);
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }
        }

    private:
        /* data */
        GLFWwindow *_window = nullptr;
        std::string _glsl_version = "#version 330";
        const u_int16_t _width = 1280;
        const u_int16_t _height = 720;

        bool _show_demo_window = true;
        const char *_window_title = "Hello, world!";
        ImVec4 _clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        /* Grid Data */
        std::unique_ptr<Grid> _grid;
        uint16_t _steps_per_frame = 100;
        uint16_t _cell_size = 20;
        bool _is_running = false;

        static inline void glfw_error_callback(int error, const char *description)
        {
            fprintf(stderr, "Glfw Error %d: %s\n", error, description);
        }

        void SetupImGui();
        void SetupOpenGL();
        void SetupGrid();

        // void Update();
        void Render();

        // Playground Methods
        void UpdatePlayground();
        void RenderPlayground();

        // Common Methods
        void MainMenuBar();
        // void HandleEvents();
        void Cleanup();
    };

}
