<p align="center">
  <a href="https://github.com/DragonJoker/Ashes/actions?query=workflow%3ABuild"><img alt="Build status" src="https://github.com/DragonJoker/Ashes/workflows/Build/badge.svg"></a>
</p>


Ashes
=====
Ashes is a drop-in replacement or ICD to Vulkan.  
It allows to write Vulkan code, and to select the rendering API that will be used.  
  
To build it, you can use CMake.

## Renderers available

- Vulkan: Ashes is a passthrough, when using Vulkan rendering API, and it has no additional cost if dynamic loader is used.
- OpenGL: From OpenGL 3.3 to OpenGL 4.6 (Core profile), it can be used directly as an ICD.
- Direct3D 11: From feature level 11.0.

## Supported platforms

- Windows.
- Linux.
- MacOS.

## How to use it

Three workflows are possible:

### ICD mode

Just set the environment variable VK_ICD_FILENAMES to one of the jsons in Ashes' install dir, and you're done. 

### Drop-in replacement mode

You load Ashes' shared library instead of Vulkan's, and you're done.  
The API selection will be done on first call of `vkGetInstanceProcAddr` (for dynamic loader), or on first Vulkan API call (for static loader).  

### Manual mode

You can ask for the list of loaded plugins, through a call to the function `int ashEnumeratePlugins( uint32_t * count , AshPluginDescription * plugins );`.  
It works as Vulkan ones : if `plugins` is set to `NULL`, `count` will then contain the number of loaded plugins.  

### API

The following API is available, in `ashes.h`:  

```c
typedef struct AshPluginFeatures
{
	// Whether or not the plugin supports buffer ranges.
	VkBool32 hasBufferRange;
	// Whether or not the plugin supports image textures.
	VkBool32 hasImageTexture;
	// Whether or not the plugin supports vertex base instance.
	VkBool32 hasBaseInstance;
	// Whether or not the plugin supports clearing of single images.
	VkBool32 hasClearTexImage;
	// Whether or not the plugin supports compute shaders.
	VkBool32 hasComputeShaders;
	// Whether or not the plugin supports shader storage buffers.
	VkBool32 hasStorageBuffers;
	// Whether or not the plugin supports persistent mapping.
	VkBool32 supportsPersistentMapping;
	// The plugin's maximum supported shader language version.
	uint32_t maxShaderLanguageVersion;
} AshPluginFeatures;

typedef struct AshPluginSupport
{
	// The plugin's priority (from 1 (low) to 10 (high)).
	uint32_t priority;
	// Whether or not the plugin is supported.
	VkBool32 supported;
} AshPluginSupport;

typedef struct AshPluginDescription
{
	//The plugin's short name.
	char name[16];
	//The plugin's description.
	char description[64];
	//The plugin's main entry point.
	PFN_vkGetInstanceProcAddr getInstanceProcAddr;
	//The plugin's supported features.
	AshPluginFeatures features;
	//The plugin's static functions (for static loader support).
	AshPluginStaticFunction functions;
	//The plugin's support informations.
	AshPluginSupport support;
} AshPluginDescription;

// Enumerates the available rendering APIs.
typedef void( VKAPI_PTR * PFN_ashEnumeratePluginsDescriptions )( uint32_t *, AshPluginDescription * );
Ashes_API void VKAPI_PTR ashEnumeratePluginsDescriptions( uint32_t * count
	, AshPluginDescription * pDescriptions );

// Defines the active rendering API.
typedef VkResult( VKAPI_PTR * PFN_ashSelectPlugin )( AshPluginDescription );
Ashes_API VkResult VKAPI_PTR ashSelectPlugin( AshPluginDescription description );

// Retrieves the active rendering API informations.
typedef VkResult( VKAPI_PTR * PFN_ashGetPluginDescription )( AshPluginDescription * );
Ashes_API VkResult VKAPI_PTR ashGetCurrentPluginDescription( AshPluginDescription * description );

```

From this, you can retrieve the supported rendering APIs, check the features they support, activate the one you want/can use.

Here is a small example, to select the rendering API from a command-line option:
```c
#define ASHES_VK_PROTOTYPES
#include <ashes/ashes.h>

AshPluginDescription * enumeratePlugins( uint32_t * pluginsCount )
{
	AshPluginDescription * result = NULL;
	ashEnumeratePluginsDescriptions( pluginsCount, NULL );

	if ( *pluginsCount )
	{
		result = malloc( ( *pluginsCount ) * sizeof( AshPluginDescription ) );
		ashEnumeratePluginsDescriptions( pluginsCount, result );
	}

	return result;
}

int selectPlugin( AshPluginDescription * plugins, uint32_t pluginsCount, char * option )
{
	int selectedPlugin = -1;

	if ( pluginsCount > 0 )
	{
		char name[17];

		for ( uint32_t i = 0; i < pluginsCount; ++i )
		{
			strncpy( name, "-", 16 );
			strncat( name, plugins[i].name, 16 );

			if ( strcmp( option, name ) == 0 )
			{
				selectedPlugin = i;
			}
		}
	}

	return selectedPlugin;
}

int main( int argc, char ** argv )
{
	uint32_t pluginsCount = 0u;
	int selectedPlugin = -1;
	AshPluginDescription * plugins = enumeratePlugins( &pluginsCount );

	for ( int i = 1; i < argc; ++i )
	{
		if ( selectedPlugin == -1 )
		{
			selectedPlugin = selectPlugin( plugins, pluginsCount, argv[i] );
		}
	}

	ashSelectPlugin( selectPlugin );

	// Now write classic Vulkan code.
	// ...
	//
}
```

## Contact

You can reach me on the Discord server dedicated to my projects: [DragonJoker's Lair](https://discord.gg/yVmaAvQ)

## Validation

Ashes is validated using Sascha Willems' Vulkan examples repository.  
I've forked it, to be able to test the drivers.

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
