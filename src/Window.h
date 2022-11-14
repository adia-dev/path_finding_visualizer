// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <string>
#include <iostream>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

/* PROJECT INCLUDES */
#include "Utilities/Singleton.h"

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
        void Render();

        static Window &Get()
        {
            static Window instance;
            return instance;
        }

    private:
        /* data */
        GLFWwindow *_window = nullptr;

        bool _show_demo_window = true;
        bool _show_another_window = false;
        const char *_window_title = "Hello, world!";
        ImVec4 _clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        static inline void glfw_error_callback(int error, const char *description)
        {
            fprintf(stderr, "Glfw Error %d: %s\n", error, description);
        }
    };

}
