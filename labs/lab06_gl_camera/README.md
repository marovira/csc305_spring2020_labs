# CSc 305 Lab 6: Uniform Matrices

# Introduction

The purpose of this lab is to make you familiar with the process of creating,
setting, and using inform variables along with the MVP matrix. The end goal of
this lab is to produce a spinning triangle whose animation can be switched
on/off with the spacebar. The triangle will be placed at the origin, with the
camera looking down from (1, 1, 1).

* Getting uniform variables: needs to be done *after* the shaders have been
  loaded but *before* the main loop. The main reason behind this is that the
  locations are constant and therefore there is no need to keep querying them.
  The way to obtain the uniform location is with
  `glGetUniformLocation`.
* Setting the uniform variables should be done in the main loop after the
  clearing of the buffers and binding of the shader program but *before* any
  render calls. The invocation for setting them will depend on the type of
  variable we're setting. Since we are using matrices, we will use
  `glUniformMatrix4fv`.
* We will require lambdas in order to bind the callbacks. Specifically, we will
  require `keyPressCallback`. The lambdas can contain the entire implementation
  or they can invoke a member function.
* The view matrix will be set with `glm::lookat` with the corresponding
  parameters.
* The projection matrix will be set with `glm::perspective`. The field of view
  will be 45 (specified in radians with `glm::radians`) and the aspect ratio is
  computed as `width / height` where these two are the dimensions of the
  *screen*. The near and far plane can be set to 1 and 1000000 respectively.
* The model matrix can be computed using `glm::rotate`. Keep in mind that the
  angle needs to be specified in radians. The axis of rotation will be (0, 1,
  0).

## C++ Lambdas

Lambdas are the method for creating `anonymous functions`: functions with no
identity or name. These types of functions are handy when you need to provide
a callback routine to another component (like where we used [`errorCallback`](https://github.com/marovira/csc305_spring2020_labs/blob/master/labs/lab05_gl_intro/lab.hpp#L65) in the previous lab).

You can give a lambda context/variables from the enclosing scope by `capturing` the
relevant information. Of course like any other C++ function, lambdas support function
parameters and returns. There are three formats for lambdas in C++:

* `[ captures ] ( params ) -> ret { body }`
* `[ captures ] ( params ) { body }`
* `[ captures ] { body }`

Params and body are what you expects. However, captures have some extra syntax to
support capturing by reference or value. You can capture everything by reference
using: `[&] { body }`. You can capture everything by value using: `[=] { body }`.
You can also mix using `[&, var0, ..., varN] { body }` or `[=, &var0, ..., &varN] { body }`. Where `&` is the leading capture, every other capture must be by value.
Conversely, where `=` is the leading capture, every other capture must be by reference. More lambda information can be found on [C++ Reference: Lambda expressions](https://en.cppreference.com/w/cpp/language/lambda).

Atlas wants to call [`keyPressCallback`](https://github.com/marovira/atlas/blob/master/include/atlas/glx/Context.hpp#L18) as a function which takes [four integer
parameters](https://www.glfw.org/docs/latest/input_guide.html#input_keyboard): `key`,
`scancode`, `action`, and `mods`. The parameters you'll likely care about are: `key`
and `action`.

## Uniform Variables

A uniform variable is a global variable (available to all shaders within the same program)
which is constant per rendering cycle. In this lab, the MVP matrices will be uniform variables.

You can get a handle for a uniform variable through a call to `glGetUniformLocation`.
`glGetUniformLocation` has two parameters, the first is the program handle and the
second is a `const char *` (string literal) naming the uniform variable. As explained
earlier, this call should only be made once per uniform variable, so not inside the
main render loop.

Inside the main render loop, you must provide the data to the uniform variable. This
is done through a function in the `glUniform` family. In the case of the lab
`glUniformMatrix4fv` will be used to pass the MVP matrices to the shader. Inside the
shaders, you can use the uniform variable through: `uniform mat4 matrix;` (where
`matrix` is the name of the uniform variable).

To pass a `glm::mat4` or `math::Matrix4` to `glUniformMatrix4fv`, you will want to use
the `glm::value_ptr` function.

## Spinning the Triangle

There's a number of ways to spin the triangle. The main idea is to have a variable
whose value incrementally changes, and apply that value to transform the triangle.
Check into [`glfwGetTime`](https://www.glfw.org/docs/3.0/group__time.html) and
[`glm::radians`](https://glm.g-truc.net/0.9.4/api/a00136.html). Putting the pieces
together is up to you.
