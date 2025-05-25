# Branches
[`[master]`](https://github.com/11x1/half-life-2-trainer/tree/master) - contains a simple injector  
[`[internal-dll]`](https://github.com/11x1/half-life-2-trainer/tree/internal-dll) - internal dll code
  
# Media
[demo video](https://new.wxcoy.cc/fail/95341ba3-c4e7-455c-941f-b1460101662b)  
[getting a video inside the console](https://www.youtube.com/watch?v=jVhQD6DqThE)  
  
# How to run
1. Clone the repository
2. Build the loader (make sure `lexend-regular.ttf` is in the same directory as the loader executable)
3. Build the internal DLL
4. Run the loader executable and enter the DLL path in the input field
5. Run the target process (Half-Life 2), wait for the game to load into the main menu
6. Inject the DLL by clicking the "Inject" button in the loader window.
7. Once the DLL is injected, you can open the in-game console and start painting!   
note: caps-lock to paint, controls above the console to control the thickness and color (red/black)

# About the project
I chose the theme of the project to be about reading and writing in another process's memory space, because I am familiar with it in some places. I chose Valve's standalone game Half-Life 2 as the target process because I am already partially familiar with their game engine Source Engine.

## Explanation of terms in English to make the reading more understandable
`Handle` - Windows API term, used in the context of windows for access purposes. [More Info](https://learn.microsoft.com/en-us/windows/win32/sysinfo/handles-and-objects)  
`RTTI` - Run-time type information, important for vft hooking. [More Info](https://learn.microsoft.com/en-us/cpp/cpp/run-time-type-information?view=msvc-170)  
`Injecting`, a process (in terms of this project) of running a third-party module in a target process.  
`Pattern Scanning` - allows to find the desired variable, function or asm command/operation by searching for a "pattern" of bytes in a given memory area.   
`VFT Hooking` - Virtual Function Hooking, hooking of virtual methods of a class, allows external code to be executed before/after the class method is run. [More Info](https://www.codereversing.com/archives/596)

# Parts of the project
The project consists of two parts:

### Part 1, or *loader*
Sole purpose is the injection of the second part into the target process in order to ensure easy operation in the memory space of the target process.

### Part 2, the Dynamic-Link Library (DLL)
Manages everything that requires the target process to operate in memory.

## A little explanation
In simple terms, the loader looks for a window with the process name "hl2.exe" and opens a "handle" into that window. Next, the loader looks for the `kernel32.dll` module to use the LoadLibrary method from Windows' own API to create a thread in the target process' context.

Next, the injected module (now in the target process memory space) uses the target process's own modules and behaves as an "internal" within the project. I chose this approach over external because it gives the module almost full access to the game's memory space.

## Loader
- Contains ImGui sample code for creating a window.
- Uses the undocumented Windows API to create a blur effect behind the window (documented with sources in the source).
- Commented injection process target process

## Dynamic-Link Library (DLL)
- Creates wrapper classes for in-game modules
- Uses pattern matching to find the necessary places in the modules, e.g. the address of an in-game method to find the engine version.
- VFT Hooking to run your own code in various virtual methods.
- A finished implementation to find classes in game modules by class names (RTTI good/bad feature), also includes (hopefully) reasonably commented code. [More Info](https://blog.rop.la/en/reversing/2022/12/13/identifying-vftables-through-ms-cpp-rtti.html)