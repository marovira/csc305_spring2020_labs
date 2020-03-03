# CSc 305 Lab 5: Introduction to OpenGL

# Introduction

The purpose of this lab is to get you acquainted with the process of creating an
OpenGL context and setting up a window where you will later on be rendering to.
Atlas provides facilities to simplify this process significantly, so we will
first walk through the setup code and the structures that Atlas uses.

## Code Structure

This lab is composed of two classes `Program` and `Triangle`. `Program` bundles the
core OpenGL/GLFW data needed to launch and maintain a window to render onto. `Triangle`
is a simple primitive to demonstrate the minimal amount of OpenGL data required to
render a triangle using the GPU. The following is a high-level description of each
member function and its intended purpose. Following, the `Program` functions will be
explained in depth.

`Program` functions:
-	`Program`, constructor creates a window and OpenGL context
-	`run`, takes a `Triangle` and loops while rendering the `Triangle`
-	`freeGPUData`, routine to de-allocate all OpenGL and GLFW data: must be called
before destruction of the object
-	`createGLContext`, private helper function to allocate an OpenGL context

`Triangle` functions:
-	`Triangle`, constructor which sets up shader handles
-	`loadShaders`, performs the initial load and attachment of `Triangle`'s
shaders
-	`loadDataToGPU`, takes a set of vertices and colors and allocates OpenGL
buffers to hold the triangle in GPU memory
-	`reloadShaders`, in case the shaders were modified, recompile and link the shaders
-	`render`, hook to render the `Triangle` onto the program's window
-	`freeGPUData`, routine to de-allocate all OpenGL data: must be called before
destruction of the object

## `Program::Program`

The first step in the constructor is to setup the settings data member with
information about the window size and title.

```c++
settings.size.width  = width;
settings.size.height = height;
settings.title       = title;
```

Then we give GLFW our static function member `errorCallback` to call in case
of an error. Afterwards, create a GLFW window. If we fail at any point binding
the `errorCallback` or creating the window throw an exception to terminate the
program.

```c++
if (!glx::initializeGLFW(errorCallback))
{
	throw OpenGLError("Failed to initialize GLFW with error callback");
}

mWindow = glx::createGLFWWindow(settings);
if (mWindow == nullptr)
{
	throw OpenGLError("Failed to create GLFW Window");
}
```

Call `Program::createGLContext` to create the context object.

## `Program::createGLContext`

Bind the data member `callbacks` to the window such that the window can
communicate events back to the program. Afterwards, make the context of
the window current on the core program thread. Finally, set the number of
screen updates to wait after `glfwSwapBuffers` is called (this is Vsync:
the game setting you always turn off).

```c++
glx::bindWindowCallbacks(mWindow, callbacks);
glfwMakeContextCurrent(mWindow);
glfwSwapInterval(1);
```

Create the OpenGL context object belonging to the window, then initialize
the callback object.

```c++
if (!glx::createGLContext(mWindow, settings.version))
{
	throw OpenGLError("Failed to create OpenGL context");
}

glx::initializeGLCallback(
	glx::ErrorSource::All, glx::ErrorType::All, glx::ErrorSeverity::All);
```

## `Program::run`

Within the main loop, which loops while `!glfwWindowShouldClose(mWindow)`,
the following code block is executed before rendering the triangle.
The block retrieves the present dimensions of the window, then sets
the OpenGL viewport to match. OpenGL is told the color to clear
the screen to, then the colour and depth buffers are cleared.

```c++
glfwGetFramebufferSize(mWindow, &width, &height);
glViewport(0, 0, width, height);
glClearColor(0, 0, 0, 1);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```

After calling the render hook of `Triangle`, we swap the frame buffers
then tell GLFW to invoke callbacks to any events which occurred during
the rendering.

```c++
glfwSwapBuffers(mWindow);
glfwPollEvents();
```

## `Program::freeGPUData`

De-allocate resources allocated earlier. In general, for `Triangle`
any `glCreateX` call must have an associated `glDeleteX` in its
`freeGPUData` function.

```c++
glx::destroyGLFWWindow(mWindow);
glx::terminateGLFW();
```
