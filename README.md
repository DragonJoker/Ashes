RendererLib
===========

RendererLib is a rendering library written in C++14, matching closely Vulkan interfaces.
Vulkan and OpenGL renderers are actually written.
It comes with a set of test applications, to validate the API.

It is a WIP, the library is really far from complete!!


## Test applications

These applications are used to validate the basic functionalities of the library.

### [Device creation](source/Test/01-DeviceCreation/)

Most basic example, initialises the logical device.

### [Swapchain creation](source/Test/02-SwapChainCreation/)
<img src="./screenshots/02.png" height="72px" align="right">

Another simple test, checking that swapchain creation succeeds.

### [Vertex layout](source/Test/03-VertexLayout/)
<img src="./screenshots/03.png" height="72px" align="right">

Tests vertex layouts, by displaying a coloured rectangle.

### [Staging buffer](source/Test/04-StagingBuffer/)
<img src="./screenshots/04.png" height="72px" align="right">

Tests transfers to VRAM using staging buffers.

### [Texture 2D](source/Test/05-Texture2D/)
<img src="./screenshots/05.png" height="72px" align="right">

Tests texture 2D load and display.

### [Uniform Buffers](source/Test/06-UniformBuffers/)
<img src="./screenshots/06.png" height="72px" align="right">

Tests the usage of uniform buffers.

### [Texture 1D](source/Test/07-Texture1D/)
<img src="./screenshots/07.png" height="72px" align="right">

Tests texture 1D load and display.

### [Texture Buffer](source/Test/08-TextureBuffer/)
<img src="./screenshots/08.png" height="72px" align="right">

Tests texture buffer load and display.

### [Frame Buffer](source/Test/09-FrameBuffer/)
<img src="./screenshots/09.png" height="72px" align="right">

Tests frame buffers, and index buffers too.

### [Spinning cube](source/Test/10-SpinningCube/)
<img src="./screenshots/10.png" height="72px" align="right">

Tests depth buffer, depth test, and multiple uniform buffers.

### [Texture 3D](source/Test/11-Texture3D/)
<img src="./screenshots/11.png" height="72px" align="right">

Demonstrates load and display of a 3D texture.

### [Texture Cube](source/Test/12-TextureCube/)
<img src="./screenshots/12.png" height="72px" align="right">

Loads a cube texture and displays it as a skybox.

### [Push Constants](source/Test/13-PushConstants/)
<img src="./screenshots/13.png" height="72px" align="right">

Demonstrates the use of push constants, by displaying two cubes with different colour modifiers, stored in push constants.


### [Render to Cube](source/Test/14-RenderToCube/)
<img src="./screenshots/14.png" height="72px" align="right">

Tests render to cube, by taking an equirectangular texture, and loading it into a skybox.


## Sample applications

These applications use the library in a more complex way, closer to what would be done within an engine.

### [Object Loading](source/Samples/01-ObjectLoading/)
<img src="./screenshots/s01.png" height="72px" align="right">

Loads an object, with textures, and displays it.

### [Lighting](source/Samples/02-Lighting/)
<img src="./screenshots/s02.png" height="72px" align="right">

Applies lights to the previously loaded object.

### [Normal Mapping](source/Samples/03-NormalMapping/)
<img src="./screenshots/s03.png" height="72px" align="right">

Applies normal mapping.
