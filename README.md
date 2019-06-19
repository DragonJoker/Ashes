|         Server     | Result |
|:------------------:|--------|
|  Travis CI (Linux) | [![Build Status](https://travis-ci.org/DragonJoker/Ashes.svg?branch=master)](https://travis-ci.org/DragonJoker/Ashes) |
| AppVeyor (Windows) | [![Build status](https://ci.appveyor.com/api/projects/status/418lak7ca1u0d97c?svg=true)](https://ci.appveyor.com/project/DragonJoker/rendererlib) |


Ashes
=====

Ashes is a rendering library written in C++17, matching closely Vulkan interfaces.
The final goal of this library is to expose as much of Vulkan features as possible.
It comes with a set of test applications, to validate the API.

I've started this project because I need to upgrade my 3D engine (Castor3D), to use Vulkan, and I needed that kind of library, to replace my current renderer (OpenGL).
I needed a way to work with OpenGL like we need to work with Vulkan.
It is still a WIP, the library is far from complete!!

To build it, you can use either CMake or premake.

## Renderers available

- VkRenderer : A Vulkan renderer.
- Gl3Renderer : An OpenGL 3.X renderer, based upon OpenGL 3.2.
- Gl4Renderer : An OpenGL 4.X renderer, based upon OpenGL 4.2.
- D3D11Renderer : A Direct3D 11 renderer.

## Test applications

These applications are used to validate the basic functionalities of the library.

### [Device creation](test/01-DeviceCreation/)

Most basic example, initialises the logical device.

### [Swapchain creation](test/02-SwapChainCreation/)
<img src="./screenshots/02.png" height="72px" align="right">

Another simple test, checking that swapchain creation succeeds.

### [Vertex layout](test/03-VertexLayout/)
<img src="./screenshots/03.png" height="72px" align="right">

Tests vertex layouts, by displaying a coloured rectangle.

### [Staging buffer](test/04-StagingBuffer/)
<img src="./screenshots/04.png" height="72px" align="right">

Tests transfers to VRAM using staging buffers.

### [Texture 1D](test/05-Texture1D/)
<img src="./screenshots/05.png" height="72px" align="right">

Tests texture 1D load and display.

### [Texture 2D](test/06-Texture2D/)
<img src="./screenshots/06.png" height="72px" align="right">

Tests texture 2D load and display.

### [Texture 3D](test/07-Texture3D/)
<img src="./screenshots/07.png" height="72px" align="right">

Demonstrates load and display of a 3D texture.

### [Texture Cube](test/08-TextureCube/)
<img src="./screenshots/08.png" height="72px" align="right">

Loads a cube texture and displays it as a skybox.

### [Texture Buffer](test/09-TextureBuffer/)
<img src="./screenshots/09.png" height="72px" align="right">

Tests texture buffer load and display.

### [Compressed Texture 2D](test/10-CompressedTexture2D/)
<img src="./screenshots/17.png" height="72px" align="right">

Tests loading a texture 2D, compressed in either BC3, ASTC 8x8 or ETC2, depending on support by the rendering API.

### [Compressed Texture 2D Array](test/11-CompressedTexture2DArray/)
<img src="./screenshots/11.png" height="72px" align="right">

Tests loading a texture 2D array, compressed in either BC3, ASTC 8x8 or ETC2, depending on support by the rendering API.

### [Texture 2D Mipmaps](test/12-Texture2DMipmaps/)
<img src="./screenshots/12.png" height="72px" align="right">

Tests loading a texture 2D and its mipmap levels from a file.

### [Texture 2D Mipmaps Generation](test/13-TextureMipmapGeneration/)
<img src="./screenshots/13.png" height="72px" align="right">

Tests loading a texture 2D from a file, and generating its mipmaps from level 0.

### [Uniform Buffers](test/14-UniformBuffers/)
<img src="./screenshots/14.png" height="72px" align="right">

Tests the usage of uniform buffers.

### [Push Constants](test/15-PushConstants/)
<img src="./screenshots/15.png" height="72px" align="right">

Demonstrates the use of push constants, by displaying two cubes with different colour modifiers, stored in push constants.

### [Render to Cube](test/16-RenderToCube/)
<img src="./screenshots/16.png" height="72px" align="right">

Tests render to cube, by taking an equirectangular texture, and loading it into a skybox.

### [Frame Buffer](test/17-FrameBuffer/)
<img src="./screenshots/17.png" height="72px" align="right">

Tests frame buffers, and index buffers too.

### [Spinning cube](test/18-SpinningCube/)
<img src="./screenshots/18.png" height="72px" align="right">

Tests depth buffer, depth test, and multiple uniform buffers.

### [Instantiation](test/19-Instantiation/)
<img src="./screenshots/19.png" height="72px" align="right">

Tests hardware instantiation, by rendering 1'000'000 cubes.

### [Compute Pipeline](test/20-ComputePipeline/)
<img src="./screenshots/20.png" height="72px" align="right">

Tests Compute pipeline, by processing the output of the PushConstants sample to add a graphical effect.

### [Dynamic Uniform Buffers](test/21-DynamicUniformBuffer/)
<img src="./screenshots/21.png" height="72px" align="right">

Takes the push constant test case and uses dynamic buffer descriptor instead of multiple descriptors.

### [Specialisation Constants](test/22-SpecialisationConstants/)
<img src="./screenshots/22.png" height="72px" align="right">

Tests the specialisation constants in Vulkan (matching them with uniforms in OpenGL renderers).

### [SPIR-V Specialisation Constants](test/23-SPIRVSpecialisationConstants/)
<img src="./screenshots/23.png" height="72px" align="right">

Tests the specialisation constants in Vulkan and OpenGL renderers (as long as they support SPIRV shaders).

### [Bloom](test/24-Bloom/)
<img src="./screenshots/24.png" height="72px" align="right">

Bloom implementation using downscale through mipmaps.


## Sample applications

#### Sample apps to implement
- Compute particle system.

These applications use the library in a more complex way, closer to what would be done within an engine.

### [Object Loading](samples/01-ObjectLoading/)
<img src="./screenshots/s01.png" height="72px" align="right">

Loads an object, with textures, and displays it.

### [Lighting](samples/02-Lighting/)
<img src="./screenshots/s02.png" height="72px" align="right">

Applies lights to the previously loaded object.

### [Normal Mapping](samples/03-NormalMapping/)
<img src="./screenshots/s03.png" height="72px" align="right">

Applies normal mapping.

### [Deferred Rendering](samples/04-DeferredRendering/)
<img src="./screenshots/s04.png" height="72px" align="right">

Deferred rendering implementation for opaque objects

### [Billboards](samples/05-Billboards/)
<img src="./screenshots/s05.png" height="72px" align="right">

Billboards sample.
