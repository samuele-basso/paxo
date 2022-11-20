#include "Loader.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include "event/Event.h"
#include "core/Log.h"
#include "core/Platform.h"

enum GlfwClientApi
{
    GlfwClientApi_Unknown,
    GlfwClientApi_OpenGL,
    GlfwClientApi_Vulkan
};

struct ImGui_ImplGlfw_Data
{
    GLFWwindow* Window;
    GlfwClientApi ClientApi;
    double Time;
    GLFWwindow* MouseWindow;
    bool MouseJustPressed[ImGuiMouseButton_COUNT];
    GLFWcursor* MouseCursors[ImGuiMouseCursor_COUNT];
    GLFWwindow* KeyOwnerWindows[512];
    bool InstalledCallbacks;
    bool WantUpdateMonitors;

    GLFWwindowfocusfun PrevUserCallbackWindowFocus;
    GLFWcursorenterfun PrevUserCallbackCursorEnter;
    GLFWmousebuttonfun PrevUserCallbackMousebutton;
    GLFWscrollfun PrevUserCallbackScroll;
    GLFWkeyfun PrevUserCallbackKey;
    GLFWcharfun PrevUserCallbackChar;
    GLFWmonitorfun PrevUserCallbackMonitor;

    ImGui_ImplGlfw_Data()
    {
        memset(this, 0, sizeof(*this));
    }
};


namespace editor
{
    namespace gui
    {

#ifdef RT_ACTIVE_API_DEFAULT_OPENGL

        static void init_api_internal(GLFWwindow* window)
        {
            const char* version = "#version 460";
            ImGui_ImplGlfw_InitForOpenGL(window, false);
            ImGui_ImplOpenGL3_Init(version);
        }

#elif RT_ACTIVE_API_DIRECTX

        static void init_api_internal(GLFWwindow* window)
        {
            RT_ASSERT(false);
        }

#elif RT_ACTIVE_API_VULKAN

        static void init_api_internal(GLFWwindow* window)
        {
            RT_ASSERT(false);
        }

#endif

        void install_callback()
        {
            rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::window_focus_event, rt::evnt::evnt_callback([](rt::evnt::rt_event* payload) {
                rt::evnt::window_focus_event* e = (rt::evnt::window_focus_event*)payload;

                ImGui_ImplGlfw_Data* bd =
                    ImGui::GetCurrentContext() ? (ImGui_ImplGlfw_Data*)ImGui::GetIO().BackendPlatformUserData : NULL;
                if (bd->PrevUserCallbackWindowFocus != NULL && e->window == bd->Window)
                {
                    bd->PrevUserCallbackWindowFocus(e->window, true);
                }

                ImGuiIO& io = ImGui::GetIO();
                io.AddFocusEvent(true);
                })
            );

            rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::window_lost_focus_event, rt::evnt::evnt_callback([](rt::evnt::rt_event* payload) {
                rt::evnt::window_lost_focus_event* e = (rt::evnt::window_lost_focus_event*)payload;

                ImGui_ImplGlfw_Data* bd =
                    ImGui::GetCurrentContext() ? (ImGui_ImplGlfw_Data*)ImGui::GetIO().BackendPlatformUserData : NULL;
                if (bd->PrevUserCallbackWindowFocus != NULL && e->window == bd->Window)
                {
                    bd->PrevUserCallbackWindowFocus(e->window, false);
                }

                ImGuiIO& io = ImGui::GetIO();
                io.AddFocusEvent(false);
                })
            );

            rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::mouse_button_press_event, rt::evnt::evnt_callback([](rt::evnt::rt_event* payload) {
                rt::evnt::mouse_button_press_event* e = (rt::evnt::mouse_button_press_event*)payload;

                ImGui_ImplGlfw_Data* bd =
                    ImGui::GetCurrentContext() ? (ImGui_ImplGlfw_Data*)ImGui::GetIO().BackendPlatformUserData : NULL;
                if (bd->PrevUserCallbackMousebutton != NULL && e->window == bd->Window)
                {
                    bd->PrevUserCallbackMousebutton(e->window, e->button, GLFW_PRESS, 0);
                }

                if (e->button >= 0 && e->button < IM_ARRAYSIZE(bd->MouseJustPressed))
                {
                    bd->MouseJustPressed[e->button] = true;
                }
                })
            );

            rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::mouse_button_release_event, rt::evnt::evnt_callback([](rt::evnt::rt_event* payload) {
                rt::evnt::mouse_button_release_event* e = (rt::evnt::mouse_button_release_event*)payload;

                ImGui_ImplGlfw_Data* bd =
                    ImGui::GetCurrentContext() ? (ImGui_ImplGlfw_Data*)ImGui::GetIO().BackendPlatformUserData : NULL;
                if (bd->PrevUserCallbackMousebutton != NULL && e->window == bd->Window)
                {
                    bd->PrevUserCallbackMousebutton(e->window, e->button, GLFW_RELEASE, 0);
                }
                })
            );

            rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::mouse_scroll_event, rt::evnt::evnt_callback([](rt::evnt::rt_event* payload) {
                rt::evnt::mouse_scroll_event* e = (rt::evnt::mouse_scroll_event*)payload;

                ImGui_ImplGlfw_Data* bd =
                    ImGui::GetCurrentContext() ? (ImGui_ImplGlfw_Data*)ImGui::GetIO().BackendPlatformUserData : NULL;
                if (bd->PrevUserCallbackScroll != NULL && e->window == bd->Window)
                {
                    bd->PrevUserCallbackScroll(e->window, e->x_offset, e->y_offset);
                }

                ImGuiIO& io = ImGui::GetIO();
                io.MouseWheelH += e->x_offset;
                io.MouseWheel += e->y_offset;
                })
            );

            rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::key_press_event, rt::evnt::evnt_callback([](rt::evnt::rt_event* payload) {
                rt::evnt::key_press_event* e = (rt::evnt::key_press_event*)payload;

                ImGui_ImplGlfw_Data* bd =
                    ImGui::GetCurrentContext() ? (ImGui_ImplGlfw_Data*)ImGui::GetIO().BackendPlatformUserData : NULL;
                auto action = GLFW_PRESS;
                if (e->repeat)
                {
                    action = GLFW_REPEAT;
                }

                if (bd->PrevUserCallbackKey != NULL && e->window == bd->Window)
                {
                    bd->PrevUserCallbackKey(e->window, e->key_code, 0, action, 0);
                }

                ImGuiIO& io = ImGui::GetIO();
                if (e->key_code >= 0 && e->key_code < IM_ARRAYSIZE(io.KeysDown))
                {
                    if (!e->repeat)
                    {
                        io.KeysDown[e->key_code] = true;
                    }
                }

                io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
                io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
                io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
#ifdef RT_WINDOWS_64
                io.KeySuper = false;
#else
                io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
#endif
                })
            );

            rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::key_release_event, rt::evnt::evnt_callback([](rt::evnt::rt_event* payload) {
                rt::evnt::key_release_event* e = (rt::evnt::key_release_event*)payload;

                ImGui_ImplGlfw_Data* bd =
                    ImGui::GetCurrentContext() ? (ImGui_ImplGlfw_Data*)ImGui::GetIO().BackendPlatformUserData : NULL;
                if (bd->PrevUserCallbackKey != NULL && e->window == bd->Window)
                {
                    bd->PrevUserCallbackKey(e->window, e->key_code, 0, GLFW_RELEASE, 0);
                }

                ImGuiIO& io = ImGui::GetIO();
                if (e->key_code >= 0 && e->key_code < IM_ARRAYSIZE(io.KeysDown))
                {
                    io.KeysDown[e->key_code] = false;
                }

                io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
                io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
                io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
#ifdef RT_WINDOWS_64
                io.KeySuper = false;
#else
                io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
#endif
                })
            );

            rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::cursor_enter_event, rt::evnt::evnt_callback([](rt::evnt::rt_event* payload) {
                rt::evnt::cursor_enter_event* e = (rt::evnt::cursor_enter_event*)payload;

                ImGui_ImplGlfw_Data* bd =
                    ImGui::GetCurrentContext() ? (ImGui_ImplGlfw_Data*)ImGui::GetIO().BackendPlatformUserData : NULL;
                if (bd->PrevUserCallbackCursorEnter != NULL && e->window == bd->Window)
                {
                    bd->PrevUserCallbackCursorEnter(e->window, true);
                }

                bd->MouseWindow = e->window;
                })
            );

            rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::cursor_exit_event, rt::evnt::evnt_callback([](rt::evnt::rt_event* payload) {
                rt::evnt::cursor_exit_event* e = (rt::evnt::cursor_exit_event*)payload;

                ImGui_ImplGlfw_Data* bd =
                    ImGui::GetCurrentContext() ? (ImGui_ImplGlfw_Data*)ImGui::GetIO().BackendPlatformUserData : NULL;
                if (bd->PrevUserCallbackCursorEnter != NULL && e->window == bd->Window)
                {
                    bd->PrevUserCallbackCursorEnter(e->window, false);
                }

                if (bd->MouseWindow == e->window)
                {
                    bd->MouseWindow = NULL;
                }
                })
            );

            rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::char_event, rt::evnt::evnt_callback([](rt::evnt::rt_event* payload) {
                rt::evnt::char_event* e = (rt::evnt::char_event*)payload;

                ImGui_ImplGlfw_Data* bd =
                    ImGui::GetCurrentContext() ? (ImGui_ImplGlfw_Data*)ImGui::GetIO().BackendPlatformUserData : NULL;
                if (bd->PrevUserCallbackChar != NULL && e->window == bd->Window)
                {
                    bd->PrevUserCallbackChar(e->window, e->code_point);
                }

                ImGuiIO& io = ImGui::GetIO();
                io.AddInputCharacter(e->code_point);
                })
            );

            glfwSetMonitorCallback([](GLFWmonitor* monitor, int evnt) {
                ImGui_ImplGlfw_Data* bd =
                    ImGui::GetCurrentContext() ? (ImGui_ImplGlfw_Data*)ImGui::GetIO().BackendPlatformUserData : NULL;
                    bd->WantUpdateMonitors = true;
                }
            );
        }

        void init_api(GLFWwindow* window)
        {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();

            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

            const float k_font_size = 17.0f;
            io.Fonts->AddFontFromFileTTF("resources/opensans/OpenSans-VariableFont_wdth,wght.ttf", k_font_size);

            init_api_internal(window);

            auto& style = ImGui::GetStyle();

            style.WindowMenuButtonPosition = ImGuiDir_None;

            style.WindowBorderSize = 0.5f;
            style.PopupBorderSize = 0.0f;
            style.TabBorderSize = 0.0f;

            style.WindowRounding = 1.5f;
            style.FrameRounding = 1.5f;
            style.TabRounding = 1.5f;

            style.WindowPadding = { 7.5f, 7.5f };

            auto& colors = style.Colors;

            colors[ImGuiCol_CheckMark] = ImVec4{ 0.898f, 0.898f, 0.898f, 1.0f };

            colors[ImGuiCol_Header] = ImVec4{ 0.25f, 0.255f, 0.26f, 1.0f };
            colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
            colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

            colors[ImGuiCol_Button] = ImVec4{ 0.25f, 0.255f, 0.26f, 1.0f };
            colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
            colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

            colors[ImGuiCol_FrameBg] = ImVec4{ 0.1412f, 0.1412f, 0.1412f, 1.0f };
            colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.1412f, 0.1412f, 0.1412f, 1.0f };
            colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.1412f, 0.1412f, 0.1412f, 1.0f };

            colors[ImGuiCol_Tab] = ImVec4{ 0.17f, 0.175f, 0.17f, 1.0f };
            colors[ImGuiCol_TabHovered] = ImVec4{ 0.225f, 0.230f, 0.235f, 1.0f };
            colors[ImGuiCol_TabActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
            colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.17f, 0.175f, 0.17f, 1.0f };
            colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

            colors[ImGuiCol_WindowBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

            colors[ImGuiCol_TitleBg] = ImVec4{ 0.1412f, 0.1412f, 0.1412f, 1.0f };
            colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.1412f, 0.1412f, 0.1412f, 1.0f };
            colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.1412f, 0.1412f, 0.1412f, 1.0f };
        }

        void shutdown_api()
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }


#ifdef RT_ACTIVE_API_DEFAULT_OPENGL

        static void new_frame_internal()
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
        }

#elif RT_ACTIVE_API_DIRECTX

        static void new_frame_internal()
        {
        }

#elif RT_ACTIVE_API_VULKAN

        static void new_frame_internal()
        {
        }

#endif

        void new_frame()
        {
            new_frame_internal();
            ImGui::NewFrame();
        }

#ifdef RT_ACTIVE_API_DEFAULT_OPENGL

        static void end_frame_internal()
        {
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

#elif RT_ACTIVE_API_DIRECTX

        static void end_frame_internal()
        {
        }

#elif RT_ACTIVE_API_VULKAN

        static void end_frame_internal()
        {
        }

#endif

        void end_frame()
        {
            ImGui::Render();

            end_frame_internal();

            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup);
            }
        }

        void dock()
        {
            ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4{ 0.1412f, 0.1412f, 0.1412f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_SeparatorHovered, ImVec4{ 0.1412f, 0.1412f, 0.1412f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_SeparatorActive, ImVec4{ 0.1412f, 0.1412f, 0.1412f, 1.0f });
            ImGui::DockSpaceOverViewport();
            ImGui::PopStyleColor(3);
        }

    }
}