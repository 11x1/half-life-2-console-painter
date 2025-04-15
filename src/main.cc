#include "function_thread/main.hh"

#include "Windows.h"
#include <d3d9.h>
#include <dwmapi.h>
#include <tchar.h>

#include <filesystem>
#include <thread>
#include <format>

#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_impl_dx9.h"
#include "../libs/imgui/imgui_impl_win32.h"
#include "../libs/imgui/imgui_internal.h"

#include "globals.hh"
#include "state_manager/state_manager.hh"

// https://github.com/ocornut/imgui/tree/master/examples/example_win32_directx9
// Data
static LPDIRECT3D9 g_pD3D = nullptr;
static LPDIRECT3DDEVICE9 g_pd3dDevice = nullptr;
static bool g_DeviceLost = false;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS g_d3dpp = { };

// Forward declarations of helper functions
bool CreateDeviceD3D( HWND hWnd );

void CleanupDeviceD3D( );

void ResetDevice( );

LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

bool done = false;

namespace application {
    void render( ) {
        const auto& io = ImGui::GetIO( );

        ImGuiViewport* viewport = ImGui::GetMainViewport( );
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList( );
        constexpr ImU32 bg_color = IM_COL32( 30, 30, 30, 200 );
        draw_list->AddRectFilled( { 0, 0 }, { viewport->Size.x, viewport->Size.y }, bg_color, 5,
                                  ImDrawFlags_RoundCornersAll );

        constexpr ImU32 text_color = IM_COL32( 255, 255, 0, 255 );

        const auto fps_fmt{
            std::vformat( "fps: {}", std::make_format_args( std::clamp< int >( io.Framerate, 0.f, 1000.f ) ) )
        };
        draw_list->AddText( { 10, 10 }, text_color, fps_fmt.c_str( ) );

        POINT mp{ };
        GetCursorPos( &mp );

        const auto mouse_fmt{ std::vformat( "m={},{}", std::make_format_args( mp.x, mp.y ) ) };
        draw_list->AddText( { 10, 25 }, text_color, mouse_fmt.c_str( ) );

        RECT rect{ };
        GetWindowRect( GetActiveWindow( ), &rect );
        const auto pos_fmt{ std::vformat( "scr={},{}", std::make_format_args( rect.left, rect.top ) ) };
        draw_list->AddText( { 10, 40 }, text_color, pos_fmt.c_str( ) );

        const auto state_fmt = std::format( "state={}", g_state_manager.get_state_name( ) );
        draw_list->AddText( { 100, 50 }, text_color, state_fmt.c_str( ) );

        g_function_thread.render( *draw_list );

        static bool dragging{ false };
        static bool oob{ false };
        static int diff[ 2 ]{ 0, 0 };

        constexpr ImU32 drag_area_color = IM_COL32( 80, 80, 80, 180 );
        ImVec2 drag_start{ viewport->Size.x - 100, 0 };
        ImVec2 drag_end = { viewport->Size.x, 20 };
        draw_list->AddRectFilled( drag_start, drag_end, drag_area_color, 5,
                                  ImDrawFlags_RoundCornersTopRight | ImDrawFlags_RoundCornersBottomLeft );

        bool m1 = GetAsyncKeyState( VK_LBUTTON ) != 0;
        if ( rect.left + drag_start.x <= mp.x && mp.x <= drag_end.x + rect.left && rect.top + drag_start.y <= mp.y && mp
             .y <= rect.top + +drag_end.y ) {
            if ( !dragging && m1 && !oob ) {
                dragging = true;
                diff[ 0 ] = mp.x - rect.left;
                diff[ 1 ] = mp.y - rect.top;
            }
        } else if ( m1 && !oob && !dragging ) {
            oob = true;
        }

        if ( dragging ) {
            SetWindowPos( GetActiveWindow( ), nullptr, mp.x - diff[ 0 ], mp.y - diff[ 1 ], 600, 400, 0 );
        }

        if ( !m1 && dragging )
            dragging = false;

        if ( !m1 && oob )
            oob = false;

        if ( GetAsyncKeyState( VK_END ) )
            done = true;
    }
}

enum ACCENT_STATE {
    ACCENT_DISABLED = 0,
    ACCENT_ENABLE_GRADIENT = 1,
    ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
    ACCENT_ENABLE_BLURBEHIND = 3,
    ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,
    ACCENT_INVALID_STATE = 5
};

struct ACCENT_POLICY {
    ACCENT_STATE AccentState;
    DWORD AccentFlags;
    DWORD GradientColor;
    DWORD AnimationId;
};

struct WINDOWCOMPOSITIONATTRIBDATA {
    DWORD Attrib;
    PVOID pvData;
    SIZE_T cbData;
};

typedef BOOL ( WINAPI*pfnSetWindowCompositionAttribute )( HWND, WINDOWCOMPOSITIONATTRIBDATA* );

pfnSetWindowCompositionAttribute SetWindowCompositionAttribute = nullptr;

bool EnableBlurBehind( HWND hwnd ) {
    HMODULE hUser32 = LoadLibrary( "user32.dll" );
    if ( !hUser32 )
        return false;

    SetWindowCompositionAttribute = reinterpret_cast< pfnSetWindowCompositionAttribute >( GetProcAddress(
        hUser32, "SetWindowCompositionAttribute" ) );
    if ( !SetWindowCompositionAttribute ) {
        FreeLibrary( hUser32 );
        return false;
    }

    ACCENT_POLICY accentPolicy = { ACCENT_ENABLE_BLURBEHIND, 2, 0, 0 };
    WINDOWCOMPOSITIONATTRIBDATA data = { 19, &accentPolicy, sizeof( accentPolicy ) }; // 19 is WCA_ACCENT_POLICY

    const BOOL result = SetWindowCompositionAttribute( hwnd, &data );

    FreeLibrary( hUser32 );

    return result == true;
}


// Main code
int main( int, char** ) {
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = {
        sizeof( wc ), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle( nullptr ), nullptr, nullptr, nullptr, nullptr,
        L"ImGui Example", nullptr
    };
    ::RegisterClassExW( &wc );
    HWND hwnd = CreateWindowExW( WS_EX_LAYERED, wc.lpszClassName, L"grr window (angyr)", WS_POPUP, 100, 100, 600, 400,
                                 nullptr, nullptr, wc.hInstance, nullptr );

    SetLayeredWindowAttributes( hwnd, 0, 255, LWA_ALPHA );

    // Initialize Direct3D
    if ( !CreateDeviceD3D( hwnd ) ) {
        CleanupDeviceD3D( );
        ::UnregisterClassW( wc.lpszClassName, wc.hInstance );
        return 1;
    }

    if ( !EnableBlurBehind( hwnd ) ) {
        MessageBox( nullptr, "Failed to enable blur effect!", "Error", MB_ICONERROR );
    }

    // Show the window
    ::ShowWindow( hwnd, SW_SHOW );
    ::UpdateWindow( hwnd );

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );
    auto& io = ImGui::GetIO( );
    ( void ) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark( );

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init( hwnd );
    ImGui_ImplDX9_Init( g_pd3dDevice );

    g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    ImVec4 clear_color = ImVec4( 0.1f, 0.1f, 0.1f, 0.f );

    g_state_manager.set_state( STARTING );

    { // wrap in, we want to kill the app before
        // we kill the window
        // thread goes out of scope -> death

        auto function_thread_thread = std::thread( []( ) {
            g_function_thread.main( );
        } );

        function_thread_thread.detach( );

        // Main loop
        while ( g_should_run.load( ) ) {
            // Poll and handle messages (inputs, window resize, etc.)
            // See the WndProc() function below for our to dispatch events to the Win32 backend.
            MSG msg;
            while ( ::PeekMessage( &msg, nullptr, 0U, 0U, PM_REMOVE ) ) {
                ::TranslateMessage( &msg );
                ::DispatchMessage( &msg );
                if ( msg.message == WM_QUIT )
                    done = true;
            }
            if ( done )
                break;

            // Handle lost D3D9 device
            if ( g_DeviceLost ) {
                HRESULT hr = g_pd3dDevice->TestCooperativeLevel( );
                if ( hr == D3DERR_DEVICELOST ) {
                    ::Sleep( 10 );
                    continue;
                }
                if ( hr == D3DERR_DEVICENOTRESET )
                    ResetDevice( );
                g_DeviceLost = false;
            }

            // Handle window resize (we don't resize directly in the WM_SIZE handler)
            if ( g_ResizeWidth != 0 && g_ResizeHeight != 0 ) {
                g_d3dpp.BackBufferWidth = g_ResizeWidth;
                g_d3dpp.BackBufferHeight = g_ResizeHeight;
                g_ResizeWidth = g_ResizeHeight = 0;
                ResetDevice( );
            }

            // Start the Dear ImGui frame
            ImGui_ImplDX9_NewFrame( );
            ImGui_ImplWin32_NewFrame( );
            ImGui::NewFrame( );

            ImGui::SetNextWindowBgAlpha( 1.0f );

            application::render( );

            // Rendering
            ImGui::EndFrame( );
            g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
            // g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
            g_pd3dDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
            D3DCOLOR clear_col_dx = D3DCOLOR_RGBA( (int)(clear_color.x*clear_color.w*255.0f),
                                                   (int)(clear_color.y*clear_color.w*255.0f),
                                                   (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f) );
            g_pd3dDevice->Clear( 0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0 );
            if ( g_pd3dDevice->BeginScene( ) >= 0 ) {
                ImGui::Render( );
                ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
                g_pd3dDevice->EndScene( );
            }
            HRESULT result = g_pd3dDevice->Present( nullptr, nullptr, nullptr, nullptr );
            if ( result == D3DERR_DEVICELOST )
                g_DeviceLost = true;

            if ( GetAsyncKeyState( VK_END ) )
                g_should_run = false;
        }
    }

    printf( "cleaning up imgui & dx...\n" );

    // Cleanup
    ImGui_ImplDX9_Shutdown( );
    ImGui_ImplWin32_Shutdown( );
    ImGui::DestroyContext( );

    CleanupDeviceD3D( );
    ::DestroyWindow( hwnd );
    ::UnregisterClassW( wc.lpszClassName, wc.hInstance );

    printf( "exiting\n" );
    return 0;
}

// Helper functions

bool CreateDeviceD3D( HWND hWnd ) {
    if ( ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) == nullptr )
        return false;

    // Create the D3DDevice
    ZeroMemory( &g_d3dpp, sizeof(g_d3dpp) );
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if ( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp,
                               &g_pd3dDevice ) < 0 )
        return false;

    return true;
}

void CleanupDeviceD3D( ) {
    if ( g_pd3dDevice ) {
        g_pd3dDevice->Release( );
        g_pd3dDevice = nullptr;
    }
    if ( g_pD3D ) {
        g_pD3D->Release( );
        g_pD3D = nullptr;
    }
}

void ResetDevice( ) {
    ImGui_ImplDX9_InvalidateDeviceObjects( );
    HRESULT hr = g_pd3dDevice->Reset( &g_d3dpp );
    if ( hr == D3DERR_INVALIDCALL )
        IM_ASSERT( 0 );
    ImGui_ImplDX9_CreateDeviceObjects( );

    g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
    if ( ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ) )
        return true;

    switch ( msg ) {
        case WM_SIZE:
            if ( wParam == SIZE_MINIMIZED )
                return 0;
            g_ResizeWidth = ( UINT ) LOWORD( lParam ); // Queue resize
            g_ResizeHeight = ( UINT ) HIWORD( lParam );
            return 0;
        case WM_SYSCOMMAND:
            if ( ( wParam & 0xfff0 ) == SC_KEYMENU ) // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage( 0 );
            return 0;
    }
    return ::DefWindowProcW( hWnd, msg, wParam, lParam );
}
