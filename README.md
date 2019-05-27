Ashes
=====

Ashes is a rendering library written in C++17, matching closely Vulkan interfaces.
The final goal of this library is to expose as much of Vulkan features as possible.
It comes with a set of test applications, to validate the API.


I've started this project because I need to upgrade my 3D engine (Castor3D), to use Vulkan, and I needed that kind of library, to replace my current renderer (OpenGL).
I needed a way to work with OpenGL like we need to work with Vulkan.
It is still a WIP, the library is far from complete!!

## Available renderers

- VkRenderer : A Vulkan renderer.
- Gl3Renderer : An OpenGL 3.X renderer, based upon OpenGL 3.2.
- Gl4Renderer : An OpenGL 4.X renderer, based upon OpenGL 4.2.
- D3D11Renderer : A Direct3D 11 renderer.

## Documentation

The API is documented using Doxygen, and a version can be found here :
- [English version](./doc/Ashes/English)
- [French version](./doc/Ashes/French)

## Build

To build it, you can use either CMake or premake.

## Test applications

These applications are used to validate the basic functionalities of the library.

### Device creation

Most basic example, initialises the logical device.

### Swapchain creation
<img src="./img/screenshots/02.png" height="72px" align="right">

Another simple test, checking that swapchain creation succeeds.

### Vertex layout
<img src="./img/screenshots/03.png" height="72px" align="right">

Tests vertex layouts, by displaying a coloured rectangle.

### Staging buffer
<img src="./img/screenshots/04.png" height="72px" align="right">

Tests transfers to VRAM using staging buffers.

### Texture 2D
<img src="./img/screenshots/05.png" height="72px" align="right">

Tests texture 2D load and display.

### Uniform Buffers
<img src="./img/screenshots/06.png" height="72px" align="right">

Tests the usage of uniform buffers.

### Texture 1D
<img src="./img/screenshots/07.png" height="72px" align="right">

Tests texture 1D load and display.

### Texture Buffer
<img src="./img/screenshots/08.png" height="72px" align="right">

Tests texture buffer load and display.

### Frame Buffer
<img src="./img/screenshots/09.png" height="72px" align="right">

Tests frame buffers, and index buffers too.

### Spinning cube
<img src="./img/screenshots/10.png" height="72px" align="right">

Tests depth buffer, depth test, and multiple uniform buffers.

### Texture 3D
<img src="./img/screenshots/11.png" height="72px" align="right">

Demonstrates load and display of a 3D texture.

### Texture Cube
<img src="./img/screenshots/12.png" height="72px" align="right">

Loads a cube texture and displays it as a skybox.

### Push Constants
<img src="./img/screenshots/13.png" height="72px" align="right">

Demonstrates the use of push constants, by displaying two cubes with different colour modifiers, stored in push constants.

### Render to Cube
<img src="./img/screenshots/14.png" height="72px" align="right">

Tests render to cube, by taking an equirectangular texture, and loading it into a skybox.

### Instantiation
<img src="./img/screenshots/15.png" height="72px" align="right">

Tests hardware instantiation, by rendering 1'000'000 cubes.

### Compute Pipeline
<img src="./img/screenshots/16.png" height="72px" align="right">

Tests Compute pipeline, by processing the output of the PushConstants sample to add a graphical effect.

### Compressed Texture 2D
<img src="./img/screenshots/17.png" height="72px" align="right">

Tests loading a texture 2D, compressed in either BC3, ASTC 8x8 or ETC2, depending on support by the rendering API.

### Texture 2D Mipmaps
<img src="./img/screenshots/18.png" height="72px" align="right">

Tests loading a texture 2D and its mipmap levels from a file.

### Compressed Texture 2D Array
<img src="./img/screenshots/19.png" height="72px" align="right">

Tests loading a texture 2D array, compressed in either BC3, ASTC 8x8 or ETC2, depending on support by the rendering API.

### Dynamic Uniform Buffers
<img src="./img/screenshots/20.png" height="72px" align="right">

Takes the push constant test case and uses dynamic buffer descriptor instead of multiple descriptors.

### Specialisation Constants
<img src="./img/screenshots/21.png" height="72px" align="right">

Tests the specialisation constants in Vulkan (matching them with uniforms in OpenGL renderers).

### SPIR-V Specialisation Constants
<img src="./img/screenshots/22.png" height="72px" align="right">

Tests the specialisation constants in Vulkan and OpenGL renderers (as long as they support SPIRV shaders).

### Bloom
<img src="./img/screenshots/23.png" height="72px" align="right">

Bloom implementation using downscale through mipmaps.


## Sample applications

#### Sample apps to implement
- Compute particle system.

These applications use the library in a more complex way, closer to what would be done within an engine.

### Object Loading
<img src="./img/screenshots/s01.png" height="72px" align="right">

Loads an object, with textures, and displays it.

### Lighting
<img src="./img/screenshots/s02.png" height="72px" align="right">

Applies lights to the previously loaded object.

### Normal Mapping
<img src="./img/screenshots/s03.png" height="72px" align="right">

Applies normal mapping.

### Deferred Rendering
<img src="./img/screenshots/s04.png" height="72px" align="right">

Deferred rendering implementation for opaque objects

### Billboards
<img src="./img/screenshots/s05.png" height="72px" align="right">

Billboards sample.
