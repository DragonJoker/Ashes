/*
* Copyright (c) 2015-2016 The Khronos Group Inc.
* Copyright (c) 2015-2016 Valve Corporation
* Copyright (c) 2015-2016 LunarG, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and/or associated documentation files (the "Materials"), to
* deal in the Materials without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Materials, and to permit persons to whom the Materials are
* furnished to do so, subject to the following conditions:
*
* The above copyright notice(s) and this permission notice shall be included in
* all copies or substantial portions of the Materials.
*
* THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
*
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE MATERIALS OR THE
* USE OR OTHER DEALINGS IN THE MATERIALS.
*
* Author: Courtney Goeltzenleuchter <courtney@LunarG.com>
* Author: David Pinedo <david@lunarg.com>
* Author: Mark Lobodzinski <mark@lunarg.com>
*/
#ifdef __GNUC__
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#else
#define strndup(p, n) strdup(p)
#endif

#include <Core/DebugReportCallback.hpp>
#include <Core/Device.hpp>
#include <Core/Exception.hpp>
#include <Core/Instance.hpp>
#include <Core/PhysicalDevice.hpp>
#include <Core/PlatformWindowHandle.hpp>
#include <Core/SwapChain.hpp>
#include <Miscellaneous/FormatProperties.hpp>
#include <Utils/DynamicLibrary.hpp>

#if ASHES_WIN32
#	include <fcntl.h>
#	include <io.h>
#endif // _WIN32

#if ASHES_XLIB || ASHES_XCB
#	include <X11/Xutil.h>
#endif

#if ASHES_MIR
#	warning "Vulkaninfo does not have code for Mir at this time"
#endif

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cinttypes>
#include <iostream>

#define ERR(err)                                                               \
    printf("%s:%d: failed with %s\n", __FILE__, __LINE__,                      \
           vk_result_string(err));

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#ifdef _WIN32

#define snprintf _snprintf
#define strdup _strdup

// Returns nonzero if the console is used only for this process. Will return
// zero if another process (such as cmd.exe) is also attached.
static int ConsoleIsExclusive( void )
{
	DWORD pids[2];
	DWORD num_pids = GetConsoleProcessList( pids, ARRAY_SIZE( pids ) );
	return num_pids <= 1;
}

#define WAIT_FOR_CONSOLE_DESTROY                                               \
    do {                                                                       \
        if ( ConsoleIsExclusive() ) Sleep(INFINITE);                           \
    } while (0)
#else
#define WAIT_FOR_CONSOLE_DESTROY
#endif

#define ERR_EXIT(err)                                                          \
    do {                                                                       \
        ERR(err);                                                              \
        fflush(stdout);                                                        \
        WAIT_FOR_CONSOLE_DESTROY;                                              \
        exit(-1);                                                              \
    } while (0)

#if defined(NDEBUG) && defined(__GNUC__)
#define U_ASSERT_ONLY __attribute__((unused))
#else
#define U_ASSERT_ONLY
#endif

#define MAX_QUEUE_TYPES 5
#define APP_SHORT_NAME "vulkaninfo"

static bool html_output = false;
static bool human_readable_output = true;
static bool json_output = false;
static uint32_t selected_gpu = 0;

class RendererPlugin;

struct LayerExtensionList
{
	ashes::LayerProperties layer_properties;
	ashes::ExtensionPropertiesArray extension_properties;
};

struct AppInstance
{
	RendererPlugin const * plugin;
	ashes::InstancePtr instance;
	ashes::DebugReportCallbackPtr debug_report;
	uint32_t instance_version;
	uint32_t vulkan_major;
	uint32_t vulkan_minor;
	uint32_t vulkan_patch;
	std::vector< LayerExtensionList > global_layers;
	ashes::ExtensionPropertiesArray global_extensions;
	ashes::StringArray inst_layers;
	ashes::StringArray inst_extensions;

	ashes::SurfacePtr surface;
	ashes::SurfaceCapabilities surface_capabilities;
	int width, height;


#if ASHES_WIN32
	HINSTANCE h_instance;  // Windows Instance
	HWND h_wnd;            // window handle
#elif ASHES_XCB
	xcb_connection_t *xcb_connection;
	xcb_screen_t *xcb_screen;
	xcb_window_t xcb_window;
#elif ASHES_XLIB
	Display *xlib_display;
	Window xlib_window;
#elif ASHES_ANDROID  // TODO
	ANativeWindow *window;
#endif
};

struct AppGpu
{
	uint32_t id;
	ashes::PhysicalDevicePtr obj;

	ashes::PhysicalDeviceProperties props;

	ashes::QueueFamilyPropertiesArray queue_props;
	std::vector< ashes::DeviceQueueCreateInfo > queue_reqs;

	struct AppInstance *inst;

	ashes::PhysicalDeviceMemoryProperties memory_props;

	ashes::PhysicalDeviceFeatures features;
	ashes::PhysicalDeviceLimits limits;

	ashes::ExtensionPropertiesArray device_extensions;
};

// return most severe flag only
static const char *DebugReportFlagString( const ashes::DebugReportFlags flags )
{
	if ( checkFlag( flags, ashes::DebugReportFlag::eError ) )
	{
		return "ERROR";
	}

	if ( checkFlag( flags, ashes::DebugReportFlag::eWarning ) )
	{
		return "WARNING";
	}

	if ( checkFlag( flags, ashes::DebugReportFlag::ePerformanceWarning ) )
	{
		return "PERF";
	}

	if ( checkFlag( flags, ashes::DebugReportFlag::eInformation ) )
	{
		return "INFO";
	}

	if ( checkFlag( flags, ashes::DebugReportFlag::eDebug ) )
	{
		return "DEBUG";
	}

	return "UNKNOWN";
}

static bool ASHES_API DbgCallback( ashes::DebugReportFlags msgFlags, ashes::DebugReportObjectType objType,
	uint64_t srcObject, size_t location, int32_t msgCode, const char *pLayerPrefix,
	const char *pMsg, void *pUserData )
{
	fprintf( stderr, "%s: [%s] Code %d : %s\n", DebugReportFlagString( msgFlags ), pLayerPrefix, msgCode, pMsg );
	fflush( stderr );

	// True is reserved for layer developers, and MAY mean calls are not distributed down the layer chain after validation error.
	// False SHOULD always be returned by apps:
	return false;
}

// Ashes specifics
#if ASHES_WIN32

#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

#include <direct.h>
#include <Shlobj.h>
#include <windows.h>

#elif defined( __linux__ )

#include <sys/stat.h>

#include <unistd.h>
#include <dirent.h>
#include <pwd.h>

#endif

#if ASHES_WIN32

static char constexpr PathSeparator = '\\';

namespace
{
	template< typename DirectoryFuncType, typename FileFuncType >
	bool traverse_folder( std::string const & folderPath
		, DirectoryFuncType directoryFunction
		, FileFuncType fileFunction )
	{
		assert( !folderPath.empty() );
		bool result = false;
		WIN32_FIND_DATAA findData;
		HANDLE handle = ::FindFirstFileA( ( folderPath + PathSeparator + "*.*" ).c_str(), &findData );

		if ( handle != INVALID_HANDLE_VALUE )
		{
			result = true;
			std::string name = findData.cFileName;

			if ( name != "." && name != ".." )
			{
				if ( ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY )
				{
					result = directoryFunction( folderPath + PathSeparator + name );
				}
				else
				{
					fileFunction( folderPath + PathSeparator + name );
				}
			}

			while ( result && ::FindNextFileA( handle, &findData ) == TRUE )
			{
				if ( findData.cFileName != name )
				{
					name = findData.cFileName;

					if ( name != "." && name != ".." )
					{
						if ( ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY )
						{
							result = directoryFunction( folderPath + PathSeparator + name );
						}
						else
						{
							fileFunction( folderPath + PathSeparator + name );
						}
					}
				}
			}

			::FindClose( handle );
		}

		return result;
	}
}

#else

static char constexpr PathSeparator = '/';

namespace
{
	template< typename DirectoryFuncType, typename FileFuncType >
	bool traverse_folder( std::string const & folderPath
		, DirectoryFuncType directoryFunction
		, FileFuncType fileFunction )
	{
		assert( !folderPath.empty() );
		bool result = false;
		DIR * dir;

		if ( ( dir = opendir( folderPath.c_str() ) ) == nullptr )
		{
			switch ( errno )
			{
			case EACCES:
				std::cerr << "Can't open dir : Permission denied - Directory : " << folderPath << std::endl;
				break;

			case EBADF:
				std::cerr << "Can't open dir : Invalid file descriptor - Directory : " << folderPath << std::endl;
				break;

			case EMFILE:
				std::cerr << "Can't open dir : Too many file descriptor in use - Directory : " << folderPath << std::endl;
				break;

			case ENFILE:
				std::cerr << "Can't open dir : Too many files currently open - Directory : " << folderPath << std::endl;
				break;

			case ENOENT:
				std::cerr << "Can't open dir : Directory doesn't exist - Directory : " << folderPath << std::endl;
				break;

			case ENOMEM:
				std::cerr << "Can't open dir : Insufficient memory - Directory : " << folderPath << std::endl;
				break;

			case ENOTDIR:
				std::cerr << "Can't open dir : <name> is not a directory - Directory : " << folderPath << std::endl;
				break;

			default:
				std::cerr << "Can't open dir : Unknown error - Directory : " << folderPath << std::endl;
				break;
			}

			result = false;
		}
		else
		{
			result = true;
			dirent * dirent;

			while ( result && ( dirent = readdir( dir ) ) != nullptr )
			{
				std::string name = dirent->d_name;

				if ( name != "." && name != ".." )
				{
					if ( dirent->d_type == DT_DIR )
					{
						result = directoryFunction( folderPath + PathSeparator + name );
					}
					else
					{
						fileFunction( folderPath + PathSeparator + name );
					}
				}
			}

			closedir( dir );
		}

		return result;
	}
}

#endif

std::string get_parent_path( std::string const & path )
{
	return path.substr( 0, path.find_last_of( PathSeparator ) );
}

std::string get_filename( std::string const & path )
{
	return path.substr( path.find_last_of( PathSeparator ) + 1 );
}

#if ASHES_WIN32

std::string get_bin_folder()
{
	std::string pathReturn;
	char path[FILENAME_MAX];
	DWORD result = ::GetModuleFileNameA( nullptr
		, path
		, sizeof( path ) );

	if ( result != 0 )
	{
		pathReturn = path;
	}

	pathReturn = get_parent_path( pathReturn );
	return pathReturn;
}

bool list_folder_files( std::string const & folderPath
	, std::vector< std::string > & files
	, bool recursive )
{
	struct FileFunction
	{
		explicit FileFunction( std::vector< std::string > & files )
			: m_files( files )
		{
		}
		void operator()( std::string const & path )
		{
			m_files.push_back( path );
		}
		std::vector< std::string > & m_files;
	};

	if ( recursive )
	{
		struct DirectoryFunction
		{
			explicit DirectoryFunction( std::vector< std::string > & files )
				: m_files( files )
			{
			}
			bool operator()( std::string const & path )
			{
				return traverse_folder( path
					, DirectoryFunction( m_files )
					, FileFunction( m_files ) );
			}
			std::vector< std::string > & m_files;
		};

		return traverse_folder( folderPath
			, DirectoryFunction( files )
			, FileFunction( files ) );
	}
	else
	{
		struct DirectoryFunction
		{
			DirectoryFunction()
			{
			}
			bool operator()( std::string const & path )
			{
				return true;
			}
		};

		return traverse_folder( folderPath
			, DirectoryFunction()
			, FileFunction( files ) );
	}
}

#elif defined( __linux__ )

std::string get_bin_folder()
{
	std::string pathReturn;
	char path[FILENAME_MAX];
	char buffer[32];
	sprintf( buffer, "/proc/%d/exe", getpid() );
	int bytes = std::min< std::size_t >( readlink( buffer
		, path
		, sizeof( path ) )
		, sizeof( path ) - 1 );

	if ( bytes > 0 )
	{
		path[bytes] = '\0';
		pathReturn = path;
	}

	pathReturn = get_parent_path( pathReturn );
	return pathReturn;
}

bool list_folder_files( std::string const & folderPath
	, std::vector< std::string > & files
	, bool recursive )
{
	struct FileFunction
	{
		explicit FileFunction( std::vector< std::string > & files )
			: m_files( files )
		{
		}
		void operator()( std::string const & path )
		{
			m_files.push_back( path );
		}

		std::vector< std::string > & m_files;
	};

	if ( recursive )
	{
		struct DirectoryFunction
		{
			explicit DirectoryFunction( std::vector< std::string > & files )
				: m_files( files )
			{
			}
			bool operator()( std::string const & path )
			{
				return traverse_folder( path
					, DirectoryFunction( m_files )
					, FileFunction( m_files ) );
			}
			std::vector< std::string > & m_files;
		};

		return traverse_folder( folderPath
			, DirectoryFunction( files )
			, FileFunction( files ) );
	}
	else
	{
		struct DirectoryFunction
		{
			DirectoryFunction()
			{
			}
			bool operator()( std::string const & path )
			{
				return true;
			}
		};

		return traverse_folder( folderPath
			, DirectoryFunction()
			, FileFunction( files ) );
	}
}

#endif

class RendererPlugin
{
	using CreatorFunction = ashes::Result( *)( ashes::InstanceCreateInfo, ashes::Instance ** );
	using VersionEnumeratorFunction = ashes::Result( *)( uint32_t * );
	using LayerPropertiesEnumeratorFunction = ashes::Result( *)( uint32_t *, ashes::LayerProperties * );
	using ExtensionPropertiesEnumeratorFunction = ashes::Result( *)( char const * const, uint32_t *, ashes::ExtensionProperties * );
	using NamerFunction = char const *( *)( );

public:
	RendererPlugin( ashes::DynamicLibrary library )
		: m_library{ std::move( library ) }
		, m_creator{ nullptr }
	{
		if ( !m_library.getFunction( "createInstance", m_creator ) )
		{
			throw std::runtime_error{ "[" + get_filename( m_library.getPath() ) + "] is not a renderer plugin" };
		}

		if ( !m_library.getFunction( "enumerateVersion", m_enumerateVersion ) )
		{
			throw std::runtime_error{ "[" + get_filename( m_library.getPath() ) + "] is not a renderer plugin" };
		}

		if ( !m_library.getFunction( "enumerateLayerProperties", m_enumerateLayerProperties ) )
		{
			throw std::runtime_error{ "[" + get_filename( m_library.getPath() ) + "] is not a renderer plugin" };
		}

		if ( !m_library.getFunction( "enumerateExtensionProperties", m_enumerateExtensionProperties ) )
		{
			throw std::runtime_error{ "[" + get_filename( m_library.getPath() ) + "] is not a renderer plugin" };
		}

		NamerFunction shortNamer;

		if ( !m_library.getFunction( "getShortName", shortNamer ) )
		{
			throw std::runtime_error{ "[" + get_filename( m_library.getPath() ) + "] is not a renderer plugin" };
		}

		NamerFunction fullNamer;

		if ( !m_library.getFunction( "getFullName", fullNamer ) )
		{
			throw std::runtime_error{ "[" + get_filename( m_library.getPath() ) + "] is not a renderer plugin" };
		}

		m_shortName = shortNamer();
		m_fullName = fullNamer();
	}

	uint32_t enumerateVersion()const
	{
		uint32_t result;
		auto res = m_enumerateVersion( &result );

		if ( res != ashes::Result::eSuccess )
		{
			throw ashes::Exception{ res, "Version retrieval" };
		}

		return result;
	}

	ashes::LayerPropertiesArray enumerateLayerProperties()const
	{
		uint32_t count;
		ashes::LayerPropertiesArray result;
		ashes::Result res;

		do
		{
			res = m_enumerateLayerProperties( &count, nullptr );

			if ( count )
			{
				result.resize( count );
				res = m_enumerateLayerProperties( &count, result.data() );
			}
		}
		while ( res == ashes::Result::eIncomplete );

		if ( res != ashes::Result::eSuccess )
		{
			throw ashes::Exception{ res, "Instance layers retrieval" };
		}

		return result;
	}

	ashes::ExtensionPropertiesArray enumerateExtensionProperties( std::string const & layerName )const
	{
		uint32_t count{};
		ashes::ExtensionPropertiesArray result;
		ashes::Result res{};

		do
		{
			res = m_enumerateExtensionProperties( layerName.empty() ? nullptr : layerName.c_str()
				, &count
				, nullptr );

			if ( count )
			{
				result.resize( count );
				res = m_enumerateExtensionProperties( layerName.empty() ? nullptr : layerName.c_str()
					, &count
					, result.data() );
			}
		}
		while ( res == ashes::Result::eIncomplete );

		if ( res != ashes::Result::eSuccess )
		{
			throw ashes::Exception{ res, "Instance layer [" + layerName + "] extensions retrieval" };
		}

		return result;
	}

	ashes::InstancePtr create( ashes::InstanceCreateInfo createInfo )const
	{
		ashes::Instance * result;
		auto res = m_creator( std::move( createInfo ), &result );

		if ( res != ashes::Result::eSuccess )
		{
			throw ashes::Exception{ res, "Instance creation" };
		}

		return ashes::InstancePtr{ result };
	}

	inline std::string const & getShortName()const
	{
		return m_shortName;
	}

private:
	ashes::DynamicLibrary m_library;
	CreatorFunction m_creator;
	VersionEnumeratorFunction m_enumerateVersion;
	LayerPropertiesEnumeratorFunction m_enumerateLayerProperties;
	ExtensionPropertiesEnumeratorFunction m_enumerateExtensionProperties;
	std::string m_shortName;
	std::string m_fullName;
};

class InstanceFactory
{
protected:
	using Key = std::string;
	using PtrType = RendererPlugin const *;
	using ObjMap = std::map< Key, PtrType >;

public:
	void registerType( Key const & key, PtrType plugin )
	{
		m_registered[key] = plugin;
	}

	template< typename ... Parameters >
	PtrType findPlugin( Key const & key )const
	{
		PtrType result{ nullptr };
		auto it = m_registered.find( key );

		if ( it != m_registered.end() )
		{
			result = it->second;
		}
		else
		{
			static std::string const Error = "Unknown object type";
			std::cerr << Error << std::endl;
			throw std::runtime_error{ Error };
		}

		return result;
	}

private:
	ObjMap m_registered;
};

std::vector< RendererPlugin > list_plugins( InstanceFactory & factory )
{
	std::vector< std::string > files;
	std::vector< RendererPlugin > result;

	if ( list_folder_files( get_bin_folder(), files, false ) )
	{
		for ( auto file : files )
		{
			if ( file.find( ".dll" ) != std::string::npos
				|| file.find( ".so" ) != std::string::npos )
				try
			{
				ashes::DynamicLibrary lib{ file };
				result.emplace_back( std::move( lib ) );
			}
			catch ( std::exception & exc )
			{
				std::cerr << exc.what() << std::endl;
			}
		}

		for ( auto & plugin : result )
		{
			factory.registerType( plugin.getShortName(), &plugin );
		}
	}

	return result;
}
static std::string VkResultString( ashes::Result err )
{
	return getName( err );
}

static std::string VkPhysicalDeviceTypeString( ashes::PhysicalDeviceType type )
{
	return getName( type );
}

static std::string VkFormatString( ashes::Format fmt )
{
	return getName( fmt );
}

#if ASHES_XCB || ASHES_XLIB || ASHES_WIN32
static std::string VkPresentModeString( ashes::PresentMode mode )
{
	switch ( mode )
	{
	case ashes::PresentMode::eImmediate:
		return "IMMEDIATE_KHR";

	case ashes::PresentMode::eMailbox:
		return "MAILBOX_KHR";

	case ashes::PresentMode::eFifo:
		return "FIFO_KHR";

	case ashes::PresentMode::eFifoRelaxed:
		return "FIFO_RELAXED_KHR";

	case ashes::PresentMode::eSharedDemandRefresh:
		return "SHARED_DEMAND_REFRESH_KHR";

	case ashes::PresentMode::eSharedContinuousRefresh:
		return "SHARED_CONTINUOUS_REFRESH_KHR";
	}

	return std::string{};
}
#endif

static bool CheckExtensionEnabled( const char * extension_to_check
	, ashes::StringArray const & extension_list )
{
	return extension_list.end() != std::find( extension_list.begin()
		, extension_list.end()
		, extension_to_check );
}

static void ExtractVersion( uint32_t version
	, uint32_t * major
	, uint32_t * minor
	, uint32_t * patch )
{
	*major = version >> 22;
	*minor = ( version >> 12 ) & 0x3ff;
	*patch = version & 0xfff;
}

static void AppGetPhysicalDeviceLayerExtensions( AppGpu * gpu
	, std::string const & layer_name
	, std::vector< ashes::ExtensionProperties > & extension_properties )
{
	extension_properties = gpu->obj->enumerateExtensionProperties( layer_name );
}

static void AppGetGlobalLayerExtensions( AppInstance *inst
	, std::string const & layer_name
	, ashes::ExtensionPropertiesArray & extension_properties )
{
	extension_properties = inst->plugin->enumerateExtensionProperties( layer_name );
}

/* Gets a list of layer and instance extensions */
static void AppGetInstanceExtensions( AppInstance * inst )
{
	auto global_layer_properties = inst->plugin->enumerateLayerProperties();
	inst->global_layers.resize( global_layer_properties.size() );

	for ( uint32_t i = 0; i < global_layer_properties.size(); ++i )
	{
		ashes::LayerProperties & src_info = global_layer_properties[i];
		auto & dst_info = inst->global_layers[i];
		dst_info.layer_properties = src_info;

		// Save away layer extension info for report
		// Gets layer extensions, if first parameter is not NULL
		AppGetGlobalLayerExtensions( inst
			, src_info.layerName
			, dst_info.extension_properties );
	}

	// Collect global extensions
	// Gets instance extensions, if no layer was specified in the first
	// paramteter
	AppGetGlobalLayerExtensions( inst
		, std::string{}
		, inst->global_extensions );
}

// Prints opening code for html output file
void PrintHtmlHeader( FILE *out )
{
	fprintf( out, "<!doctype html>\n" );
	fprintf( out, "<html>\n" );
	fprintf( out, "\t<head>\n" );
	fprintf( out, "\t\t<title>vulkaninfo</title>\n" );
	fprintf( out, "\t\t<style type='text/css'>\n" );
	fprintf( out, "\t\thtml {\n" );
	fprintf( out, "\t\t\tbackground-color: #0b1e48;\n" );
	fprintf( out, "\t\t\tbackground-image: url(\"https://vulkan.lunarg.com/img/bg-starfield.jpg\");\n" );
	fprintf( out, "\t\t\tbackground-position: center;\n" );
	fprintf( out, "\t\t\t-webkit-background-size: cover;\n" );
	fprintf( out, "\t\t\t-moz-background-size: cover;\n" );
	fprintf( out, "\t\t\t-o-background-size: cover;\n" );
	fprintf( out, "\t\t\tbackground-size: cover;\n" );
	fprintf( out, "\t\t\tbackground-attachment: fixed;\n" );
	fprintf( out, "\t\t\tbackground-repeat: no-repeat;\n" );
	fprintf( out, "\t\t\theight: 100%%;\n" );
	fprintf( out, "\t\t}\n" );
	fprintf( out, "\t\t#header {\n" );
	fprintf( out, "\t\t\tz-index: -1;\n" );
	fprintf( out, "\t\t}\n" );
	fprintf( out, "\t\t#header>img {\n" );
	fprintf( out, "\t\t\tposition: absolute;\n" );
	fprintf( out, "\t\t\twidth: 160px;\n" );
	fprintf( out, "\t\t\tmargin-left: -280px;\n" );
	fprintf( out, "\t\t\ttop: -10px;\n" );
	fprintf( out, "\t\t\tleft: 50%%;\n" );
	fprintf( out, "\t\t}\n" );
	fprintf( out, "\t\t#header>h1 {\n" );
	fprintf( out, "\t\t\tfont-family: Arial, \"Helvetica Neue\", Helvetica, sans-serif;\n" );
	fprintf( out, "\t\t\tfont-size: 44px;\n" );
	fprintf( out, "\t\t\tfont-weight: 200;\n" );
	fprintf( out, "\t\t\ttext-shadow: 4px 4px 5px #000;\n" );
	fprintf( out, "\t\t\tcolor: #eee;\n" );
	fprintf( out, "\t\t\tposition: absolute;\n" );
	fprintf( out, "\t\t\twidth: 400px;\n" );
	fprintf( out, "\t\t\tmargin-left: -80px;\n" );
	fprintf( out, "\t\t\ttop: 8px;\n" );
	fprintf( out, "\t\t\tleft: 50%%;\n" );
	fprintf( out, "\t\t}\n" );
	fprintf( out, "\t\tbody {\n" );
	fprintf( out, "\t\t\tfont-family: Consolas, monaco, monospace;\n" );
	fprintf( out, "\t\t\tfont-size: 14px;\n" );
	fprintf( out, "\t\t\tline-height: 20px;\n" );
	fprintf( out, "\t\t\tcolor: #eee;\n" );
	fprintf( out, "\t\t\theight: 100%%;\n" );
	fprintf( out, "\t\t\tmargin: 0;\n" );
	fprintf( out, "\t\t\toverflow: hidden;\n" );
	fprintf( out, "\t\t}\n" );
	fprintf( out, "\t\t#wrapper {\n" );
	fprintf( out, "\t\t\tbackground-color: rgba(0, 0, 0, 0.7);\n" );
	fprintf( out, "\t\t\tborder: 1px solid #446;\n" );
	fprintf( out, "\t\t\tbox-shadow: 0px 0px 10px #000;\n" );
	fprintf( out, "\t\t\tpadding: 8px 12px;\n\n" );
	fprintf( out, "\t\t\tdisplay: inline-block;\n" );
	fprintf( out, "\t\t\tposition: absolute;\n" );
	fprintf( out, "\t\t\ttop: 80px;\n" );
	fprintf( out, "\t\t\tbottom: 25px;\n" );
	fprintf( out, "\t\t\tleft: 50px;\n" );
	fprintf( out, "\t\t\tright: 50px;\n" );
	fprintf( out, "\t\t\toverflow: auto;\n" );
	fprintf( out, "\t\t}\n" );
	fprintf( out, "\t\tdetails>details {\n" );
	fprintf( out, "\t\t\tmargin-left: 22px;\n" );
	fprintf( out, "\t\t}\n" );
	fprintf( out, "\t\tdetails>summary:only-child::-webkit-details-marker {\n" );
	fprintf( out, "\t\t\tdisplay: none;\n" );
	fprintf( out, "\t\t}\n" );
	fprintf( out, "\t\t.var, .type, .val {\n" );
	fprintf( out, "\t\t\tdisplay: inline;\n" );
	fprintf( out, "\t\t}\n" );
	fprintf( out, "\t\t.var {\n" );
	fprintf( out, "\t\t}\n" );
	fprintf( out, "\t\t.type {\n" );
	fprintf( out, "\t\t\tcolor: #acf;\n" );
	fprintf( out, "\t\t\tmargin: 0 12px;\n" );
	fprintf( out, "\t\t}\n" );
	fprintf( out, "\t\t.val {\n" );
	fprintf( out, "\t\t\tcolor: #afa;\n" );
	fprintf( out, "\t\t\tbackground: #222;\n" );
	fprintf( out, "\t\t\ttext-align: right;\n" );
	fprintf( out, "\t\t}\n" );
	fprintf( out, "\t\t</style>\n" );
	fprintf( out, "\t</head>\n" );
	fprintf( out, "\t<body>\n" );
	fprintf( out, "\t\t<div id='header'>\n" );
	fprintf( out, "\t\t\t<h1>vulkaninfo</h1>\n" );
	fprintf( out, "\t\t</div>\n" );
	fprintf( out, "\t\t<div id='wrapper'>\n" );
}

// Prints closing code for html output file
void PrintHtmlFooter( FILE *out )
{
	fprintf( out, "\t\t</div>\n" );
	fprintf( out, "\t</body>\n" );
	fprintf( out, "</html>" );
}

// Prints opening code for json output file
void PrintJsonHeader( const int vulkan_major, const int vulkan_minor, const int vulkan_patch )
{
	printf( "{\n" );
	printf( "\t\"$schema\": \"https://schema.khronos.org/vulkan/devsim_1_0_0.json#\",\n" );
	printf( "\t\"comments\": {\n" );
	printf( "\t\t\"desc\": \"JSON configuration file describing GPU %u. Generated using the vulkaninfo program.\",\n", selected_gpu );
	printf( "\t\t\"vulkanApiVersion\": \"%d.%d.%d\"\n", vulkan_major, vulkan_minor, vulkan_patch );
	printf( "\t}" );
}

// Checks if current argument specifies json output, interprets/updates gpu selection
bool CheckForJsonOption( const char *arg )
{
	if ( strncmp( "--json", arg, 6 ) == 0 || strcmp( arg, "-j" ) == 0 )
	{
		if ( strlen( arg ) > 7 && strncmp( "--json=", arg, 7 ) == 0 )
		{
			selected_gpu = strtol( arg + 7, NULL, 10 );
		}
		human_readable_output = false;
		json_output = true;
		return true;
	}
	else
	{
		return false;
	}
}

static void AppCompileInstanceExtensionsToEnable( struct AppInstance *inst )
{
// Get all supported Instance extensions (excl. layer-provided ones)
	inst->inst_extensions.reserve( inst->global_extensions.size() );

	for ( auto & ext : inst->global_extensions )
	{
		inst->inst_extensions.push_back( ext.extensionName );
	}
}

std::string CheckForInstanceOption( const char * arg )
{
	std::string result = arg;
	result = result.substr( result.find( '-' ) + 1 );
	result = result.substr( result.find( '/' ) + 1 );
	return result;
}

void print_usage( char *argv0 );
std::string ProcessCommandLine( int argc
	, char ** argv
	, FILE ** out )
{
	std::string result = "vk";

	// Combinations of output: html only, html AND json, json only, human readable only
	for ( int i = 1; i < argc; ++i )
	{
		if ( !CheckForJsonOption( argv[i] ) )
		{
			if ( strcmp( argv[i], "--html" ) == 0 )
			{
				human_readable_output = false;
				html_output = true;
				continue;
			}
			else if ( strcmp( argv[i], "--help" ) == 0 || strcmp( argv[i], "-h" ) == 0 )
			{
				print_usage( argv[0] );
				exit( 1 );
			}
			else
			{
				result = CheckForInstanceOption( argv[i] );
			}
		}
	}

	return result;
}

static void AppCreateInstance( AppInstance * inst )
{
	inst->instance_version = inst->plugin->enumerateVersion();

	inst->vulkan_major = ashes::getMajor( inst->instance_version );
	inst->vulkan_minor = ashes::getMinor( inst->instance_version );
	inst->vulkan_patch = ashes::getPatch( ashes::HEADER_VERSION );

	AppGetInstanceExtensions( inst );

	const ashes::DebugReportCallbackCreateInfo dbg_info
	{
		ashes::DebugReportFlag::eError | ashes::DebugReportFlag::eWarning,
		DbgCallback,
		nullptr,
	};

	const ashes::ApplicationInfo app_info
	{
		APP_SHORT_NAME,
		1,
		APP_SHORT_NAME,
		1,
		ashes::API_VERSION_1_0,
	};

	AppCompileInstanceExtensionsToEnable( inst );
	const ashes::InstanceCreateInfo inst_info
	{
		0,
		app_info,
		{},
		inst->inst_extensions,
	};
	inst->instance = inst->plugin->create( inst_info );
	inst->debug_report = inst->instance->createDebugReportCallback( std::move( dbg_info ) );

	//AppLoadInstanceCommands( inst );
}

static void AppDestroyInstance( struct AppInstance *inst )
{
	inst->surface.reset();
	inst->debug_report.reset();
	inst->instance.reset();
}

static void AppGpuInit( AppGpu * gpu
	, AppInstance * inst
	, uint32_t id
	, ashes::PhysicalDevicePtr obj )
{
	uint32_t i;

	gpu->id = id;
	gpu->obj = std::move( obj );
	gpu->inst = inst;

	gpu->props = gpu->obj->getProperties();

#if 0
	if ( CheckExtensionEnabled( VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, gpu->inst->inst_extensions,
		gpu->inst->inst_extensions_count ) )
	{
		struct pNextChainBuildingBlockInfo chain_info[] = {
			{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_PROPERTIES_EXT,
			.mem_size = sizeof( VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_POINT_CLIPPING_PROPERTIES_KHR,
		.mem_size = sizeof( VkPhysicalDevicePointClippingPropertiesKHR ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES_KHR,
		.mem_size = sizeof( VkPhysicalDevicePushDescriptorPropertiesKHR ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DISCARD_RECTANGLE_PROPERTIES_EXT,
		.mem_size = sizeof( VkPhysicalDeviceDiscardRectanglePropertiesEXT ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES_KHR,
		.mem_size = sizeof( VkPhysicalDeviceMultiviewPropertiesKHR ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES_KHR,
		.mem_size = sizeof( VkPhysicalDeviceMaintenance3PropertiesKHR ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES_KHR, .mem_size = sizeof( VkPhysicalDeviceIDPropertiesKHR ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES_KHR,
		.mem_size = sizeof( VkPhysicalDeviceDriverPropertiesKHR ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT_CONTROLS_PROPERTIES_KHR,
		.mem_size = sizeof( VkPhysicalDeviceFloatControlsPropertiesKHR ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PCI_BUS_INFO_PROPERTIES_EXT,
		.mem_size = sizeof( VkPhysicalDevicePCIBusInfoPropertiesEXT ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_PROPERTIES_EXT,
		.mem_size = sizeof( VkPhysicalDeviceTransformFeedbackPropertiesEXT ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_PROPERTIES_EXT,
		.mem_size = sizeof( VkPhysicalDeviceFragmentDensityMapPropertiesEXT ) } };

		uint32_t chain_info_len = ARRAY_SIZE( chain_info );

		gpu->props2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR;
		buildpNextChain( ( struct VkStructureHeader * )&gpu->props2, chain_info, chain_info_len );

		inst->vkGetPhysicalDeviceProperties2KHR( gpu->obj, &gpu->props2 );
	}
#endif
	/* get queue count */
	gpu->queue_props = gpu->obj->getQueueFamilyProperties();

#if 0
	if ( CheckExtensionEnabled( VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, gpu->inst->inst_extensions,
		gpu->inst->inst_extensions_count ) )
	{
		gpu->queue_props2 = malloc( sizeof( gpu->queue_props2[0] ) * gpu->queue_count );

		if ( !gpu->queue_props2 )
		{
			ERR_EXIT( VK_ERROR_OUT_OF_HOST_MEMORY );
		}

		for ( i = 0; i < gpu->queue_count; ++i )
		{
			gpu->queue_props2[i].sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2_KHR;
			gpu->queue_props2[i].pNext = NULL;
		}

		inst->vkGetPhysicalDeviceQueueFamilyProperties2KHR( gpu->obj, &gpu->queue_count, gpu->queue_props2 );
	}
#endif

	/* set up queue requests */
	gpu->queue_reqs.resize( gpu->queue_props.size() );

	for ( i = 0; i < gpu->queue_props.size(); ++i )
	{
		std::vector< float > queue_priorities;
#if 0
		if ( CheckExtensionEnabled( VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, gpu->inst->inst_extensions,
			gpu->inst->inst_extensions_count ) )
		{
			queue_priorities.resize( gpu->queue_props2[i].queueFamilyProperties.queueCount );
		}
		else
#endif
		{
			queue_priorities.resize( gpu->queue_props[i].queueCount );
		}

		gpu->queue_reqs[i].flags = 0;
		gpu->queue_reqs[i].queueFamilyIndex = i;
		gpu->queue_reqs[i].queuePriorities = queue_priorities;
	}

	gpu->memory_props = gpu->obj->getMemoryProperties();
	gpu->features = gpu->obj->getFeatures();

#if 0
	if ( CheckExtensionEnabled( VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, gpu->inst->inst_extensions,
		gpu->inst->inst_extensions_count ) )
	{
		gpu->memory_props2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2_KHR;
		gpu->memory_props2.pNext = NULL;

		inst->vkGetPhysicalDeviceMemoryProperties2KHR( gpu->obj, &gpu->memory_props2 );

		struct pNextChainBuildingBlockInfo chain_info[] = {
			{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR,
			.mem_size = sizeof( VkPhysicalDevice8BitStorageFeaturesKHR ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES_KHR,
		.mem_size = sizeof( VkPhysicalDevice16BitStorageFeaturesKHR ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES_KHR,
		.mem_size = sizeof( VkPhysicalDeviceSamplerYcbcrConversionFeaturesKHR ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTER_FEATURES_KHR,
		.mem_size = sizeof( VkPhysicalDeviceVariablePointerFeaturesKHR ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_FEATURES_EXT,
		.mem_size = sizeof( VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES_KHR,
		.mem_size = sizeof( VkPhysicalDeviceMultiviewFeaturesKHR ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT16_INT8_FEATURES_KHR,
		.mem_size = sizeof( VkPhysicalDeviceFloat16Int8FeaturesKHR ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT64_FEATURES_KHR,
		.mem_size = sizeof( VkPhysicalDeviceShaderAtomicInt64FeaturesKHR ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_FEATURES_EXT,
		.mem_size = sizeof( VkPhysicalDeviceTransformFeedbackFeaturesEXT ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES_EXT,
		.mem_size = sizeof( VkPhysicalDeviceScalarBlockLayoutFeaturesEXT ) },
		{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_FEATURES_EXT,
		.mem_size = sizeof( VkPhysicalDeviceFragmentDensityMapFeaturesEXT ) } };

		uint32_t chain_info_len = ARRAY_SIZE( chain_info );

		gpu->features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
		buildpNextChain( ( struct VkStructureHeader * )&gpu->features2, chain_info, chain_info_len );

		inst->vkGetPhysicalDeviceFeatures2KHR( gpu->obj, &gpu->features2 );
	}
#endif

	AppGetPhysicalDeviceLayerExtensions( gpu
		, std::string{}
		, gpu->device_extensions );
}

static void AppGpuDestroy( AppGpu *gpu )
{
	gpu->obj.reset();
}

// clang-format off

//-----------------------------------------------------------

//---------------------------Win32---------------------------
#if ASHES_WIN32
#include <Windows.h>

// MS-Windows event handling function:
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return ( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
}

static void AppCreateWin32Window( AppInstance *inst )
{
	inst->h_instance = GetModuleHandle( NULL );

	WNDCLASSEXA win_class;

	// Initialize the window class structure:
	win_class.cbSize = sizeof( WNDCLASSEX );
	win_class.style = CS_HREDRAW | CS_VREDRAW;
	win_class.lpfnWndProc = WndProc;
	win_class.cbClsExtra = 0;
	win_class.cbWndExtra = 0;
	win_class.hInstance = inst->h_instance;
	win_class.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	win_class.hCursor = LoadCursor( NULL, IDC_ARROW );
	win_class.hbrBackground = ( HBRUSH )GetStockObject( WHITE_BRUSH );
	win_class.lpszMenuName = NULL;
	win_class.lpszClassName = APP_SHORT_NAME;
	win_class.hInstance = inst->h_instance;
	win_class.hIconSm = LoadIcon( NULL, IDI_WINLOGO );
	// Register window class:
	if ( !RegisterClassExA( &win_class ) )
	{
		// It didn't work, so try to give a useful error:
		printf( "Failed to register the window class!\n" );
		fflush( stdout );
		exit( 1 );
	}
	// Create window with the registered class:
	RECT wr = { 0, 0, inst->width, inst->height };
	AdjustWindowRect( &wr, WS_OVERLAPPEDWINDOW, FALSE );
	inst->h_wnd = CreateWindowExA( 0,
		APP_SHORT_NAME,       // class name
		APP_SHORT_NAME,       // app name
							  //WS_VISIBLE | WS_SYSMENU |
		WS_OVERLAPPEDWINDOW,  // window style
		100, 100,             // x/y coords
		wr.right - wr.left,   // width
		wr.bottom - wr.top,   // height
		NULL,                 // handle to parent
		NULL,                 // handle to menu
		inst->h_instance,      // hInstance
		NULL );                // no extra parameters
	if ( !inst->h_wnd )
	{
		// It didn't work, so try to give a useful error:
		printf( "Failed to create a window!\n" );
		fflush( stdout );
		exit( 1 );
	}
}

static void AppCreateWin32Surface( AppInstance *inst
	, ashes::PhysicalDevice const & gpu )
{
	inst->surface = inst->instance->createSurface( gpu
		, ashes::WindowHandle{ std::make_unique< ashes::IMswWindowHandle >( inst->h_instance, inst->h_wnd ) } );
}

static void AppDestroyWin32Window( AppInstance *inst )
{
	DestroyWindow( inst->h_wnd );
}
#endif //ASHES_WIN32
//-----------------------------------------------------------

#if ASHES_XCB     || \
    ASHES_XLIB    || \
    ASHES_WIN32
static void AppDestroySurface( AppInstance *inst )
{ //same for all platforms
}
#endif

//----------------------------XCB----------------------------

#if ASHES_XCB
static void AppCreateXcbWindow( AppInstance *inst )
{
	//--Init Connection--
	const xcb_setup_t *setup;
	xcb_screen_iterator_t iter;
	int scr;

	// API guarantees non-null xcb_connection
	inst->xcb_connection = xcb_connect( NULL, &scr );
	int conn_error = xcb_connection_has_error( inst->xcb_connection );
	if ( conn_error )
	{
		fprintf( stderr, "XCB failed to connect to the X server due to error:%d.\n", conn_error );
		fflush( stderr );
		inst->xcb_connection = NULL;
	}

	setup = xcb_get_setup( inst->xcb_connection );
	iter = xcb_setup_roots_iterator( setup );
	while ( scr-- > 0 )
	{
		xcb_screen_next( &iter );
	}

	inst->xcb_screen = iter.data;
	//-------------------

	inst->xcb_window = xcb_generate_id( inst->xcb_connection );
	xcb_create_window( inst->xcb_connection, XCB_COPY_FROM_PARENT, inst->xcb_window,
		inst->xcb_screen->root, 0, 0, inst->width, inst->height, 0,
		XCB_WINDOW_CLASS_INPUT_OUTPUT, inst->xcb_screen->root_visual,
		0, NULL );

	xcb_intern_atom_cookie_t cookie = xcb_intern_atom( inst->xcb_connection, 1, 12, "WM_PROTOCOLS" );
	xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply( inst->xcb_connection, cookie, 0 );
	free( reply );
}

static void AppCreateXcbSurface( AppInstance *inst
	, ashes::PhysicalDevice const & gpu )
{
	if ( !inst->xcb_connection )
	{
		return;
	}

	inst->surface = inst->instance->createSurface( gpu
		, ashes::WindowHandle{ std::make_unique< ashes::IXcbWindowHandle >( inst->xcb_connection, inst->xcb_window ) } );
}

static void AppDestroyXcbWindow( AppInstance *inst )
{
	if ( !inst->xcb_connection )
	{
		return; // Nothing to destroy
	}

	xcb_destroy_window( inst->xcb_connection, inst->xcb_window );
	xcb_disconnect( inst->xcb_connection );
}
//VK_USE_PLATFORM_XCB_KHR
//-----------------------------------------------------------

//----------------------------XLib---------------------------
#elif ASHES_XLIB
static void AppCreateXlibWindow( AppInstance *inst )
{
	long visualMask = VisualScreenMask;
	int numberOfVisuals;

	inst->xlib_display = XOpenDisplay( NULL );
	if ( inst->xlib_display == NULL )
	{
		printf( "XLib failed to connect to the X server.\nExiting ...\n" );
		fflush( stdout );
		exit( 1 );
	}

	XVisualInfo vInfoTemplate = {};
	vInfoTemplate.screen = DefaultScreen( inst->xlib_display );
	XVisualInfo *visualInfo = XGetVisualInfo( inst->xlib_display, visualMask,
		&vInfoTemplate, &numberOfVisuals );
	inst->xlib_window = XCreateWindow(
		inst->xlib_display, RootWindow( inst->xlib_display, vInfoTemplate.screen ), 0, 0,
		inst->width, inst->height, 0, visualInfo->depth, InputOutput,
		visualInfo->visual, 0, NULL );

	XSync( inst->xlib_display, false );
}

static void AppCreateXlibSurface( AppInstance *inst
	, ashes::PhysicalDevice const & gpu )
{
	inst->surface = inst->instance->createSurface( gpu
		, ashes::WindowHandle{ std::make_unique< ashes::IXWindowHandle >( inst->xlib_window, inst->xlib_display ) } );
}

static void AppDestroyXlibWindow( AppInstance *inst )
{
	XDestroyWindow( inst->xlib_display, inst->xlib_window );
	XCloseDisplay( inst->xlib_display );
}
#endif //VK_USE_PLATFORM_XLIB_KHR
//-----------------------------------------------------------

#if ASHES_XCB     || \
    ASHES_XLIB    || \
    ASHES_WIN32
static int AppDumpSurfaceFormats( AppInstance * inst
	, AppGpu const * gpu
	, FILE *out )
{
	// Get the list of ashes::Format's that are supported
	std::vector< ashes::SurfaceFormat > surf_formats;
#if 0
	VkSurfaceFormat2KHR *surf_formats2 = NULL;

	const VkPhysicalDeviceSurfaceInfo2KHR surface_info2 = { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR,
		.surface = inst->surface };

	if ( CheckExtensionEnabled( VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME, gpu->inst->inst_extensions,
		gpu->inst->inst_extensions_count ) )
	{
		err = inst->vkGetPhysicalDeviceSurfaceFormats2KHR( gpu->obj, &surface_info2, &format_count, NULL );
		if ( err ) ERR_EXIT( err );
		surf_formats2 = ( VkSurfaceFormat2KHR * )malloc( format_count * sizeof( VkSurfaceFormat2KHR ) );
		if ( !surf_formats2 ) ERR_EXIT( VK_ERROR_OUT_OF_HOST_MEMORY );
		for ( uint32_t i = 0; i < format_count; ++i )
		{
			surf_formats2[i].sType = VK_STRUCTURE_TYPE_SURFACE_FORMAT_2_KHR;
			surf_formats2[i].pNext = NULL;
		}
		err = inst->vkGetPhysicalDeviceSurfaceFormats2KHR( gpu->obj, &surface_info2, &format_count, surf_formats2 );
		if ( err ) ERR_EXIT( err );
	}
	else
#endif
	{
		surf_formats = inst->surface->getFormats();
	}

	auto format_count = uint32_t( surf_formats.size() );

	if ( html_output )
	{
		fprintf( out, "\t\t\t\t\t<details><summary>Formats: count = <div class='val'>%d</div></summary>", format_count );
		if ( format_count > 0 )
		{
			fprintf( out, "\n" );
		}
		else
		{
			fprintf( out, "</details>\n" );
		}
	}
	else if ( human_readable_output )
	{
		printf( "Formats:\t\tcount = %d\n", format_count );
	}
	for ( uint32_t i = 0; i < format_count; ++i )
	{
		if ( html_output )
		{
#if 0
			if ( CheckExtensionEnabled( VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME, gpu->inst->inst_extensions,
				gpu->inst->inst_extensions_count ) )
			{
				fprintf( out, "\t\t\t\t\t\t<details><summary><div class='type'>%s</div></summary></details>\n",
					VkFormatString( surf_formats2[i].surfaceFormat.format ).c_str() );
			}
			else
#endif
			{
				fprintf( out, "\t\t\t\t\t\t<details><summary><div class='type'>%s</div></summary></details>\n",
					VkFormatString( surf_formats[i].format ).c_str() );
			}
		}
		else if ( human_readable_output )
		{
#if 0
			if ( CheckExtensionEnabled( VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME, gpu->inst->inst_extensions,
				gpu->inst->inst_extensions_count ) )
			{
				printf( "\t%s\n", VkFormatString( surf_formats2[i].surfaceFormat.format ).c_str() );
			}
			else
#endif
			{
				printf( "\t%s\n", VkFormatString( surf_formats[i].format ).c_str() );
			}
		}
	}
	if ( format_count > 0 && html_output )
	{
		fprintf( out, "\t\t\t\t\t</details>\n" );
	}

	fflush( out );
	fflush( stdout );

	return format_count;
}

static int AppDumpSurfacePresentModes( AppInstance * inst
	, AppGpu const * gpu
	, FILE *out )
{
	// Get the list of VkPresentMode's that are supported:
	auto surf_present_modes = inst->surface->getPresentModes();
	uint32_t present_mode_count = uint32_t( surf_present_modes.size() );

	if ( html_output )
	{
		fprintf( out, "\t\t\t\t\t<details><summary>Present Modes: count = <div class='val'>%d</div></summary>", present_mode_count );
		if ( present_mode_count > 0 )
		{
			fprintf( out, "\n" );
		}
		else
		{
			fprintf( out, "</details>" );
		}
	}
	else if ( human_readable_output )
	{
		printf( "Present Modes:\t\tcount = %d\n", present_mode_count );
	}
	for ( uint32_t i = 0; i < present_mode_count; ++i )
	{
		if ( html_output )
		{
			fprintf( out, "\t\t\t\t\t\t<details><summary><div class='type'>%s</div></summary></details>\n",
				VkPresentModeString( surf_present_modes[i] ).c_str() );
		}
		else if ( human_readable_output )
		{
			printf( "\t%s\n", VkPresentModeString( surf_present_modes[i] ).c_str() );
		}
	}
	if ( present_mode_count > 0 && html_output )
	{
		fprintf( out, "\t\t\t\t\t</details>\n" );
	}

	fflush( out );
	fflush( stdout );

	return present_mode_count;
}

static void AppDumpSurfaceCapabilities( AppInstance *inst
	, AppGpu const *gpu
	, FILE *out )
{
#if 0
	if ( CheckExtensionEnabled( VK_KHR_SURFACE_EXTENSION_NAME, gpu->inst->inst_extensions, gpu->inst->inst_extensions_count ) )
	{
#endif
		inst->surface_capabilities = inst->surface->getCapabilities();

		if ( html_output )
		{
			fprintf( out, "\t\t\t\t\t<details><summary>VkSurfaceCapabilitiesKHR</summary>\n" );
			fprintf( out, "\t\t\t\t\t\t<details><summary>minImageCount = <div class='val'>%u</div></summary></details>\n",
				inst->surface_capabilities.minImageCount );
			fprintf( out, "\t\t\t\t\t\t<details><summary>maxImageCount = <div class='val'>%u</div></summary></details>\n",
				inst->surface_capabilities.maxImageCount );
			fprintf( out, "\t\t\t\t\t\t<details><summary>currentExtent</summary>\n" );
			fprintf( out, "\t\t\t\t\t\t\t<details><summary>width = <div class='val'>%u</div></summary></details>\n",
				inst->surface_capabilities.currentExtent.width );
			fprintf( out, "\t\t\t\t\t\t\t<details><summary>height = <div class='val'>%u</div></summary></details>\n",
				inst->surface_capabilities.currentExtent.height );
			fprintf( out, "\t\t\t\t\t\t</details>\n" );
			fprintf( out, "\t\t\t\t\t\t<details><summary>minImageExtent</summary>\n" );
			fprintf( out, "\t\t\t\t\t\t\t<details><summary>width = <div class='val'>%u</div></summary></details>\n",
				inst->surface_capabilities.minImageExtent.width );
			fprintf( out, "\t\t\t\t\t\t\t<details><summary>height = <div class='val'>%u</div></summary></details>\n",
				inst->surface_capabilities.minImageExtent.height );
			fprintf( out, "\t\t\t\t\t\t</details>\n" );
			fprintf( out, "\t\t\t\t\t\t<details><summary>maxImageExtent</summary>\n" );
			fprintf( out, "\t\t\t\t\t\t\t<details><summary>width = <div class='val'>%u</div></summary></details>\n",
				inst->surface_capabilities.maxImageExtent.width );
			fprintf( out, "\t\t\t\t\t\t\t<details><summary>height = <div class='val'>%u</div></summary></details>\n",
				inst->surface_capabilities.maxImageExtent.height );
			fprintf( out, "\t\t\t\t\t\t</details>\n" );
			fprintf( out, "\t\t\t\t\t\t<details><summary>maxImageArrayLayers = <div class='val'>%u</div></summary></details>\n",
				inst->surface_capabilities.maxImageArrayLayers );
			fprintf( out, "\t\t\t\t\t\t<details><summary>supportedTransform</summary>\n" );
			if ( inst->surface_capabilities.supportedTransforms == ashes::SurfaceTransformFlag::eNone )
			{
				fprintf( out, "\t\t\t\t\t\t\t<details><summary>None</summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eIdentity )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eRotate90 )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eRotate180 )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eRotate270 )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eHorizontalMirror )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eHorizontalMirrorRotate90 )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eHorizontalMirrorRotate180 )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eHorizontalMirrorRotate270 )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eInherit )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR</div></summary></details>\n" );
			}
			fprintf( out, "\t\t\t\t\t\t</details>\n" );
			fprintf( out, "\t\t\t\t\t\t<details><summary>currentTransform</summary>\n" );
			if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eNone )
			{
				fprintf( out, "\t\t\t\t\t\t\t<details><summary>None</summary></details>\n" );
			}
			if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eIdentity )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR</div></summary></details>\n" );
			}
			else if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eRotate90 )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR</div></summary></details>\n" );
			}
			else if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eRotate180 )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR</div></summary></details>\n" );
			}
			else if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eRotate270 )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR</div></summary></details>\n" );
			}
			else if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eHorizontalMirror )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR</div></summary></details>\n" );
			}
			else if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eHorizontalMirrorRotate90 )
			{
				fprintf( out,
					"\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR</div></summary></details>\n" );
			}
			else if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eHorizontalMirrorRotate180 )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR</div></summary></details>\n" );
			}
			else if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eHorizontalMirrorRotate270 )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR</div></summary></details>\n" );
			}
			else if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eInherit )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR</div></summary></details>\n" );
			}
			fprintf( out, "\t\t\t\t\t\t</details>\n" );
			fprintf( out, "\t\t\t\t\t\t<details><summary>supportedCompositeAlpha</summary>\n" );
			if ( inst->surface_capabilities.supportedCompositeAlpha == ashes::CompositeAlphaFlag::eNone )
			{
				fprintf( out, "\t\t\t\t\t\t\t<details><summary>None</summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedCompositeAlpha & ashes::CompositeAlphaFlag::eOpaque )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedCompositeAlpha & ashes::CompositeAlphaFlag::ePreMultiplied )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedCompositeAlpha & ashes::CompositeAlphaFlag::ePostMultiplied )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedCompositeAlpha & ashes::CompositeAlphaFlag::eInherit )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR</div></summary></details>\n" );
			}
			fprintf( out, "\t\t\t\t\t\t</details>\n" );
			fprintf( out, "\t\t\t\t\t\t<details><summary>supportedUsageFlags</summary>\n" );
			if ( inst->surface_capabilities.supportedUsageFlags == ashes::ImageUsageFlag::eNone )
			{
				fprintf( out, "\t\t\t\t\t\t\t<details><summary>None</summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedUsageFlags & ashes::ImageUsageFlag::eTransferSrc )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_IMAGE_USAGE_TRANSFER_SRC_BIT</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedUsageFlags & ashes::ImageUsageFlag::eTransferDst )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_IMAGE_USAGE_TRANSFER_DST_BIT</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedUsageFlags & ashes::ImageUsageFlag::eSampled )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div class='type'>VK_IMAGE_USAGE_SAMPLED_BIT</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedUsageFlags & ashes::ImageUsageFlag::eStorage )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div class='type'>VK_IMAGE_USAGE_STORAGE_BIT</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedUsageFlags & ashes::ImageUsageFlag::eColourAttachment )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedUsageFlags & ashes::ImageUsageFlag::eDepthStencilAttachment )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedUsageFlags & ashes::ImageUsageFlag::eTransientAttachment )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT</div></summary></details>\n" );
			}
			if ( inst->surface_capabilities.supportedUsageFlags & ashes::ImageUsageFlag::eInputAttachment )
			{
				fprintf( out,
					"\t\t\t\t\t\t\t<details><summary><div "
					"class='type'>VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT</div></summary></details>\n" );
			}
			fprintf( out, "\t\t\t\t\t\t</details>\n" );
		}
		else if ( human_readable_output )
		{
			printf( "VkSurfaceCapabilitiesKHR:\n" );
			printf( "\tminImageCount       = %u\n", inst->surface_capabilities.minImageCount );
			printf( "\tmaxImageCount       = %u\n", inst->surface_capabilities.maxImageCount );
			printf( "\tcurrentExtent:\n" );
			printf( "\t\twidth       = %u\n", inst->surface_capabilities.currentExtent.width );
			printf( "\t\theight      = %u\n", inst->surface_capabilities.currentExtent.height );
			printf( "\tminImageExtent:\n" );
			printf( "\t\twidth       = %u\n", inst->surface_capabilities.minImageExtent.width );
			printf( "\t\theight      = %u\n", inst->surface_capabilities.minImageExtent.height );
			printf( "\tmaxImageExtent:\n" );
			printf( "\t\twidth       = %u\n", inst->surface_capabilities.maxImageExtent.width );
			printf( "\t\theight      = %u\n", inst->surface_capabilities.maxImageExtent.height );
			printf( "\tmaxImageArrayLayers = %u\n", inst->surface_capabilities.maxImageArrayLayers );
			printf( "\tsupportedTransform:\n" );
			if ( inst->surface_capabilities.supportedTransforms == ashes::SurfaceTransformFlag::eNone )
			{
				printf( "\t\tNone\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eIdentity )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eRotate90 )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eRotate180 )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eRotate270 )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eHorizontalMirror )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eHorizontalMirrorRotate90 )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eHorizontalMirrorRotate180 )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eHorizontalMirrorRotate270 )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR\n" );
			}
			if ( inst->surface_capabilities.supportedTransforms & ashes::SurfaceTransformFlag::eInherit )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_INHERIT_BIT_KHR\n" );
			}
			printf( "\tcurrentTransform:\n" );
			if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eNone )
			{
				printf( "\t\tNone\n" );
			}
			if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eIdentity )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR\n" );
			}
			else if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eRotate90 )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR\n" );
			}
			else if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eRotate180 )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR\n" );
			}
			else if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eRotate270 )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR\n" );
			}
			else if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eHorizontalMirror )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR\n" );
			}
			else if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eHorizontalMirrorRotate90 )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR\n" );
			}
			else if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eHorizontalMirrorRotate180 )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR\n" );
			}
			else if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eHorizontalMirrorRotate270 )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR\n" );
			}
			else if ( inst->surface_capabilities.currentTransform == ashes::SurfaceTransformFlag::eInherit )
			{
				printf( "\t\tVK_SURFACE_TRANSFORM_INHERIT_BIT_KHR\n" );
			}
			printf( "\tsupportedCompositeAlpha:\n" );
			if ( inst->surface_capabilities.supportedCompositeAlpha == 0 )
			{
				printf( "\t\tNone\n" );
			}
			if ( inst->surface_capabilities.supportedCompositeAlpha & ashes::CompositeAlphaFlag::eOpaque )
			{
				printf( "\t\tVK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR\n" );
			}
			if ( inst->surface_capabilities.supportedCompositeAlpha & ashes::CompositeAlphaFlag::ePreMultiplied )
			{
				printf( "\t\tVK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR\n" );
			}
			if ( inst->surface_capabilities.supportedCompositeAlpha & ashes::CompositeAlphaFlag::ePostMultiplied )
			{
				printf( "\t\tVK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR\n" );
			}
			if ( inst->surface_capabilities.supportedCompositeAlpha & ashes::CompositeAlphaFlag::eInherit )
			{
				printf( "\t\tVK_COMPOSITE_ALPHA_INHERIT_BIT_KHR\n" );
			}
			printf( "\tsupportedUsageFlags:\n" );
			if ( inst->surface_capabilities.supportedUsageFlags == 0 )
			{
				printf( "\t\tNone\n" );
			}
			if ( inst->surface_capabilities.supportedUsageFlags & ashes::ImageUsageFlag::eTransferSrc )
			{
				printf( "\t\tVK_IMAGE_USAGE_TRANSFER_SRC_BIT\n" );
			}
			if ( inst->surface_capabilities.supportedUsageFlags & ashes::ImageUsageFlag::eTransferDst )
			{
				printf( "\t\tVK_IMAGE_USAGE_TRANSFER_DST_BIT\n" );
			}
			if ( inst->surface_capabilities.supportedUsageFlags & ashes::ImageUsageFlag::eSampled )
			{
				printf( "\t\tVK_IMAGE_USAGE_SAMPLED_BIT\n" );
			}
			if ( inst->surface_capabilities.supportedUsageFlags & ashes::ImageUsageFlag::eStorage )
			{
				printf( "\t\tVK_IMAGE_USAGE_STORAGE_BIT\n" );
			}
			if ( inst->surface_capabilities.supportedUsageFlags & ashes::ImageUsageFlag::eColourAttachment )
			{
				printf( "\t\tVK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT\n" );
			}
			if ( inst->surface_capabilities.supportedUsageFlags & ashes::ImageUsageFlag::eDepthStencilAttachment )
			{
				printf( "\t\tVK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT\n" );
			}
			if ( inst->surface_capabilities.supportedUsageFlags & ashes::ImageUsageFlag::eTransientAttachment )
			{
				printf( "\t\tVK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT\n" );
			}
			if ( inst->surface_capabilities.supportedUsageFlags & ashes::ImageUsageFlag::eInputAttachment )
			{
				printf( "\t\tVK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT\n" );
			}
		}
#if 0
		// Get additional surface capability information from vkGetPhysicalDeviceSurfaceCapabilities2EXT
		if ( CheckExtensionEnabled( VK_EXT_DISPLAY_SURFACE_COUNTER_EXTENSION_NAME, gpu->inst->inst_extensions,
			gpu->inst->inst_extensions_count ) )
		{
			memset( &inst->surface_capabilities2_ext, 0, sizeof( VkSurfaceCapabilities2EXT ) );
			inst->surface_capabilities2_ext.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_EXT;
			inst->surface_capabilities2_ext.pNext = NULL;

			err = inst->vkGetPhysicalDeviceSurfaceCapabilities2EXT( gpu->obj, inst->surface, &inst->surface_capabilities2_ext );
			if ( err ) ERR_EXIT( err );

			if ( html_output )
			{
				fprintf( out, "\t\t\t\t\t\t<details><summary>VkSurfaceCapabilities2EXT</summary>\n" );
				fprintf( out, "\t\t\t\t\t\t\t<details><summary>supportedSurfaceCounters</summary>\n" );
				if ( inst->surface_capabilities2_ext.supportedSurfaceCounters == 0 )
				{
					fprintf( out, "\t\t\t\t\t\t\t\t<details><summary>None</summary></details>\n" );
				}
				if ( inst->surface_capabilities2_ext.supportedSurfaceCounters & VK_SURFACE_COUNTER_VBLANK_EXT )
				{
					fprintf( out,
						"\t\t\t\t\t\t\t\t<details><summary><div "
						"class='type'>VK_SURFACE_COUNTER_VBLANK_EXT</div></summary></details>\n" );
				}
				fprintf( out, "\t\t\t\t\t\t\t</details>\n" );
				fprintf( out, "\t\t\t\t\t\t</details>\n" );
			}
			else if ( human_readable_output )
			{
				printf( "VkSurfaceCapabilities2EXT:\n" );
				printf( "\tsupportedSurfaceCounters:\n" );
				if ( inst->surface_capabilities2_ext.supportedSurfaceCounters == 0 )
				{
					printf( "\t\tNone\n" );
				}
				if ( inst->surface_capabilities2_ext.supportedSurfaceCounters & VK_SURFACE_COUNTER_VBLANK_EXT )
				{
					printf( "\t\tVK_SURFACE_COUNTER_VBLANK_EXT\n" );
				}
			}
		}
#endif
#if 0
		// Get additional surface capability information from vkGetPhysicalDeviceSurfaceCapabilities2KHR
		if ( CheckExtensionEnabled( VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME, gpu->inst->inst_extensions,
			gpu->inst->inst_extensions_count ) )
		{
			if ( CheckExtensionEnabled( VK_KHR_SHARED_PRESENTABLE_IMAGE_EXTENSION_NAME, gpu->inst->inst_extensions,
				gpu->inst->inst_extensions_count ) )
			{
				inst->shared_surface_capabilities.sType = VK_STRUCTURE_TYPE_SHARED_PRESENT_SURFACE_CAPABILITIES_KHR;
				inst->shared_surface_capabilities.pNext = NULL;
				inst->surface_capabilities2.pNext = &inst->shared_surface_capabilities;
			}
			else
			{
				inst->surface_capabilities2.pNext = NULL;
			}

			inst->surface_capabilities2.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;

			VkPhysicalDeviceSurfaceInfo2KHR surface_info;
			surface_info.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
			surface_info.pNext = NULL;
			surface_info.surface = inst->surface;

			err = inst->vkGetPhysicalDeviceSurfaceCapabilities2KHR( gpu->obj, &surface_info, &inst->surface_capabilities2 );
			if ( err ) ERR_EXIT( err );

			void *place = inst->surface_capabilities2.pNext;
			while ( place )
			{
				struct VkStructureHeader *work = ( struct VkStructureHeader * )place;
				if ( work->sType == VK_STRUCTURE_TYPE_SHARED_PRESENT_SURFACE_CAPABILITIES_KHR )
				{
					VkSharedPresentSurfaceCapabilitiesKHR *shared_surface_capabilities =
						( VkSharedPresentSurfaceCapabilitiesKHR * )place;
					if ( html_output )
					{
						fprintf( out, "\t\t\t\t\t\t<details><summary>VkSharedPresentSurfaceCapabilitiesKHR</summary>\n" );
						fprintf( out, "\t\t\t\t\t\t\t<details><summary>sharedPresentSupportedUsageFlags</summary>\n" );
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags == 0 )
						{
							fprintf( out, "\t\t\t\t\t\t\t\t<details><summary>None</summary></details>\n" );
						}
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT )
						{
							fprintf( out,
								"\t\t\t\t\t\t\t\t<details><summary><div "
								"class='type'>VK_IMAGE_USAGE_TRANSFER_SRC_BIT</div></summary></details>\n" );
						}
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT )
						{
							fprintf( out,
								"\t\t\t\t\t\t\t\t<details><summary><div "
								"class='type'>VK_IMAGE_USAGE_TRANSFER_DST_BIT</div></summary></details>\n" );
						}
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags & VK_IMAGE_USAGE_SAMPLED_BIT )
						{
							fprintf( out,
								"\t\t\t\t\t\t\t\t<details><summary><div "
								"class='type'>VK_IMAGE_USAGE_SAMPLED_BIT</div></summary></details>\n" );
						}
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags & VK_IMAGE_USAGE_STORAGE_BIT )
						{
							fprintf( out,
								"\t\t\t\t\t\t\t\t<details><summary><div "
								"class='type'>VK_IMAGE_USAGE_STORAGE_BIT</div></summary></details>\n" );
						}
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT )
						{
							fprintf( out,
								"\t\t\t\t\t\t\t\t<details><summary><div "
								"class='type'>VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT</div></summary></details>\n" );
						}
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags &
							VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT )
						{
							fprintf( out,
								"\t\t\t\t\t\t\t\t<details><summary><div "
								"class='type'>VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT</div></summary></details>\n" );
						}
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags &
							VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT )
						{
							fprintf( out,
								"\t\t\t\t\t\t\t\t<details><summary><div "
								"class='type'>VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT</div></summary></details>\n" );
						}
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT )
						{
							fprintf( out,
								"\t\t\t\t\t\t\t\t<details><summary><div "
								"class='type'>VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT</div></summary></details>\n" );
						}
						fprintf( out, "\t\t\t\t\t\t\t</details>\n" );
						fprintf( out, "\t\t\t\t\t\t</details>\n" );
					}
					else if ( human_readable_output )
					{
						printf( "VkSharedPresentSurfaceCapabilitiesKHR:\n" );
						printf( "\tsharedPresentSupportedUsageFlags:\n" );
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags == 0 )
						{
							printf( "\t\tNone\n" );
						}
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT )
						{
							printf( "\t\tVK_IMAGE_USAGE_TRANSFER_SRC_BIT\n" );
						}
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT )
						{
							printf( "\t\tVK_IMAGE_USAGE_TRANSFER_DST_BIT\n" );
						}
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags & VK_IMAGE_USAGE_SAMPLED_BIT )
						{
							printf( "\t\tVK_IMAGE_USAGE_SAMPLED_BIT\n" );
						}
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags & VK_IMAGE_USAGE_STORAGE_BIT )
						{
							printf( "\t\tVK_IMAGE_USAGE_STORAGE_BIT\n" );
						}
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT )
						{
							printf( "\t\tVK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT\n" );
						}
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags &
							VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT )
						{
							printf( "\t\tVK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT\n" );
						}
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags &
							VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT )
						{
							printf( "\t\tVK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT\n" );
						}
						if ( shared_surface_capabilities->sharedPresentSupportedUsageFlags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT )
						{
							printf( "\t\tVK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT\n" );
						}
					}
				}
				place = work->pNext;
			}
		}
#endif
		if ( html_output )
		{
			fprintf( out, "\t\t\t\t\t</details>\n" );
		}
#if 0
	}
#endif
}

struct SurfaceExtensionInfo
{
	const char *name;
	void( *create_window )( struct AppInstance * );
	void( *create_surface )( struct AppInstance *, ashes::PhysicalDevice const & );
	void( *destroy_window )( struct AppInstance * );
};

static void AppDumpSurfaceExtension( AppInstance * inst
	, std::vector< AppGpu > const & gpus
	, SurfaceExtensionInfo * surface_extension
	, int * format_count
	, int * present_mode_count
	, FILE * out )
{
	if ( !CheckExtensionEnabled( surface_extension->name, inst->inst_extensions ) )
	{
		return;
	}

	auto gpu_count = uint32_t( gpus.size() );
	surface_extension->create_window( inst );
	for ( uint32_t i = 0; i < gpu_count; ++i )
	{
		surface_extension->create_surface( inst, *gpus[i].obj );
		if ( html_output )
		{
			fprintf( out, "\t\t\t\t<details><summary>GPU id : <div class='val'>%u</div> (%s)</summary>\n", i,
				gpus[i].props.deviceName.c_str() );
			fprintf( out, "\t\t\t\t\t<details><summary>Surface type : <div class='type'>%s</div></summary></details>\n",
				surface_extension->name );
		}
		else if ( human_readable_output )
		{
			printf( "GPU id       : %u (%s)\n", i, gpus[i].props.deviceName.c_str() );
			printf( "Surface type : %s\n", surface_extension->name );
		}
		*format_count += AppDumpSurfaceFormats( inst, &gpus[i], out );
		*present_mode_count += AppDumpSurfacePresentModes( inst, &gpus[i], out );
		AppDumpSurfaceCapabilities( inst, &gpus[i], out );
		AppDestroySurface( inst );
		if ( html_output )
		{
			fprintf( out, "\t\t\t\t</details>\n" );
		}
		else if ( human_readable_output )
		{
			printf( "\n" );
		}
	}
	surface_extension->destroy_window( inst );
}

#endif

static void AppDevDumpFormatProps( const AppGpu * gpu
	, ashes::Format fmt
	, bool * first_in_list
	, FILE *out )
{
	auto props = gpu->obj->getFormatProperties( fmt );
	struct
	{
		const char *name;
		uint32_t flags;
	} features[3];

	features[0].name = "linearTiling   FormatFeatureFlags";
	features[0].flags = props.linearTilingFeatures;
	features[1].name = "optimalTiling  FormatFeatureFlags";
	features[1].flags = props.optimalTilingFeatures;
	features[2].name = "bufferFeatures FormatFeatureFlags";
	features[2].flags = props.bufferFeatures;

	if ( html_output )
	{
		fprintf( out, "\t\t\t\t\t\t<details><summary><div class='type'>FORMAT_%s</div></summary>\n", VkFormatString( fmt ).c_str() );
	}
	else if ( human_readable_output )
	{
		printf( "\nFORMAT_%s:", VkFormatString( fmt ).c_str() );
	}
	for ( uint32_t i = 0; i < ARRAY_SIZE( features ); ++i )
	{
		if ( html_output )
		{
			fprintf( out, "\t\t\t\t\t\t\t<details open><summary>%s</summary>\n", features[i].name );
			if ( features[i].flags == 0 )
			{
				fprintf( out, "\t\t\t\t\t\t\t\t<details><summary>None</summary></details>\n" );
			}
			else
			{
				fprintf( out, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
					( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eSampledImage ) ? "\t\t\t\t\t\t\t\t<details><summary><div "
						"class='type'>VK_FORMAT_FEATURE_SAMPLED_IMAGE_"
						"BIT</div></summary></details>\n"
						: "" ),  // 0x0001
						( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eStorageImage ) ? "\t\t\t\t\t\t\t\t<details><summary><div "
							"class='type'>VK_FORMAT_FEATURE_STORAGE_IMAGE_"
							"BIT</div></summary></details>\n"
							: "" ),  // 0x0002
							( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eStorageImageAtomic )
								? "\t\t\t\t\t\t\t\t<details><summary><div "
								"class='type'>VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT</div></summary></details>\n"
								: "" ),  // 0x0004
								( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eUniformTexelBuffer )
									? "\t\t\t\t\t\t\t\t<details><summary><div "
									"class='type'>VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT</div></summary></details>\n"
									: "" ),  // 0x0008
									( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eStorageTexelBuffer )
										? "\t\t\t\t\t\t\t\t<details><summary><div "
										"class='type'>VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT</div></summary></details>\n"
										: "" ),  // 0x0010
										( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eStorageTexelBufferAtomic )
											? "\t\t\t\t\t\t\t\t<details><summary><div "
											"class='type'>VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT</div></summary></details>\n"
											: "" ),  // 0x0020
											( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eVertexBuffer ) ? "\t\t\t\t\t\t\t\t<details><summary><div "
												"class='type'>VK_FORMAT_FEATURE_VERTEX_BUFFER_"
												"BIT</div></summary></details>\n"
												: "" ),  // 0x0040
												( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eColourAttachment ) ? "\t\t\t\t\t\t\t\t<details><summary><div "
													"class='type'>VK_FORMAT_FEATURE_COLOR_"
													"ATTACHMENT_BIT</div></summary></details>\n"
													: "" ),  // 0x0080
													( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eColourAttachmentBlend )
														? "\t\t\t\t\t\t\t\t<details><summary><div "
														"class='type'>VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT</div></summary></details>\n"
														: "" ),  // 0x0100
														( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eDepthStencilAttachment )
															? "\t\t\t\t\t\t\t\t<details><summary><div "
															"class='type'>VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT</div></summary></details>\n"
															: "" ),  // 0x0200
															( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eBlitSrc ) ? "\t\t\t\t\t\t\t\t<details><summary><div "
																"class='type'>VK_FORMAT_FEATURE_BLIT_SRC_BIT</"
																"div></summary></details>\n"
																: "" ),  // 0x0400
																( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eBlitDst ) ? "\t\t\t\t\t\t\t\t<details><summary><div "
																	"class='type'>VK_FORMAT_FEATURE_BLIT_DST_BIT</"
																	"div></summary></details>\n"
																	: "" ),  // 0x0800
																	( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eSampledImageFilterLinear )
																		? "\t\t\t\t\t\t\t\t<details><summary><div "
																		"class='type'>VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT</div></summary></details>\n"
																		: "" ),  // 0x1000
																		( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eSampledImageFilterCubic )
																			? "\t\t\t\t\t\t\t\t<details><summary><div "
																			"class='type'>VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG</div></summary></details>\n"
																			: "" ),  // 0x2000
																			( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eTransferSrc ) ? "\t\t\t\t\t\t\t\t<details><summary><div "
																				"class='type'>VK_FORMAT_FEATURE_TRANSFER_"
																				"SRC_BIT_KHR</div></summary></details>\n"
																				: "" ),  // 0x4000
																				( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eTransferDst ) ? "\t\t\t\t\t\t\t\t<details><summary><div "
																					"class='type'>VK_FORMAT_FEATURE_TRANSFER_"
																					"DST_BIT_KHR</div></summary></details>\n"
																					: "" ) );  // 0x8000
			}
			fprintf( out, "\t\t\t\t\t\t\t</details>\n" );
		}
		else if ( human_readable_output )
		{
			printf( "\n\t%s:", features[i].name );
			if ( features[i].flags == 0 )
			{
				printf( "\n\t\tNone" );
			}
			else
			{
				printf(
					"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
					( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eSampledImage ) ? "\n\t\tVK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT"
						: "" ),  // 0x0001
						( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eStorageImage ) ? "\n\t\tVK_FORMAT_FEATURE_STORAGE_IMAGE_BIT"
							: "" ),  // 0x0002
							( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eStorageImageAtomic )
								? "\n\t\tVK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT"
								: "" ),  // 0x0004
								( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eUniformTexelBuffer )
									? "\n\t\tVK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT"
									: "" ),  // 0x0008
									( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eStorageTexelBuffer )
										? "\n\t\tVK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT"
										: "" ),  // 0x0010
										( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eStorageTexelBufferAtomic )
											? "\n\t\tVK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT"
											: "" ),  // 0x0020
											( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eVertexBuffer ) ? "\n\t\tVK_FORMAT_FEATURE_VERTEX_BUFFER_BIT"
												: "" ),  // 0x0040
												( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eColourAttachment ) ? "\n\t\tVK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT"
													: "" ),  // 0x0080
													( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eColourAttachmentBlend )
														? "\n\t\tVK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT"
														: "" ),  // 0x0100
														( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eDepthStencilAttachment )
															? "\n\t\tVK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT"
															: "" ),                                                                                            // 0x0200
															( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eBlitSrc ) ? "\n\t\tVK_FORMAT_FEATURE_BLIT_SRC_BIT" : "" ),  // 0x0400
					( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eBlitDst ) ? "\n\t\tVK_FORMAT_FEATURE_BLIT_DST_BIT" : "" ),  // 0x0800
					( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eSampledImageFilterLinear )
						? "\n\t\tVK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT"
						: "" ),  // 0x1000
						( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eSampledImageFilterCubic )
							? "\n\t\tVK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG"
							: "" ),  // 0x2000
							( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eTransferSrc ) ? "\n\t\tVK_FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR"
								: "" ),  // 0x4000
								( checkFlag( features[i].flags, ashes::FormatFeatureFlag::eTransferDst ) ? "\n\t\tVK_FORMAT_FEATURE_TRANSFER_DST_BIT_KHR"
									: "" ) );  // 0x8000
			}
		}
	}
	if ( html_output )
	{
		fprintf( out, "\t\t\t\t\t\t</details>\n" );
	}
	else if ( human_readable_output )
	{
		printf( "\n" );
	}
	if ( json_output && ( props.linearTilingFeatures || props.optimalTilingFeatures || props.bufferFeatures ) )
	{
		if ( !( *first_in_list ) )
		{
			printf( "," );
		}
		else
		{
			*first_in_list = false;
		}
		printf( "\n" );
		printf( "\t\t{\n" );
		printf( "\t\t\t\"formatID\": %d,\n", int( fmt ) );
		printf( "\t\t\t\"linearTilingFeatures\": %u,\n", props.linearTilingFeatures.value() );
		printf( "\t\t\t\"optimalTilingFeatures\": %u,\n", props.optimalTilingFeatures.value() );
		printf( "\t\t\t\"bufferFeatures\": %u\n", props.bufferFeatures.value() );
		printf( "\t\t}" );
	}
}

/* This structure encodes all the format ranges to be queried.
 * It ensures that a format is not queried if the instance
 * doesn't support it (either through the instance version or
 * through extensions).
 */
static struct FormatRange
{
// the Vulkan standard version that supports this format range, or 0 if non-standard
	uint32_t minimum_instance_version;

	// The name of the extension that supports this format range, or NULL if the range
	// is only part of the standard
	std::string const extension_name;

	// The first and last supported formats within this range.
	ashes::Format first_format;
	ashes::Format last_format;
} supported_format_ranges[] = {
	{
		// Standard formats in Vulkan 1.0
		ashes::makeVersion( 1, 0, 0 ),
		std::string{},
		ashes::Format::eRange_BEGIN,
		ashes::Format::eRange_END,
	},
	{
		// YCBCR extension, standard in Vulkan 1.1
		ashes::makeVersion( 1, 1, 0 ),
		ashes::KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME,
		ashes::Format::eG8B8G8R8_422_UNORM,
		ashes::Format::eG16_B16_R16_3PLANE_444_UNORM,
	},
	{
		// PVRTC extension, not standardized
		0,
		ashes::IMG_FORMAT_PVRTC_EXTENSION_NAME,
		ashes::Format::ePVRTC1_2BPP_UNORM_BLOCK_IMG,
		ashes::Format::ePVRTC2_4BPP_SRGB_BLOCK_IMG,
	},
};

// Helper function to determine whether a format range is currently supported.
bool FormatRangeSupported( const struct FormatRange *format_range, const struct AppGpu *gpu )
{
// True if standard and supported by both this instance and this GPU
	if ( format_range->minimum_instance_version > 0
		&& gpu->inst->instance_version >= format_range->minimum_instance_version
		&& gpu->props.apiVersion >= format_range->minimum_instance_version )
	{
		return true;
	}

	// True if this extension is present
	if ( !format_range->extension_name.empty() )
	{
		return CheckExtensionEnabled( format_range->extension_name.c_str()
			, gpu->inst->inst_extensions );
	}

	// Otherwise, not supported.
	return false;
}

bool FormatPropsEq( const ashes::FormatProperties *props1
	, const ashes::FormatProperties *props2 )
{
	if ( props1->bufferFeatures == props2->bufferFeatures
		&& props1->linearTilingFeatures == props2->linearTilingFeatures
		&& props1->optimalTilingFeatures == props2->optimalTilingFeatures )
	{
		return true;
	}
	else
	{
		return false;
	}
}

struct PropFormats
{
	ashes::FormatProperties props;

	uint32_t format_count;
	uint32_t format_reserve;
	ashes::Format *formats;
};

void FormatPropsShortenedDump( const AppGpu *gpu )
{
	const ashes::FormatProperties unsupported_prop = {0};
	uint32_t unique_props_count = 1;
	uint32_t unique_props_reserve = 50;
	PropFormats *prop_map = (PropFormats*)malloc(sizeof(PropFormats) * unique_props_reserve);
	prop_map[0].props = unsupported_prop;
	prop_map[0].format_count = 0;
	prop_map[0].format_reserve = 20;
	prop_map[0].formats = (ashes::Format*)malloc(sizeof(ashes::Format) * prop_map[0].format_reserve);

	for (uint32_t ri = 0; ri < ARRAY_SIZE(supported_format_ranges); ++ri) {
		struct FormatRange format_range = supported_format_ranges[ri];
		if (FormatRangeSupported(&format_range, gpu)) {
			for (ashes::Format fmt = format_range.first_format; fmt <= format_range.last_format; fmt=ashes::Format(uint32_t(fmt)+1)) {
				ashes::FormatProperties props = gpu->obj->getFormatProperties(ashes::Format(fmt));

				uint32_t formats_prop_i = 0;
				for (; formats_prop_i < unique_props_count; ++formats_prop_i) {
					if (FormatPropsEq(&prop_map[formats_prop_i].props, &props)) break;
				}

				if (formats_prop_i < unique_props_count) {
					struct PropFormats *propFormats = &prop_map[formats_prop_i];
					++propFormats->format_count;

					if (propFormats->format_count > propFormats->format_reserve) {
						propFormats->format_reserve *= 2;
						propFormats->formats = ( ashes::Format* )realloc(propFormats->formats, sizeof(ashes::Format) * propFormats->format_reserve);
					}

					propFormats->formats[propFormats->format_count - 1] = fmt;
				} else {
					assert(formats_prop_i == unique_props_count);
					++unique_props_count;

					if (unique_props_count > unique_props_reserve) {
						unique_props_reserve *= 2;
						prop_map = (PropFormats*)realloc(prop_map, sizeof(PropFormats) * unique_props_reserve);
					}

					struct PropFormats *propFormats = &prop_map[formats_prop_i];
					propFormats->props = props;
					propFormats->format_count = 1;
					propFormats->format_reserve = 20;
					propFormats->formats = (ashes::Format*)malloc(sizeof(ashes::Format) * propFormats->format_reserve);
					propFormats->formats[0] = fmt;
				}
			}
		}
	}

	for (uint32_t pi = 1; pi < unique_props_count; ++pi) {
		struct PropFormats *propFormats = &prop_map[pi];

		for (uint32_t fi = 0; fi < propFormats->format_count; ++fi) {
			const ashes::Format fmt = propFormats->formats[fi];

			printf("\nFORMAT_%s", VkFormatString(fmt).c_str());

			if (fi < propFormats->format_count - 1)
				printf(",");
			else
				printf(":");
		}

		struct {
			const char *name;
			uint32_t flags;
		} features[3];

		features[0].name = "linearTiling   FormatFeatureFlags";
		features[0].flags = propFormats->props.linearTilingFeatures;
		features[1].name = "optimalTiling  FormatFeatureFlags";
		features[1].flags = propFormats->props.optimalTilingFeatures;
		features[2].name = "bufferFeatures FormatFeatureFlags";
		features[2].flags = propFormats->props.bufferFeatures;

		for (uint32_t i = 0; i < ARRAY_SIZE(features); ++i) {
			printf("\n\t%s:", features[i].name);
			if (features[i].flags == 0) {
				printf("\n\t\tNone");
			} else {
				printf(
					"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
					(checkFlag(features[i].flags, ashes::FormatFeatureFlag::eSampledImage) ? "\n\t\tVK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT"
																				: ""),  // 0x0001
					(checkFlag(features[i].flags, ashes::FormatFeatureFlag::eStorageImage) ? "\n\t\tVK_FORMAT_FEATURE_STORAGE_IMAGE_BIT"
																				: ""),  // 0x0002
					(checkFlag(features[i].flags, ashes::FormatFeatureFlag::eStorageImageAtomic)
							? "\n\t\tVK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT"
							: ""),  // 0x0004
					(checkFlag(features[i].flags, ashes::FormatFeatureFlag::eUniformTexelBuffer)
							? "\n\t\tVK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT"
							: ""),  // 0x0008
					(checkFlag(features[i].flags, ashes::FormatFeatureFlag::eStorageTexelBuffer)
							? "\n\t\tVK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT"
							: ""),  // 0x0010
					(checkFlag(features[i].flags, ashes::FormatFeatureFlag::eStorageTexelBufferAtomic)
							? "\n\t\tVK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT"
							: ""),  // 0x0020
					(checkFlag(features[i].flags, ashes::FormatFeatureFlag::eVertexBuffer) ? "\n\t\tVK_FORMAT_FEATURE_VERTEX_BUFFER_BIT"
																				: ""),  // 0x0040
					(checkFlag(features[i].flags, ashes::FormatFeatureFlag::eColourAttachment) ? "\n\t\tVK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT"
																					: ""),  // 0x0080
					(checkFlag(features[i].flags, ashes::FormatFeatureFlag::eColourAttachmentBlend)
							? "\n\t\tVK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT"
							: ""),  // 0x0100
					(checkFlag(features[i].flags, ashes::FormatFeatureFlag::eDepthStencilAttachment)
							? "\n\t\tVK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT"
							: ""),                                                                                            // 0x0200
					(checkFlag(features[i].flags, ashes::FormatFeatureFlag::eBlitSrc) ? "\n\t\tVK_FORMAT_FEATURE_BLIT_SRC_BIT" : ""),  // 0x0400
					(checkFlag(features[i].flags, ashes::FormatFeatureFlag::eBlitDst) ? "\n\t\tVK_FORMAT_FEATURE_BLIT_DST_BIT" : ""),  // 0x0800
					(checkFlag(features[i].flags, ashes::FormatFeatureFlag::eSampledImageFilterLinear)
							? "\n\t\tVK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT"
							: ""),  // 0x1000
					(checkFlag(features[i].flags, ashes::FormatFeatureFlag::eSampledImageFilterCubic)
							? "\n\t\tVK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG"
							: ""),  // 0x2000
					(checkFlag(features[i].flags, ashes::FormatFeatureFlag::eTransferSrc) ? "\n\t\tVK_FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR"
																					: ""),  // 0x4000
					(checkFlag(features[i].flags, ashes::FormatFeatureFlag::eTransferDst) ? "\n\t\tVK_FORMAT_FEATURE_TRANSFER_DST_BIT_KHR"
																					: ""));  // 0x8000
			}

			printf("\n");
		}
	}

	printf("\nUnsupported formats:");
	if (prop_map[0].format_count == 0) printf("\nNone");
	for (uint32_t fi = 0; fi < prop_map[0].format_count; ++fi) {
		const ashes::Format fmt = prop_map[0].formats[fi];

		printf("\nFORMAT_%s", VkFormatString(fmt).c_str());
	}

	// cleanup
	for (uint32_t pi = 0; pi < unique_props_count; ++pi) free(prop_map[pi].formats);
	free(prop_map);
}

static void AppDevDump( const AppGpu * gpu
	, FILE *out )
{
	if ( html_output )
	{
		fprintf( out, "\t\t\t\t\t<details><summary>Format Properties</summary>\n" );
	}
	else if ( human_readable_output )
	{
		printf( "Format Properties:\n" );
		printf( "==================\n" );
	}
	if ( json_output )
	{
		printf( ",\n" );
		printf( "\t\"ArrayOfVkFormatProperties\": [" );
	}

	if ( human_readable_output )
	{
		FormatPropsShortenedDump( gpu );
	}
	else
	{
		bool first_in_list = true;  // Used for commas in json output
		for ( uint32_t i = 0; i < ARRAY_SIZE( supported_format_ranges ); ++i )
		{
			struct FormatRange format_range = supported_format_ranges[i];
			if ( FormatRangeSupported( &format_range, gpu ) )
			{
				for ( uint32_t fmt = uint32_t( format_range.first_format );
					fmt <= uint32_t( format_range.last_format );
					++fmt )
				{
					AppDevDumpFormatProps( gpu, ashes::Format( fmt ), &first_in_list, out );
				}
			}
		}
	}

	if ( html_output )
	{
		fprintf( out, "\t\t\t\t\t</details>\n" );
	}
	if ( json_output )
	{
		printf( "\n\t]" );
	}
}

#ifdef _WIN32
#define PRINTF_SIZE_T_SPECIFIER    "%Iu"
#else
#define PRINTF_SIZE_T_SPECIFIER    "%zu"
#endif

static void AppGpuDumpFeatures( ashes::PhysicalDeviceFeatures const & features, FILE *out )
{
	if ( html_output )
	{
		fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDeviceFeatures</summary>\n" );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>robustBufferAccess                      = <div "
			"class='val'>%u</div></summary></details>\n",
			features.robustBufferAccess );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>fullDrawIndexUint32                     = <div "
			"class='val'>%u</div></summary></details>\n",
			features.fullDrawIndexUint32 );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>imageCubeArray                          = <div "
			"class='val'>%u</div></summary></details>\n",
			features.imageCubeArray );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>independentBlend                        = <div "
			"class='val'>%u</div></summary></details>\n",
			features.independentBlend );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>geometryShader                          = <div "
			"class='val'>%u</div></summary></details>\n",
			features.geometryShader );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>tessellationShader                      = <div "
			"class='val'>%u</div></summary></details>\n",
			features.tessellationShader );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>sampleRateShading                       = <div "
			"class='val'>%u</div></summary></details>\n",
			features.sampleRateShading );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>dualSrcBlend                            = <div "
			"class='val'>%u</div></summary></details>\n",
			features.dualSrcBlend );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>logicOp                                 = <div "
			"class='val'>%u</div></summary></details>\n",
			features.logicOp );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>multiDrawIndirect                       = <div "
			"class='val'>%u</div></summary></details>\n",
			features.multiDrawIndirect );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>drawIndirectFirstInstance               = <div "
			"class='val'>%u</div></summary></details>\n",
			features.drawIndirectFirstInstance );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>depthClamp                              = <div "
			"class='val'>%u</div></summary></details>\n",
			features.depthClamp );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>depthBiasClamp                          = <div "
			"class='val'>%u</div></summary></details>\n",
			features.depthBiasClamp );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>fillModeNonSolid                        = <div "
			"class='val'>%u</div></summary></details>\n",
			features.fillModeNonSolid );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>depthBounds                             = <div "
			"class='val'>%u</div></summary></details>\n",
			features.depthBounds );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>wideLines                               = <div "
			"class='val'>%u</div></summary></details>\n",
			features.wideLines );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>largePoints                             = <div "
			"class='val'>%u</div></summary></details>\n",
			features.largePoints );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>alphaToOne                              = <div "
			"class='val'>%u</div></summary></details>\n",
			features.alphaToOne );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>multiViewport                           = <div "
			"class='val'>%u</div></summary></details>\n",
			features.multiViewport );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>samplerAnisotropy                       = <div "
			"class='val'>%u</div></summary></details>\n",
			features.samplerAnisotropy );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>textureCompressionETC2                  = <div "
			"class='val'>%u</div></summary></details>\n",
			features.textureCompressionETC2 );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>textureCompressionASTC_LDR              = <div "
			"class='val'>%u</div></summary></details>\n",
			features.textureCompressionASTC_LDR );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>textureCompressionBC                    = <div "
			"class='val'>%u</div></summary></details>\n",
			features.textureCompressionBC );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>occlusionQueryPrecise                   = <div "
			"class='val'>%u</div></summary></details>\n",
			features.occlusionQueryPrecise );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>pipelineStatisticsQuery                 = <div "
			"class='val'>%u</div></summary></details>\n",
			features.pipelineStatisticsQuery );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>vertexPipelineStoresAndAtomics          = <div "
			"class='val'>%u</div></summary></details>\n",
			features.vertexPipelineStoresAndAtomics );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>fragmentStoresAndAtomics                = <div "
			"class='val'>%u</div></summary></details>\n",
			features.fragmentStoresAndAtomics );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>shaderTessellationAndGeometryPointSize  = <div "
			"class='val'>%u</div></summary></details>\n",
			features.shaderTessellationAndGeometryPointSize );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>shaderImageGatherExtended               = <div "
			"class='val'>%u</div></summary></details>\n",
			features.shaderImageGatherExtended );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>shaderStorageImageExtendedFormats       = <div "
			"class='val'>%u</div></summary></details>\n",
			features.shaderStorageImageExtendedFormats );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>shaderStorageImageMultisample           = <div "
			"class='val'>%u</div></summary></details>\n",
			features.shaderStorageImageMultisample );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>shaderStorageImageReadWithoutFormat     = <div "
			"class='val'>%u</div></summary></details>\n",
			features.shaderStorageImageReadWithoutFormat );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>shaderStorageImageWriteWithoutFormat    = <div "
			"class='val'>%u</div></summary></details>\n",
			features.shaderStorageImageWriteWithoutFormat );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>shaderUniformBufferArrayDynamicIndexing = <div "
			"class='val'>%u</div></summary></details>\n",
			features.shaderUniformBufferArrayDynamicIndexing );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>shaderSampledImageArrayDynamicIndexing  = <div "
			"class='val'>%u</div></summary></details>\n",
			features.shaderSampledImageArrayDynamicIndexing );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>shaderStorageBufferArrayDynamicIndexing = <div "
			"class='val'>%u</div></summary></details>\n",
			features.shaderStorageBufferArrayDynamicIndexing );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>shaderStorageImageArrayDynamicIndexing  = <div "
			"class='val'>%u</div></summary></details>\n",
			features.shaderStorageImageArrayDynamicIndexing );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>shaderClipDistance                      = <div "
			"class='val'>%u</div></summary></details>\n",
			features.shaderClipDistance );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>shaderCullDistance                      = <div "
			"class='val'>%u</div></summary></details>\n",
			features.shaderCullDistance );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>shaderFloat64                           = <div "
			"class='val'>%u</div></summary></details>\n",
			features.shaderFloat64 );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>shaderInt64                             = <div "
			"class='val'>%u</div></summary></details>\n",
			features.shaderInt64 );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>shaderInt16                             = <div "
			"class='val'>%u</div></summary></details>\n",
			features.shaderInt16 );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>shaderResourceResidency                 = <div "
			"class='val'>%u</div></summary></details>\n",
			features.shaderResourceResidency );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>shaderResourceMinLod                    = <div "
			"class='val'>%u</div></summary></details>\n",
			features.shaderResourceMinLod );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>sparseBinding                           = <div "
			"class='val'>%u</div></summary></details>\n",
			features.sparseBinding );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>sparseResidencyBuffer                   = <div "
			"class='val'>%u</div></summary></details>\n",
			features.sparseResidencyBuffer );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>sparseResidencyImage2D                  = <div "
			"class='val'>%u</div></summary></details>\n",
			features.sparseResidencyImage2D );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>sparseResidencyImage3D                  = <div "
			"class='val'>%u</div></summary></details>\n",
			features.sparseResidencyImage3D );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>sparseResidency2Samples                 = <div "
			"class='val'>%u</div></summary></details>\n",
			features.sparseResidency2Samples );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>sparseResidency4Samples                 = <div "
			"class='val'>%u</div></summary></details>\n",
			features.sparseResidency4Samples );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>sparseResidency8Samples                 = <div "
			"class='val'>%u</div></summary></details>\n",
			features.sparseResidency8Samples );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>sparseResidency16Samples                = <div "
			"class='val'>%u</div></summary></details>\n",
			features.sparseResidency16Samples );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>sparseResidencyAliased                  = <div "
			"class='val'>%u</div></summary></details>\n",
			features.sparseResidencyAliased );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>variableMultisampleRate                 = <div "
			"class='val'>%u</div></summary></details>\n",
			features.variableMultisampleRate );
		fprintf( out,
			"\t\t\t\t\t\t<details><summary>inheritedQueries                        = <div "
			"class='val'>%u</div></summary></details>\n",
			features.inheritedQueries );
		fprintf( out, "\t\t\t\t\t</details>\n" );
	}
	else if ( human_readable_output )
	{
		printf( "VkPhysicalDeviceFeatures:\n" );
		printf( "=========================\n" );
		printf( "\trobustBufferAccess                      = %u\n", features.robustBufferAccess );
		printf( "\tfullDrawIndexUint32                     = %u\n", features.fullDrawIndexUint32 );
		printf( "\timageCubeArray                          = %u\n", features.imageCubeArray );
		printf( "\tindependentBlend                        = %u\n", features.independentBlend );
		printf( "\tgeometryShader                          = %u\n", features.geometryShader );
		printf( "\ttessellationShader                      = %u\n", features.tessellationShader );
		printf( "\tsampleRateShading                       = %u\n", features.sampleRateShading );
		printf( "\tdualSrcBlend                            = %u\n", features.dualSrcBlend );
		printf( "\tlogicOp                                 = %u\n", features.logicOp );
		printf( "\tmultiDrawIndirect                       = %u\n", features.multiDrawIndirect );
		printf( "\tdrawIndirectFirstInstance               = %u\n", features.drawIndirectFirstInstance );
		printf( "\tdepthClamp                              = %u\n", features.depthClamp );
		printf( "\tdepthBiasClamp                          = %u\n", features.depthBiasClamp );
		printf( "\tfillModeNonSolid                        = %u\n", features.fillModeNonSolid );
		printf( "\tdepthBounds                             = %u\n", features.depthBounds );
		printf( "\twideLines                               = %u\n", features.wideLines );
		printf( "\tlargePoints                             = %u\n", features.largePoints );
		printf( "\talphaToOne                              = %u\n", features.alphaToOne );
		printf( "\tmultiViewport                           = %u\n", features.multiViewport );
		printf( "\tsamplerAnisotropy                       = %u\n", features.samplerAnisotropy );
		printf( "\ttextureCompressionETC2                  = %u\n", features.textureCompressionETC2 );
		printf( "\ttextureCompressionASTC_LDR              = %u\n", features.textureCompressionASTC_LDR );
		printf( "\ttextureCompressionBC                    = %u\n", features.textureCompressionBC );
		printf( "\tocclusionQueryPrecise                   = %u\n", features.occlusionQueryPrecise );
		printf( "\tpipelineStatisticsQuery                 = %u\n", features.pipelineStatisticsQuery );
		printf( "\tvertexPipelineStoresAndAtomics          = %u\n", features.vertexPipelineStoresAndAtomics );
		printf( "\tfragmentStoresAndAtomics                = %u\n", features.fragmentStoresAndAtomics );
		printf( "\tshaderTessellationAndGeometryPointSize  = %u\n", features.shaderTessellationAndGeometryPointSize );
		printf( "\tshaderImageGatherExtended               = %u\n", features.shaderImageGatherExtended );
		printf( "\tshaderStorageImageExtendedFormats       = %u\n", features.shaderStorageImageExtendedFormats );
		printf( "\tshaderStorageImageMultisample           = %u\n", features.shaderStorageImageMultisample );
		printf( "\tshaderStorageImageReadWithoutFormat     = %u\n", features.shaderStorageImageReadWithoutFormat );
		printf( "\tshaderStorageImageWriteWithoutFormat    = %u\n", features.shaderStorageImageWriteWithoutFormat );
		printf( "\tshaderUniformBufferArrayDynamicIndexing = %u\n", features.shaderUniformBufferArrayDynamicIndexing );
		printf( "\tshaderSampledImageArrayDynamicIndexing  = %u\n", features.shaderSampledImageArrayDynamicIndexing );
		printf( "\tshaderStorageBufferArrayDynamicIndexing = %u\n", features.shaderStorageBufferArrayDynamicIndexing );
		printf( "\tshaderStorageImageArrayDynamicIndexing  = %u\n", features.shaderStorageImageArrayDynamicIndexing );
		printf( "\tshaderClipDistance                      = %u\n", features.shaderClipDistance );
		printf( "\tshaderCullDistance                      = %u\n", features.shaderCullDistance );
		printf( "\tshaderFloat64                           = %u\n", features.shaderFloat64 );
		printf( "\tshaderInt64                             = %u\n", features.shaderInt64 );
		printf( "\tshaderInt16                             = %u\n", features.shaderInt16 );
		printf( "\tshaderResourceResidency                 = %u\n", features.shaderResourceResidency );
		printf( "\tshaderResourceMinLod                    = %u\n", features.shaderResourceMinLod );
		printf( "\tsparseBinding                           = %u\n", features.sparseBinding );
		printf( "\tsparseResidencyBuffer                   = %u\n", features.sparseResidencyBuffer );
		printf( "\tsparseResidencyImage2D                  = %u\n", features.sparseResidencyImage2D );
		printf( "\tsparseResidencyImage3D                  = %u\n", features.sparseResidencyImage3D );
		printf( "\tsparseResidency2Samples                 = %u\n", features.sparseResidency2Samples );
		printf( "\tsparseResidency4Samples                 = %u\n", features.sparseResidency4Samples );
		printf( "\tsparseResidency8Samples                 = %u\n", features.sparseResidency8Samples );
		printf( "\tsparseResidency16Samples                = %u\n", features.sparseResidency16Samples );
		printf( "\tsparseResidencyAliased                  = %u\n", features.sparseResidencyAliased );
		printf( "\tvariableMultisampleRate                 = %u\n", features.variableMultisampleRate );
		printf( "\tinheritedQueries                        = %u\n", features.inheritedQueries );
	}
	if ( json_output )
	{
		printf( ",\n" );
		printf( "\t\"VkPhysicalDeviceFeatures\": {\n" );
		printf( "\t\t\"robustBufferAccess\": %u,\n", features.robustBufferAccess );
		printf( "\t\t\"fullDrawIndexUint32\": %u,\n", features.fullDrawIndexUint32 );
		printf( "\t\t\"imageCubeArray\": %u,\n", features.imageCubeArray );
		printf( "\t\t\"independentBlend\": %u,\n", features.independentBlend );
		printf( "\t\t\"geometryShader\": %u,\n", features.geometryShader );
		printf( "\t\t\"tessellationShader\": %u,\n", features.tessellationShader );
		printf( "\t\t\"sampleRateShading\": %u,\n", features.sampleRateShading );
		printf( "\t\t\"dualSrcBlend\": %u,\n", features.dualSrcBlend );
		printf( "\t\t\"logicOp\": %u,\n", features.logicOp );
		printf( "\t\t\"multiDrawIndirect\": %u,\n", features.multiDrawIndirect );
		printf( "\t\t\"drawIndirectFirstInstance\": %u,\n", features.drawIndirectFirstInstance );
		printf( "\t\t\"depthClamp\": %u,\n", features.depthClamp );
		printf( "\t\t\"depthBiasClamp\": %u,\n", features.depthBiasClamp );
		printf( "\t\t\"fillModeNonSolid\": %u,\n", features.fillModeNonSolid );
		printf( "\t\t\"depthBounds\": %u,\n", features.depthBounds );
		printf( "\t\t\"wideLines\": %u,\n", features.wideLines );
		printf( "\t\t\"largePoints\": %u,\n", features.largePoints );
		printf( "\t\t\"alphaToOne\": %u,\n", features.alphaToOne );
		printf( "\t\t\"multiViewport\": %u,\n", features.multiViewport );
		printf( "\t\t\"samplerAnisotropy\": %u,\n", features.samplerAnisotropy );
		printf( "\t\t\"textureCompressionETC2\": %u,\n", features.textureCompressionETC2 );
		printf( "\t\t\"textureCompressionASTC_LDR\": %u,\n", features.textureCompressionASTC_LDR );
		printf( "\t\t\"textureCompressionBC\": %u,\n", features.textureCompressionBC );
		printf( "\t\t\"occlusionQueryPrecise\": %u,\n", features.occlusionQueryPrecise );
		printf( "\t\t\"pipelineStatisticsQuery\": %u,\n", features.pipelineStatisticsQuery );
		printf( "\t\t\"vertexPipelineStoresAndAtomics\": %u,\n", features.vertexPipelineStoresAndAtomics );
		printf( "\t\t\"fragmentStoresAndAtomics\": %u,\n", features.fragmentStoresAndAtomics );
		printf( "\t\t\"shaderTessellationAndGeometryPointSize\": %u,\n", features.shaderTessellationAndGeometryPointSize );
		printf( "\t\t\"shaderImageGatherExtended\": %u,\n", features.shaderImageGatherExtended );
		printf( "\t\t\"shaderStorageImageExtendedFormats\": %u,\n", features.shaderStorageImageExtendedFormats );
		printf( "\t\t\"shaderStorageImageMultisample\": %u,\n", features.shaderStorageImageMultisample );
		printf( "\t\t\"shaderStorageImageReadWithoutFormat\": %u,\n", features.shaderStorageImageReadWithoutFormat );
		printf( "\t\t\"shaderStorageImageWriteWithoutFormat\": %u,\n", features.shaderStorageImageWriteWithoutFormat );
		printf( "\t\t\"shaderUniformBufferArrayDynamicIndexing\": %u,\n", features.shaderUniformBufferArrayDynamicIndexing );
		printf( "\t\t\"shaderSampledImageArrayDynamicIndexing\": %u,\n", features.shaderSampledImageArrayDynamicIndexing );
		printf( "\t\t\"shaderStorageBufferArrayDynamicIndexing\": %u,\n", features.shaderStorageBufferArrayDynamicIndexing );
		printf( "\t\t\"shaderStorageImageArrayDynamicIndexing\": %u,\n", features.shaderStorageImageArrayDynamicIndexing );
		printf( "\t\t\"shaderClipDistance\": %u,\n", features.shaderClipDistance );
		printf( "\t\t\"shaderCullDistance\": %u,\n", features.shaderCullDistance );
		printf( "\t\t\"shaderFloat64\": %u,\n", features.shaderFloat64 );
		printf( "\t\t\"shaderInt64\": %u,\n", features.shaderInt64 );
		printf( "\t\t\"shaderInt16\": %u,\n", features.shaderInt16 );
		printf( "\t\t\"shaderResourceResidency\": %u,\n", features.shaderResourceResidency );
		printf( "\t\t\"shaderResourceMinLod\": %u,\n", features.shaderResourceMinLod );
		printf( "\t\t\"sparseBinding\": %u,\n", features.sparseBinding );
		printf( "\t\t\"sparseResidencyBuffer\": %u,\n", features.sparseResidencyBuffer );
		printf( "\t\t\"sparseResidencyImage2D\": %u,\n", features.sparseResidencyImage2D );
		printf( "\t\t\"sparseResidencyImage3D\": %u,\n", features.sparseResidencyImage3D );
		printf( "\t\t\"sparseResidency2Samples\": %u,\n", features.sparseResidency2Samples );
		printf( "\t\t\"sparseResidency4Samples\": %u,\n", features.sparseResidency4Samples );
		printf( "\t\t\"sparseResidency8Samples\": %u,\n", features.sparseResidency8Samples );
		printf( "\t\t\"sparseResidency16Samples\": %u,\n", features.sparseResidency16Samples );
		printf( "\t\t\"sparseResidencyAliased\": %u,\n", features.sparseResidencyAliased );
		printf( "\t\t\"variableMultisampleRate\": %u,\n", features.variableMultisampleRate );
		printf( "\t\t\"inheritedQueries\": %u\n", features.inheritedQueries );
		printf( "\t}" );
	}
#if 0
	if ( CheckExtensionEnabled( VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, gpu->inst->inst_extensions,
		gpu->inst->inst_extensions_count ) )
	{
		void *place = gpu->features2.pNext;
		while ( place )
		{
			struct VkStructureHeader *structure = ( struct VkStructureHeader * )place;
			if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR &&
				CheckPhysicalDeviceExtensionIncluded( VK_KHR_8BIT_STORAGE_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDevice8BitStorageFeaturesKHR *b8_store_features = ( VkPhysicalDevice8BitStorageFeaturesKHR * )structure;
				if ( html_output )
				{
					fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDevice8BitStorageFeatures</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>storageBuffer8BitAccess           = <div "
						"class='val'>%u</div></summary></details>\n",
						b8_store_features->storageBuffer8BitAccess );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>uniformAndStorageBuffer8BitAccess = <div "
						"class='val'>%u</div></summary></details>\n",
						b8_store_features->uniformAndStorageBuffer8BitAccess );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>storagePushConstant8              = <div "
						"class='val'>%u</div></summary></details>\n",
						b8_store_features->storagePushConstant8 );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDevice8BitStorageFeatures:\n" );
					printf( "=====================================\n" );
					printf( "\tstorageBuffer8BitAccess           = %u\n", b8_store_features->storageBuffer8BitAccess );
					printf( "\tuniformAndStorageBuffer8BitAccess = %u\n", b8_store_features->uniformAndStorageBuffer8BitAccess );
					printf( "\tstoragePushConstant8              = %u\n", b8_store_features->storagePushConstant8 );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES_KHR &&
				CheckPhysicalDeviceExtensionIncluded( VK_KHR_16BIT_STORAGE_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDevice16BitStorageFeaturesKHR *b16_store_features = ( VkPhysicalDevice16BitStorageFeaturesKHR * )structure;
				if ( html_output )
				{
					fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDevice16BitStorageFeatures</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>storageBuffer16BitAccess           = <div "
						"class='val'>%u</div></summary></details>\n",
						b16_store_features->storageBuffer16BitAccess );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>uniformAndStorageBuffer16BitAccess = <div "
						"class='val'>%u</div></summary></details>\n",
						b16_store_features->uniformAndStorageBuffer16BitAccess );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>storagePushConstant16              = <div "
						"class='val'>%u</div></summary></details>\n",
						b16_store_features->storagePushConstant16 );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>storageInputOutput16               = <div "
						"class='val'>%u</div></summary></details>\n",
						b16_store_features->storageInputOutput16 );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDevice16BitStorageFeatures:\n" );
					printf( "=====================================\n" );
					printf( "\tstorageBuffer16BitAccess           = %u\n", b16_store_features->storageBuffer16BitAccess );
					printf( "\tuniformAndStorageBuffer16BitAccess = %u\n", b16_store_features->uniformAndStorageBuffer16BitAccess );
					printf( "\tstoragePushConstant16              = %u\n", b16_store_features->storagePushConstant16 );
					printf( "\tstorageInputOutput16               = %u\n", b16_store_features->storageInputOutput16 );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES_KHR &&
				CheckPhysicalDeviceExtensionIncluded( VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDeviceSamplerYcbcrConversionFeaturesKHR *sampler_ycbcr_features =
					( VkPhysicalDeviceSamplerYcbcrConversionFeaturesKHR * )structure;
				if ( html_output )
				{
					fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDeviceSamplerYcbcrConversionFeatures</summary>\n" );
					fprintf(
						out,
						"\t\t\t\t\t\t<details><summary>samplerYcbcrConversion = <div class='val'>%u</div></summary></details>\n",
						sampler_ycbcr_features->samplerYcbcrConversion );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceSamplerYcbcrConversionFeatures:\n" );
					printf( "===============================================\n" );
					printf( "\tsamplerYcbcrConversion = %u\n", sampler_ycbcr_features->samplerYcbcrConversion );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTER_FEATURES_KHR &&
				CheckPhysicalDeviceExtensionIncluded( VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDeviceVariablePointerFeaturesKHR *var_pointer_features =
					( VkPhysicalDeviceVariablePointerFeaturesKHR * )structure;
				if ( html_output )
				{
					fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDeviceVariablePointerFeatures</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>variablePointersStorageBuffer = <div "
						"class='val'>%u</div></summary></details>\n",
						var_pointer_features->variablePointersStorageBuffer );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>variablePointers              = <div "
						"class='val'>%u</div></summary></details>\n",
						var_pointer_features->variablePointers );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceVariablePointerFeatures:\n" );
					printf( "========================================\n" );
					printf( "\tvariablePointersStorageBuffer = %u\n", var_pointer_features->variablePointersStorageBuffer );
					printf( "\tvariablePointers              = %u\n", var_pointer_features->variablePointers );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_FEATURES_EXT &&
				CheckPhysicalDeviceExtensionIncluded( VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT *blend_op_adv_features =
					( VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT * )structure;
				if ( html_output )
				{
					fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDeviceBlendOperationAdvancedFeatures</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>advancedBlendCoherentOperations = <div "
						"class='val'>%u</div></summary></details>\n",
						blend_op_adv_features->advancedBlendCoherentOperations );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceBlendOperationAdvancedFeatures:\n" );
					printf( "===============================================\n" );
					printf( "\tadvancedBlendCoherentOperations = %u\n", blend_op_adv_features->advancedBlendCoherentOperations );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES_KHR &&
				CheckPhysicalDeviceExtensionIncluded( VK_KHR_MULTIVIEW_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDeviceMultiviewFeaturesKHR *multiview_features = ( VkPhysicalDeviceMultiviewFeaturesKHR * )structure;
				if ( html_output )
				{
					fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDeviceMultiviewFeatures</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>multiview                   = <div "
						"class='val'>%u</div></summary></details>\n",
						multiview_features->multiview );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>multiviewGeometryShader     = <div "
						"class='val'>%u</div></summary></details>\n",
						multiview_features->multiviewGeometryShader );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>multiviewTessellationShader = <div "
						"class='val'>%u</div></summary></details>\n",
						multiview_features->multiviewTessellationShader );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceMultiviewFeatures:\n" );
					printf( "==================================\n" );
					printf( "\tmultiview                   = %u\n", multiview_features->multiview );
					printf( "\tmultiviewGeometryShader     = %u\n", multiview_features->multiviewGeometryShader );
					printf( "\tmultiviewTessellationShader = %u\n", multiview_features->multiviewTessellationShader );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT16_INT8_FEATURES_KHR &&
				CheckPhysicalDeviceExtensionIncluded( VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDeviceFloat16Int8FeaturesKHR *float_int_features = ( VkPhysicalDeviceFloat16Int8FeaturesKHR * )structure;
				if ( html_output )
				{
					fprintf( out, "\n\t\t\t\t\t<details><summary>VkPhysicalDeviceFloat16Int8Features</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderFloat16 = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_int_features->shaderFloat16 );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderInt8    = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_int_features->shaderInt8 );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceFloat16Int8Features:\n" );
					printf( "====================================\n" );
					printf( "\tshaderFloat16 = %" PRIuLEAST32 "\n", float_int_features->shaderFloat16 );
					printf( "\tshaderInt8    = %" PRIuLEAST32 "\n", float_int_features->shaderInt8 );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT64_FEATURES_KHR &&
				CheckPhysicalDeviceExtensionIncluded( VK_KHR_SHADER_ATOMIC_INT64_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDeviceShaderAtomicInt64FeaturesKHR *shader_atomic_int64_features =
					( VkPhysicalDeviceShaderAtomicInt64FeaturesKHR * )structure;
				if ( html_output )
				{
					fprintf( out, "\n\t\t\t\t\t<details><summary>VkPhysicalDeviceShaderAtomicInt64Features</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderBufferInt64Atomics = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						shader_atomic_int64_features->shaderBufferInt64Atomics );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderSharedInt64Atomics = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						shader_atomic_int64_features->shaderSharedInt64Atomics );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceShaderAtomicInt64Features:\n" );
					printf( "==========================================\n" );
					printf( "\tshaderBufferInt64Atomics = %" PRIuLEAST32 "\n",
						shader_atomic_int64_features->shaderBufferInt64Atomics );
					printf( "\tshaderSharedInt64Atomics = %" PRIuLEAST32 "\n",
						shader_atomic_int64_features->shaderSharedInt64Atomics );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_FEATURES_EXT &&
				CheckPhysicalDeviceExtensionIncluded( VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDeviceTransformFeedbackFeaturesEXT *transform_feedback_features =
					( VkPhysicalDeviceTransformFeedbackFeaturesEXT * )structure;
				if ( html_output )
				{
					fprintf( out, "\n\t\t\t\t\t<details><summary>VkPhysicalDeviceTransformFeedbackFeatures</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>transformFeedback = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						transform_feedback_features->transformFeedback );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>geometryStreams   = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						transform_feedback_features->geometryStreams );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceTransformFeedbackFeatures:\n" );
					printf( "==========================================\n" );
					printf( "\ttransformFeedback = %" PRIuLEAST32 "\n", transform_feedback_features->transformFeedback );
					printf( "\tgeometryStreams   = %" PRIuLEAST32 "\n", transform_feedback_features->geometryStreams );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES_EXT &&
				CheckPhysicalDeviceExtensionIncluded( VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDeviceScalarBlockLayoutFeaturesEXT *scalar_block_layout_features =
					( VkPhysicalDeviceScalarBlockLayoutFeaturesEXT * )structure;
				if ( html_output )
				{
					fprintf( out, "\n\t\t\t\t\t<details><summary>VkPhysicalDeviceScalarBlockLayoutFeatures</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>scalarBlockLayout = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						scalar_block_layout_features->scalarBlockLayout );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceScalarBlockLayoutFeatures:\n" );
					printf( "==========================================\n" );
					printf( "\tscalarBlockLayout = %" PRIuLEAST32 "\n", scalar_block_layout_features->scalarBlockLayout );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_FEATURES_EXT &&
				CheckPhysicalDeviceExtensionIncluded( VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDeviceFragmentDensityMapFeaturesEXT *fragment_density_map_features =
					( VkPhysicalDeviceFragmentDensityMapFeaturesEXT * )structure;
				if ( html_output )
				{
					fprintf( out, "\n\t\t\t\t\t<details><summary>VkPhysicalDeviceFragmentDensityMapFeatures</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>fragmentDensityMap                    = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						fragment_density_map_features->fragmentDensityMap );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>fragmentDensityMapDynamic             = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						fragment_density_map_features->fragmentDensityMapDynamic );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>fragmentDensityMapNonSubsampledImages = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						fragment_density_map_features->fragmentDensityMapNonSubsampledImages );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceFragmentDensityMapFeatures:\n" );
					printf( "==========================================\n" );
					printf( "\tfragmentDensityMap                    = %" PRIuLEAST32 "\n",
						fragment_density_map_features->fragmentDensityMap );
					printf( "\tfragmentDensityMapDynamic             = %" PRIuLEAST32 "\n",
						fragment_density_map_features->fragmentDensityMapDynamic );
					printf( "\tfragmentDensityMapNonSubsampledImages = %" PRIuLEAST32 "\n",
						fragment_density_map_features->fragmentDensityMapNonSubsampledImages );
				}
			}
			place = structure->pNext;
		}
	}
#endif
}

static void AppDumpSparseProps( const ashes::PhysicalDeviceSparseProperties & sparse_props, FILE *out )
{
	if ( html_output )
	{
		fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDeviceSparseProperties</summary>\n" );
		fprintf( out, "\t\t\t\t\t\t<details><summary>residencyStandard2DBlockShape            = <div class='val'>%u</div></summary></details>\n", sparse_props.residencyStandard2DBlockShape );
		fprintf( out, "\t\t\t\t\t\t<details><summary>residencyStandard2DMultisampleBlockShape = <div class='val'>%u</div></summary></details>\n", sparse_props.residencyStandard2DMultisampleBlockShape );
		fprintf( out, "\t\t\t\t\t\t<details><summary>residencyStandard3DBlockShape            = <div class='val'>%u</div></summary></details>\n", sparse_props.residencyStandard3DBlockShape );
		fprintf( out, "\t\t\t\t\t\t<details><summary>residencyAlignedMipSize                  = <div class='val'>%u</div></summary></details>\n", sparse_props.residencyAlignedMipSize );
		fprintf( out, "\t\t\t\t\t\t<details><summary>residencyNonResidentStrict               = <div class='val'>%u</div></summary></details>\n", sparse_props.residencyNonResidentStrict );
		fprintf( out, "\t\t\t\t\t</details>\n" );
	}
	else if ( human_readable_output )
	{
		printf( "\tVkPhysicalDeviceSparseProperties:\n" );
		printf( "\t---------------------------------\n" );
		printf( "\t\tresidencyStandard2DBlockShape            = %u\n", sparse_props.residencyStandard2DBlockShape );
		printf( "\t\tresidencyStandard2DMultisampleBlockShape = %u\n", sparse_props.residencyStandard2DMultisampleBlockShape );
		printf( "\t\tresidencyStandard3DBlockShape            = %u\n", sparse_props.residencyStandard3DBlockShape );
		printf( "\t\tresidencyAlignedMipSize                  = %u\n", sparse_props.residencyAlignedMipSize );
		printf( "\t\tresidencyNonResidentStrict               = %u\n", sparse_props.residencyNonResidentStrict );
	}
	if ( json_output )
	{
		printf( ",\n" );
		printf( "\t\t\"sparseProperties\": {\n" );
		printf( "\t\t\t\"residencyAlignedMipSize\": %u,\n", sparse_props.residencyAlignedMipSize );
		printf( "\t\t\t\"residencyNonResidentStrict\": %u,\n", sparse_props.residencyNonResidentStrict );
		printf( "\t\t\t\"residencyStandard2DBlockShape\": %u,\n", sparse_props.residencyStandard2DBlockShape );
		printf( "\t\t\t\"residencyStandard2DMultisampleBlockShape\": %u,\n", sparse_props.residencyStandard2DMultisampleBlockShape );
		printf( "\t\t\t\"residencyStandard3DBlockShape\": %u\n", sparse_props.residencyStandard3DBlockShape );
		printf( "\t\t}" );
	}
}

static void AppDumpLimits( const ashes::PhysicalDeviceLimits & limits, FILE *out )
{
	if ( html_output )
	{
		fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDeviceLimits</summary>\n" );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxImageDimension1D                     = <div class='val'>%u</div></summary></details>\n", limits.maxImageDimension1D );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxImageDimension2D                     = <div class='val'>%u</div></summary></details>\n", limits.maxImageDimension2D );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxImageDimension3D                     = <div class='val'>%u</div></summary></details>\n", limits.maxImageDimension3D );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxImageDimensionCube                   = <div class='val'>%u</div></summary></details>\n", limits.maxImageDimensionCube );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxImageArrayLayers                     = <div class='val'>%u</div></summary></details>\n", limits.maxImageArrayLayers );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxTexelBufferElements                  = <div class='val'>0x%" PRIxLEAST32 "</div></summary></details>\n", limits.maxTexelBufferElements );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxUniformBufferRange                   = <div class='val'>0x%" PRIxLEAST32 "</div></summary></details>\n", limits.maxUniformBufferRange );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxStorageBufferRange                   = <div class='val'>0x%" PRIxLEAST32 "</div></summary></details>\n", limits.maxStorageBufferRange );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxPushConstantsSize                    = <div class='val'>%u</div></summary></details>\n", limits.maxPushConstantsSize );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxMemoryAllocationCount                = <div class='val'>%u</div></summary></details>\n", limits.maxMemoryAllocationCount );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxSamplerAllocationCount               = <div class='val'>%u</div></summary></details>\n", limits.maxSamplerAllocationCount );
		fprintf( out, "\t\t\t\t\t\t<details><summary>bufferImageGranularity                  = <div class='val'>0x%" PRIxLEAST64 "</div></summary></details>\n", limits.bufferImageGranularity );
		fprintf( out, "\t\t\t\t\t\t<details><summary>sparseAddressSpaceSize                  = <div class='val'>0x%" PRIxLEAST64 "</div></summary></details>\n", limits.sparseAddressSpaceSize );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxBoundDescriptorSets                  = <div class='val'>%u</div></summary></details>\n", limits.maxBoundDescriptorSets );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxPerStageDescriptorSamplers           = <div class='val'>%u</div></summary></details>\n", limits.maxPerStageDescriptorSamplers );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxPerStageDescriptorUniformBuffers     = <div class='val'>%u</div></summary></details>\n", limits.maxPerStageDescriptorUniformBuffers );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxPerStageDescriptorStorageBuffers     = <div class='val'>%u</div></summary></details>\n", limits.maxPerStageDescriptorStorageBuffers );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxPerStageDescriptorSampledImages      = <div class='val'>%u</div></summary></details>\n", limits.maxPerStageDescriptorSampledImages );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxPerStageDescriptorStorageImages      = <div class='val'>%u</div></summary></details>\n", limits.maxPerStageDescriptorStorageImages );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxPerStageDescriptorInputAttachments   = <div class='val'>%u</div></summary></details>\n", limits.maxPerStageDescriptorInputAttachments );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxPerStageResources                    = <div class='val'>%u</div></summary></details>\n", limits.maxPerStageResources );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxDescriptorSetSamplers                = <div class='val'>%u</div></summary></details>\n", limits.maxDescriptorSetSamplers );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxDescriptorSetUniformBuffers          = <div class='val'>%u</div></summary></details>\n", limits.maxDescriptorSetUniformBuffers );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxDescriptorSetUniformBuffersDynamic   = <div class='val'>%u</div></summary></details>\n", limits.maxDescriptorSetUniformBuffersDynamic );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxDescriptorSetStorageBuffers          = <div class='val'>%u</div></summary></details>\n", limits.maxDescriptorSetStorageBuffers );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxDescriptorSetStorageBuffersDynamic   = <div class='val'>%u</div></summary></details>\n", limits.maxDescriptorSetStorageBuffersDynamic );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxDescriptorSetSampledImages           = <div class='val'>%u</div></summary></details>\n", limits.maxDescriptorSetSampledImages );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxDescriptorSetStorageImages           = <div class='val'>%u</div></summary></details>\n", limits.maxDescriptorSetStorageImages );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxDescriptorSetInputAttachments        = <div class='val'>%u</div></summary></details>\n", limits.maxDescriptorSetInputAttachments );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxVertexInputAttributes                = <div class='val'>%u</div></summary></details>\n", limits.maxVertexInputAttributes );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxVertexInputBindings                  = <div class='val'>%u</div></summary></details>\n", limits.maxVertexInputBindings );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxVertexInputAttributeOffset           = <div class='val'>0x%" PRIxLEAST32 "</div></summary></details>\n", limits.maxVertexInputAttributeOffset );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxVertexInputBindingStride             = <div class='val'>0x%" PRIxLEAST32 "</div></summary></details>\n", limits.maxVertexInputBindingStride );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxVertexOutputComponents               = <div class='val'>%u</div></summary></details>\n", limits.maxVertexOutputComponents );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxTessellationGenerationLevel          = <div class='val'>%u</div></summary></details>\n", limits.maxTessellationGenerationLevel );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxTessellationPatchSize                        = <div class='val'>%u</div></summary></details>\n", limits.maxTessellationPatchSize );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxTessellationControlPerVertexInputComponents  = <div class='val'>%u</div></summary></details>\n", limits.maxTessellationControlPerVertexInputComponents );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxTessellationControlPerVertexOutputComponents = <div class='val'>%u</div></summary></details>\n", limits.maxTessellationControlPerVertexOutputComponents );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxTessellationControlPerPatchOutputComponents  = <div class='val'>%u</div></summary></details>\n", limits.maxTessellationControlPerPatchOutputComponents );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxTessellationControlTotalOutputComponents     = <div class='val'>%u</div></summary></details>\n", limits.maxTessellationControlTotalOutputComponents );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxTessellationEvaluationInputComponents        = <div class='val'>%u</div></summary></details>\n", limits.maxTessellationEvaluationInputComponents );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxTessellationEvaluationOutputComponents       = <div class='val'>%u</div></summary></details>\n", limits.maxTessellationEvaluationOutputComponents );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxGeometryShaderInvocations            = <div class='val'>%u</div></summary></details>\n", limits.maxGeometryShaderInvocations );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxGeometryInputComponents              = <div class='val'>%u</div></summary></details>\n", limits.maxGeometryInputComponents );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxGeometryOutputComponents             = <div class='val'>%u</div></summary></details>\n", limits.maxGeometryOutputComponents );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxGeometryOutputVertices               = <div class='val'>%u</div></summary></details>\n", limits.maxGeometryOutputVertices );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxGeometryTotalOutputComponents        = <div class='val'>%u</div></summary></details>\n", limits.maxGeometryTotalOutputComponents );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxFragmentInputComponents              = <div class='val'>%u</div></summary></details>\n", limits.maxFragmentInputComponents );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxFragmentOutputAttachments            = <div class='val'>%u</div></summary></details>\n", limits.maxFragmentOutputAttachments );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxFragmentDualSrcAttachments           = <div class='val'>%u</div></summary></details>\n", limits.maxFragmentDualSrcAttachments );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxFragmentCombinedOutputResources      = <div class='val'>%u</div></summary></details>\n", limits.maxFragmentCombinedOutputResources );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxComputeSharedMemorySize              = <div class='val'>0x%" PRIxLEAST32 "</div></summary></details>\n", limits.maxComputeSharedMemorySize );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxComputeWorkGroupCount[0]             = <div class='val'>%u</div></summary></details>\n", limits.maxComputeWorkGroupCount[0] );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxComputeWorkGroupCount[1]             = <div class='val'>%u</div></summary></details>\n", limits.maxComputeWorkGroupCount[1] );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxComputeWorkGroupCount[2]             = <div class='val'>%u</div></summary></details>\n", limits.maxComputeWorkGroupCount[2] );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxComputeWorkGroupInvocations          = <div class='val'>%u</div></summary></details>\n", limits.maxComputeWorkGroupInvocations );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxComputeWorkGroupSize[0]              = <div class='val'>%u</div></summary></details>\n", limits.maxComputeWorkGroupSize[0] );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxComputeWorkGroupSize[1]              = <div class='val'>%u</div></summary></details>\n", limits.maxComputeWorkGroupSize[1] );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxComputeWorkGroupSize[2]              = <div class='val'>%u</div></summary></details>\n", limits.maxComputeWorkGroupSize[2] );
		fprintf( out, "\t\t\t\t\t\t<details><summary>subPixelPrecisionBits                   = <div class='val'>%u</div></summary></details>\n", limits.subPixelPrecisionBits );
		fprintf( out, "\t\t\t\t\t\t<details><summary>subTexelPrecisionBits                   = <div class='val'>%u</div></summary></details>\n", limits.subTexelPrecisionBits );
		fprintf( out, "\t\t\t\t\t\t<details><summary>mipmapPrecisionBits                     = <div class='val'>%u</div></summary></details>\n", limits.mipmapPrecisionBits );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxDrawIndexedIndexValue                = <div class='val'>%u</div></summary></details>\n", limits.maxDrawIndexedIndexValue );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxDrawIndirectCount                    = <div class='val'>%u</div></summary></details>\n", limits.maxDrawIndirectCount );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxSamplerLodBias                       = <div class='val'>%f</div></summary></details>\n", limits.maxSamplerLodBias );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxSamplerAnisotropy                    = <div class='val'>%f</div></summary></details>\n", limits.maxSamplerAnisotropy );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxViewports                            = <div class='val'>%u</div></summary></details>\n", limits.maxViewports );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxViewportDimensions[0]                = <div class='val'>%u</div></summary></details>\n", limits.maxViewportDimensions[0] );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxViewportDimensions[1]                = <div class='val'>%u</div></summary></details>\n", limits.maxViewportDimensions[1] );
		fprintf( out, "\t\t\t\t\t\t<details><summary>viewportBoundsRange[0]                  =<div class='val'>%13f</div></summary></details>\n", limits.viewportBoundsRange[0] );
		fprintf( out, "\t\t\t\t\t\t<details><summary>viewportBoundsRange[1]                  =<div class='val'>%13f</div></summary></details>\n", limits.viewportBoundsRange[1] );
		fprintf( out, "\t\t\t\t\t\t<details><summary>viewportSubPixelBits                    = <div class='val'>%u</div></summary></details>\n", limits.viewportSubPixelBits );
		fprintf( out, "\t\t\t\t\t\t<details><summary>minMemoryMapAlignment                   = <div class='val'>" PRINTF_SIZE_T_SPECIFIER "</div></summary></details>\n", limits.minMemoryMapAlignment );
		fprintf( out, "\t\t\t\t\t\t<details><summary>minTexelBufferOffsetAlignment           = <div class='val'>0x%" PRIxLEAST64 "</div></summary></details>\n", limits.minTexelBufferOffsetAlignment );
		fprintf( out, "\t\t\t\t\t\t<details><summary>minUniformBufferOffsetAlignment         = <div class='val'>0x%" PRIxLEAST64 "</div></summary></details>\n", limits.minUniformBufferOffsetAlignment );
		fprintf( out, "\t\t\t\t\t\t<details><summary>minStorageBufferOffsetAlignment         = <div class='val'>0x%" PRIxLEAST64 "</div></summary></details>\n", limits.minStorageBufferOffsetAlignment );
		fprintf( out, "\t\t\t\t\t\t<details><summary>minTexelOffset                          =<div class='val'>%3d</div></summary></details>\n", limits.minTexelOffset );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxTexelOffset                          =<div class='val'>%3d</div></summary></details>\n", limits.maxTexelOffset );
		fprintf( out, "\t\t\t\t\t\t<details><summary>minTexelGatherOffset                    =<div class='val'>%3d</div></summary></details>\n", limits.minTexelGatherOffset );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxTexelGatherOffset                    =<div class='val'>%3d</div></summary></details>\n", limits.maxTexelGatherOffset );
		fprintf( out, "\t\t\t\t\t\t<details><summary>minInterpolationOffset                  =<div class='val'>%9f</div></summary></details>\n", limits.minInterpolationOffset );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxInterpolationOffset                  =<div class='val'>%9f</div></summary></details>\n", limits.maxInterpolationOffset );
		fprintf( out, "\t\t\t\t\t\t<details><summary>subPixelInterpolationOffsetBits         = <div class='val'>%u</div></summary></details>\n", limits.subPixelInterpolationOffsetBits );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxFramebufferWidth                     = <div class='val'>%u</div></summary></details>\n", limits.maxFramebufferWidth );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxFramebufferHeight                    = <div class='val'>%u</div></summary></details>\n", limits.maxFramebufferHeight );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxFramebufferLayers                    = <div class='val'>%u</div></summary></details>\n", limits.maxFramebufferLayers );
		fprintf( out, "\t\t\t\t\t\t<details><summary>framebufferColorSampleCounts            = <div class='val'>%u</div></summary></details>\n", limits.framebufferColorSampleCounts.value() );
		fprintf( out, "\t\t\t\t\t\t<details><summary>framebufferDepthSampleCounts            = <div class='val'>%u</div></summary></details>\n", limits.framebufferDepthSampleCounts.value() );
		fprintf( out, "\t\t\t\t\t\t<details><summary>framebufferStencilSampleCounts          = <div class='val'>%u</div></summary></details>\n", limits.framebufferStencilSampleCounts.value() );
		fprintf( out, "\t\t\t\t\t\t<details><summary>framebufferNoAttachmentsSampleCounts    = <div class='val'>%u</div></summary></details>\n", limits.framebufferNoAttachmentsSampleCounts.value() );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxColorAttachments                     = <div class='val'>%u</div></summary></details>\n", limits.maxColorAttachments );
		fprintf( out, "\t\t\t\t\t\t<details><summary>sampledImageColorSampleCounts           = <div class='val'>%u</div></summary></details>\n", limits.sampledImageColorSampleCounts.value() );
		fprintf( out, "\t\t\t\t\t\t<details><summary>sampledImageDepthSampleCounts           = <div class='val'>%u</div></summary></details>\n", limits.sampledImageDepthSampleCounts.value() );
		fprintf( out, "\t\t\t\t\t\t<details><summary>sampledImageStencilSampleCounts         = <div class='val'>%u</div></summary></details>\n", limits.sampledImageStencilSampleCounts.value() );
		fprintf( out, "\t\t\t\t\t\t<details><summary>sampledImageIntegerSampleCounts         = <div class='val'>%u</div></summary></details>\n", limits.sampledImageIntegerSampleCounts.value() );
		fprintf( out, "\t\t\t\t\t\t<details><summary>storageImageSampleCounts                = <div class='val'>%u</div></summary></details>\n", limits.storageImageSampleCounts.value() );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxSampleMaskWords                      = <div class='val'>%u</div></summary></details>\n", limits.maxSampleMaskWords );
		fprintf( out, "\t\t\t\t\t\t<details><summary>timestampComputeAndGraphics             = <div class='val'>%u</div></summary></details>\n", limits.timestampComputeAndGraphics );
		fprintf( out, "\t\t\t\t\t\t<details><summary>timestampPeriod                         = <div class='val'>%f</div></summary></details>\n", limits.timestampPeriod );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxClipDistances                        = <div class='val'>%u</div></summary></details>\n", limits.maxClipDistances );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxCullDistances                        = <div class='val'>%u</div></summary></details>\n", limits.maxCullDistances );
		fprintf( out, "\t\t\t\t\t\t<details><summary>maxCombinedClipAndCullDistances         = <div class='val'>%u</div></summary></details>\n", limits.maxCombinedClipAndCullDistances );
		fprintf( out, "\t\t\t\t\t\t<details><summary>discreteQueuePriorities                 = <div class='val'>%u</div></summary></details>\n", limits.discreteQueuePriorities );
		fprintf( out, "\t\t\t\t\t\t<details><summary>pointSizeRange[0]                       = <div class='val'>%f</div></summary></details>\n", limits.pointSizeRange[0] );
		fprintf( out, "\t\t\t\t\t\t<details><summary>pointSizeRange[1]                       = <div class='val'>%f</div></summary></details>\n", limits.pointSizeRange[1] );
		fprintf( out, "\t\t\t\t\t\t<details><summary>lineWidthRange[0]                       = <div class='val'>%f</div></summary></details>\n", limits.lineWidthRange[0] );
		fprintf( out, "\t\t\t\t\t\t<details><summary>lineWidthRange[1]                       = <div class='val'>%f</div></summary></details>\n", limits.lineWidthRange[1] );
		fprintf( out, "\t\t\t\t\t\t<details><summary>pointSizeGranularity                    = <div class='val'>%f</div></summary></details>\n", limits.pointSizeGranularity );
		fprintf( out, "\t\t\t\t\t\t<details><summary>lineWidthGranularity                    = <div class='val'>%f</div></summary></details>\n", limits.lineWidthGranularity );
		fprintf( out, "\t\t\t\t\t\t<details><summary>strictLines                             = <div class='val'>%u</div></summary></details>\n", limits.strictLines );
		fprintf( out, "\t\t\t\t\t\t<details><summary>standardSampleLocations                 = <div class='val'>%u</div></summary></details>\n", limits.standardSampleLocations );
		fprintf( out, "\t\t\t\t\t\t<details><summary>optimalBufferCopyOffsetAlignment        = <div class='val'>0x%" PRIxLEAST64 "</div></summary></details>\n", limits.optimalBufferCopyOffsetAlignment );
		fprintf( out, "\t\t\t\t\t\t<details><summary>optimalBufferCopyRowPitchAlignment      = <div class='val'>0x%" PRIxLEAST64 "</div></summary></details>\n", limits.optimalBufferCopyRowPitchAlignment );
		fprintf( out, "\t\t\t\t\t\t<details><summary>nonCoherentAtomSize                     = <div class='val'>0x%" PRIxLEAST64 "</div></summary></details>\n", limits.nonCoherentAtomSize );
		fprintf( out, "\t\t\t\t\t</details>\n" );
	}
	else if ( human_readable_output )
	{
		printf( "\tVkPhysicalDeviceLimits:\n" );
		printf( "\t-----------------------\n" );
		printf( "\t\tmaxImageDimension1D                     = %u\n", limits.maxImageDimension1D );
		printf( "\t\tmaxImageDimension2D                     = %u\n", limits.maxImageDimension2D );
		printf( "\t\tmaxImageDimension3D                     = %u\n", limits.maxImageDimension3D );
		printf( "\t\tmaxImageDimensionCube                   = %u\n", limits.maxImageDimensionCube );
		printf( "\t\tmaxImageArrayLayers                     = %u\n", limits.maxImageArrayLayers );
		printf( "\t\tmaxTexelBufferElements                  = 0x%" PRIxLEAST32 "\n", limits.maxTexelBufferElements );
		printf( "\t\tmaxUniformBufferRange                   = 0x%" PRIxLEAST32 "\n", limits.maxUniformBufferRange );
		printf( "\t\tmaxStorageBufferRange                   = 0x%" PRIxLEAST32 "\n", limits.maxStorageBufferRange );
		printf( "\t\tmaxPushConstantsSize                    = %u\n", limits.maxPushConstantsSize );
		printf( "\t\tmaxMemoryAllocationCount                = %u\n", limits.maxMemoryAllocationCount );
		printf( "\t\tmaxSamplerAllocationCount               = %u\n", limits.maxSamplerAllocationCount );
		printf( "\t\tbufferImageGranularity                  = 0x%" PRIxLEAST64 "\n", limits.bufferImageGranularity );
		printf( "\t\tsparseAddressSpaceSize                  = 0x%" PRIxLEAST64 "\n", limits.sparseAddressSpaceSize );
		printf( "\t\tmaxBoundDescriptorSets                  = %u\n", limits.maxBoundDescriptorSets );
		printf( "\t\tmaxPerStageDescriptorSamplers           = %u\n", limits.maxPerStageDescriptorSamplers );
		printf( "\t\tmaxPerStageDescriptorUniformBuffers     = %u\n", limits.maxPerStageDescriptorUniformBuffers );
		printf( "\t\tmaxPerStageDescriptorStorageBuffers     = %u\n", limits.maxPerStageDescriptorStorageBuffers );
		printf( "\t\tmaxPerStageDescriptorSampledImages      = %u\n", limits.maxPerStageDescriptorSampledImages );
		printf( "\t\tmaxPerStageDescriptorStorageImages      = %u\n", limits.maxPerStageDescriptorStorageImages );
		printf( "\t\tmaxPerStageDescriptorInputAttachments   = %u\n", limits.maxPerStageDescriptorInputAttachments );
		printf( "\t\tmaxPerStageResources                    = %u\n", limits.maxPerStageResources );
		printf( "\t\tmaxDescriptorSetSamplers                = %u\n", limits.maxDescriptorSetSamplers );
		printf( "\t\tmaxDescriptorSetUniformBuffers          = %u\n", limits.maxDescriptorSetUniformBuffers );
		printf( "\t\tmaxDescriptorSetUniformBuffersDynamic   = %u\n", limits.maxDescriptorSetUniformBuffersDynamic );
		printf( "\t\tmaxDescriptorSetStorageBuffers          = %u\n", limits.maxDescriptorSetStorageBuffers );
		printf( "\t\tmaxDescriptorSetStorageBuffersDynamic   = %u\n", limits.maxDescriptorSetStorageBuffersDynamic );
		printf( "\t\tmaxDescriptorSetSampledImages           = %u\n", limits.maxDescriptorSetSampledImages );
		printf( "\t\tmaxDescriptorSetStorageImages           = %u\n", limits.maxDescriptorSetStorageImages );
		printf( "\t\tmaxDescriptorSetInputAttachments        = %u\n", limits.maxDescriptorSetInputAttachments );
		printf( "\t\tmaxVertexInputAttributes                = %u\n", limits.maxVertexInputAttributes );
		printf( "\t\tmaxVertexInputBindings                  = %u\n", limits.maxVertexInputBindings );
		printf( "\t\tmaxVertexInputAttributeOffset           = 0x%" PRIxLEAST32 "\n", limits.maxVertexInputAttributeOffset );
		printf( "\t\tmaxVertexInputBindingStride             = 0x%" PRIxLEAST32 "\n", limits.maxVertexInputBindingStride );
		printf( "\t\tmaxVertexOutputComponents               = %u\n", limits.maxVertexOutputComponents );
		printf( "\t\tmaxTessellationGenerationLevel          = %u\n", limits.maxTessellationGenerationLevel );
		printf( "\t\tmaxTessellationPatchSize                        = %u\n", limits.maxTessellationPatchSize );
		printf( "\t\tmaxTessellationControlPerVertexInputComponents  = %u\n", limits.maxTessellationControlPerVertexInputComponents );
		printf( "\t\tmaxTessellationControlPerVertexOutputComponents = %u\n", limits.maxTessellationControlPerVertexOutputComponents );
		printf( "\t\tmaxTessellationControlPerPatchOutputComponents  = %u\n", limits.maxTessellationControlPerPatchOutputComponents );
		printf( "\t\tmaxTessellationControlTotalOutputComponents     = %u\n", limits.maxTessellationControlTotalOutputComponents );
		printf( "\t\tmaxTessellationEvaluationInputComponents        = %u\n", limits.maxTessellationEvaluationInputComponents );
		printf( "\t\tmaxTessellationEvaluationOutputComponents       = %u\n", limits.maxTessellationEvaluationOutputComponents );
		printf( "\t\tmaxGeometryShaderInvocations            = %u\n", limits.maxGeometryShaderInvocations );
		printf( "\t\tmaxGeometryInputComponents              = %u\n", limits.maxGeometryInputComponents );
		printf( "\t\tmaxGeometryOutputComponents             = %u\n", limits.maxGeometryOutputComponents );
		printf( "\t\tmaxGeometryOutputVertices               = %u\n", limits.maxGeometryOutputVertices );
		printf( "\t\tmaxGeometryTotalOutputComponents        = %u\n", limits.maxGeometryTotalOutputComponents );
		printf( "\t\tmaxFragmentInputComponents              = %u\n", limits.maxFragmentInputComponents );
		printf( "\t\tmaxFragmentOutputAttachments            = %u\n", limits.maxFragmentOutputAttachments );
		printf( "\t\tmaxFragmentDualSrcAttachments           = %u\n", limits.maxFragmentDualSrcAttachments );
		printf( "\t\tmaxFragmentCombinedOutputResources      = %u\n", limits.maxFragmentCombinedOutputResources );
		printf( "\t\tmaxComputeSharedMemorySize              = 0x%" PRIxLEAST32 "\n", limits.maxComputeSharedMemorySize );
		printf( "\t\tmaxComputeWorkGroupCount[0]             = %u\n", limits.maxComputeWorkGroupCount[0] );
		printf( "\t\tmaxComputeWorkGroupCount[1]             = %u\n", limits.maxComputeWorkGroupCount[1] );
		printf( "\t\tmaxComputeWorkGroupCount[2]             = %u\n", limits.maxComputeWorkGroupCount[2] );
		printf( "\t\tmaxComputeWorkGroupInvocations          = %u\n", limits.maxComputeWorkGroupInvocations );
		printf( "\t\tmaxComputeWorkGroupSize[0]              = %u\n", limits.maxComputeWorkGroupSize[0] );
		printf( "\t\tmaxComputeWorkGroupSize[1]              = %u\n", limits.maxComputeWorkGroupSize[1] );
		printf( "\t\tmaxComputeWorkGroupSize[2]              = %u\n", limits.maxComputeWorkGroupSize[2] );
		printf( "\t\tsubPixelPrecisionBits                   = %u\n", limits.subPixelPrecisionBits );
		printf( "\t\tsubTexelPrecisionBits                   = %u\n", limits.subTexelPrecisionBits );
		printf( "\t\tmipmapPrecisionBits                     = %u\n", limits.mipmapPrecisionBits );
		printf( "\t\tmaxDrawIndexedIndexValue                = %u\n", limits.maxDrawIndexedIndexValue );
		printf( "\t\tmaxDrawIndirectCount                    = %u\n", limits.maxDrawIndirectCount );
		printf( "\t\tmaxSamplerLodBias                       = %f\n", limits.maxSamplerLodBias );
		printf( "\t\tmaxSamplerAnisotropy                    = %f\n", limits.maxSamplerAnisotropy );
		printf( "\t\tmaxViewports                            = %u\n", limits.maxViewports );
		printf( "\t\tmaxViewportDimensions[0]                = %u\n", limits.maxViewportDimensions[0] );
		printf( "\t\tmaxViewportDimensions[1]                = %u\n", limits.maxViewportDimensions[1] );
		printf( "\t\tviewportBoundsRange[0]                  =%13f\n", limits.viewportBoundsRange[0] );
		printf( "\t\tviewportBoundsRange[1]                  =%13f\n", limits.viewportBoundsRange[1] );
		printf( "\t\tviewportSubPixelBits                    = %u\n", limits.viewportSubPixelBits );
		printf( "\t\tminMemoryMapAlignment                   = " PRINTF_SIZE_T_SPECIFIER "\n", limits.minMemoryMapAlignment );
		printf( "\t\tminTexelBufferOffsetAlignment           = 0x%" PRIxLEAST64 "\n", limits.minTexelBufferOffsetAlignment );
		printf( "\t\tminUniformBufferOffsetAlignment         = 0x%" PRIxLEAST64 "\n", limits.minUniformBufferOffsetAlignment );
		printf( "\t\tminStorageBufferOffsetAlignment         = 0x%" PRIxLEAST64 "\n", limits.minStorageBufferOffsetAlignment );
		printf( "\t\tminTexelOffset                          =%3d\n", limits.minTexelOffset );
		printf( "\t\tmaxTexelOffset                          =%3d\n", limits.maxTexelOffset );
		printf( "\t\tminTexelGatherOffset                    =%3d\n", limits.minTexelGatherOffset );
		printf( "\t\tmaxTexelGatherOffset                    =%3d\n", limits.maxTexelGatherOffset );
		printf( "\t\tminInterpolationOffset                  =%9f\n", limits.minInterpolationOffset );
		printf( "\t\tmaxInterpolationOffset                  =%9f\n", limits.maxInterpolationOffset );
		printf( "\t\tsubPixelInterpolationOffsetBits         = %u\n", limits.subPixelInterpolationOffsetBits );
		printf( "\t\tmaxFramebufferWidth                     = %u\n", limits.maxFramebufferWidth );
		printf( "\t\tmaxFramebufferHeight                    = %u\n", limits.maxFramebufferHeight );
		printf( "\t\tmaxFramebufferLayers                    = %u\n", limits.maxFramebufferLayers );
		printf( "\t\tframebufferColorSampleCounts            = %u\n", limits.framebufferColorSampleCounts.value() );
		printf( "\t\tframebufferDepthSampleCounts            = %u\n", limits.framebufferDepthSampleCounts.value() );
		printf( "\t\tframebufferStencilSampleCounts          = %u\n", limits.framebufferStencilSampleCounts.value() );
		printf( "\t\tframebufferNoAttachmentsSampleCounts    = %u\n", limits.framebufferNoAttachmentsSampleCounts.value() );
		printf( "\t\tmaxColorAttachments                     = %u\n", limits.maxColorAttachments );
		printf( "\t\tsampledImageColorSampleCounts           = %u\n", limits.sampledImageColorSampleCounts.value() );
		printf( "\t\tsampledImageDepthSampleCounts           = %u\n", limits.sampledImageDepthSampleCounts.value() );
		printf( "\t\tsampledImageStencilSampleCounts         = %u\n", limits.sampledImageStencilSampleCounts.value() );
		printf( "\t\tsampledImageIntegerSampleCounts         = %u\n", limits.sampledImageIntegerSampleCounts.value() );
		printf( "\t\tstorageImageSampleCounts                = %u\n", limits.storageImageSampleCounts.value() );
		printf( "\t\tmaxSampleMaskWords                      = %u\n", limits.maxSampleMaskWords );
		printf( "\t\ttimestampComputeAndGraphics             = %u\n", limits.timestampComputeAndGraphics );
		printf( "\t\ttimestampPeriod                         = %f\n", limits.timestampPeriod );
		printf( "\t\tmaxClipDistances                        = %u\n", limits.maxClipDistances );
		printf( "\t\tmaxCullDistances                        = %u\n", limits.maxCullDistances );
		printf( "\t\tmaxCombinedClipAndCullDistances         = %u\n", limits.maxCombinedClipAndCullDistances );
		printf( "\t\tdiscreteQueuePriorities                 = %u\n", limits.discreteQueuePriorities );
		printf( "\t\tpointSizeRange[0]                       = %f\n", limits.pointSizeRange[0] );
		printf( "\t\tpointSizeRange[1]                       = %f\n", limits.pointSizeRange[1] );
		printf( "\t\tlineWidthRange[0]                       = %f\n", limits.lineWidthRange[0] );
		printf( "\t\tlineWidthRange[1]                       = %f\n", limits.lineWidthRange[1] );
		printf( "\t\tpointSizeGranularity                    = %f\n", limits.pointSizeGranularity );
		printf( "\t\tlineWidthGranularity                    = %f\n", limits.lineWidthGranularity );
		printf( "\t\tstrictLines                             = %u\n", limits.strictLines );
		printf( "\t\tstandardSampleLocations                 = %u\n", limits.standardSampleLocations );
		printf( "\t\toptimalBufferCopyOffsetAlignment        = 0x%" PRIxLEAST64 "\n", limits.optimalBufferCopyOffsetAlignment );
		printf( "\t\toptimalBufferCopyRowPitchAlignment      = 0x%" PRIxLEAST64 "\n", limits.optimalBufferCopyRowPitchAlignment );
		printf( "\t\tnonCoherentAtomSize                     = 0x%" PRIxLEAST64 "\n", limits.nonCoherentAtomSize );
	}
	if ( json_output )
	{
		printf( ",\n" );
		printf( "\t\t\"limits\": {\n" );
		printf( "\t\t\t\"bufferImageGranularity\": %llu,\n", ( unsigned long long )limits.bufferImageGranularity );
		printf( "\t\t\t\"discreteQueuePriorities\": %u,\n", limits.discreteQueuePriorities );
		printf( "\t\t\t\"framebufferColorSampleCounts\": %u,\n", limits.framebufferColorSampleCounts.value() );
		printf( "\t\t\t\"framebufferDepthSampleCounts\": %u,\n", limits.framebufferDepthSampleCounts.value() );
		printf( "\t\t\t\"framebufferNoAttachmentsSampleCounts\": %u,\n", limits.framebufferNoAttachmentsSampleCounts.value() );
		printf( "\t\t\t\"framebufferStencilSampleCounts\": %u,\n", limits.framebufferStencilSampleCounts.value() );
		printf( "\t\t\t\"lineWidthGranularity\": %g,\n", limits.lineWidthGranularity );
		printf( "\t\t\t\"lineWidthRange\": [\n" );
		printf( "\t\t\t\t%g,\n", limits.lineWidthRange[0] );
		printf( "\t\t\t\t%g\n", limits.lineWidthRange[1] );
		printf( "\t\t\t],\n" );
		printf( "\t\t\t\"maxBoundDescriptorSets\": %u,\n", limits.maxBoundDescriptorSets );
		printf( "\t\t\t\"maxClipDistances\": %u,\n", limits.maxClipDistances );
		printf( "\t\t\t\"maxColorAttachments\": %u,\n", limits.maxColorAttachments );
		printf( "\t\t\t\"maxCombinedClipAndCullDistances\": %u,\n", limits.maxCombinedClipAndCullDistances );
		printf( "\t\t\t\"maxComputeSharedMemorySize\": %u,\n", limits.maxComputeSharedMemorySize );
		printf( "\t\t\t\"maxComputeWorkGroupCount\": [\n" );
		printf( "\t\t\t\t%u,\n", limits.maxComputeWorkGroupCount[0] );
		printf( "\t\t\t\t%u,\n", limits.maxComputeWorkGroupCount[1] );
		printf( "\t\t\t\t%u\n", limits.maxComputeWorkGroupCount[2] );
		printf( "\t\t\t],\n" );
		printf( "\t\t\t\"maxComputeWorkGroupInvocations\": %u,\n", limits.maxComputeWorkGroupInvocations );
		printf( "\t\t\t\"maxComputeWorkGroupSize\": [\n" );
		printf( "\t\t\t\t%u,\n", limits.maxComputeWorkGroupSize[0] );
		printf( "\t\t\t\t%u,\n", limits.maxComputeWorkGroupSize[1] );
		printf( "\t\t\t\t%u\n", limits.maxComputeWorkGroupSize[2] );
		printf( "\t\t\t],\n" );
		printf( "\t\t\t\"maxCullDistances\": %u,\n", limits.maxCullDistances );
		printf( "\t\t\t\"maxDescriptorSetInputAttachments\": %u,\n", limits.maxDescriptorSetInputAttachments );
		printf( "\t\t\t\"maxDescriptorSetSampledImages\": %u,\n", limits.maxDescriptorSetSampledImages );
		printf( "\t\t\t\"maxDescriptorSetSamplers\": %u,\n", limits.maxDescriptorSetSamplers );
		printf( "\t\t\t\"maxDescriptorSetStorageBuffers\": %u,\n", limits.maxDescriptorSetStorageBuffers );
		printf( "\t\t\t\"maxDescriptorSetStorageBuffersDynamic\": %u,\n", limits.maxDescriptorSetStorageBuffersDynamic );
		printf( "\t\t\t\"maxDescriptorSetStorageImages\": %u,\n", limits.maxDescriptorSetStorageImages );
		printf( "\t\t\t\"maxDescriptorSetUniformBuffers\": %u,\n", limits.maxDescriptorSetUniformBuffers );
		printf( "\t\t\t\"maxDescriptorSetUniformBuffersDynamic\": %u,\n", limits.maxDescriptorSetUniformBuffersDynamic );
		printf( "\t\t\t\"maxDrawIndexedIndexValue\": %u,\n", limits.maxDrawIndexedIndexValue );
		printf( "\t\t\t\"maxDrawIndirectCount\": %u,\n", limits.maxDrawIndirectCount );
		printf( "\t\t\t\"maxFragmentCombinedOutputResources\": %u,\n", limits.maxFragmentCombinedOutputResources );
		printf( "\t\t\t\"maxFragmentDualSrcAttachments\": %u,\n", limits.maxFragmentDualSrcAttachments );
		printf( "\t\t\t\"maxFragmentInputComponents\": %u,\n", limits.maxFragmentInputComponents );
		printf( "\t\t\t\"maxFragmentOutputAttachments\": %u,\n", limits.maxFragmentOutputAttachments );
		printf( "\t\t\t\"maxFramebufferHeight\": %u,\n", limits.maxFramebufferHeight );
		printf( "\t\t\t\"maxFramebufferLayers\": %u,\n", limits.maxFramebufferLayers );
		printf( "\t\t\t\"maxFramebufferWidth\": %u,\n", limits.maxFramebufferWidth );
		printf( "\t\t\t\"maxGeometryInputComponents\": %u,\n", limits.maxGeometryInputComponents );
		printf( "\t\t\t\"maxGeometryOutputComponents\": %u,\n", limits.maxGeometryOutputComponents );
		printf( "\t\t\t\"maxGeometryOutputVertices\": %u,\n", limits.maxGeometryOutputVertices );
		printf( "\t\t\t\"maxGeometryShaderInvocations\": %u,\n", limits.maxGeometryShaderInvocations );
		printf( "\t\t\t\"maxGeometryTotalOutputComponents\": %u,\n", limits.maxGeometryTotalOutputComponents );
		printf( "\t\t\t\"maxImageArrayLayers\": %u,\n", limits.maxImageArrayLayers );
		printf( "\t\t\t\"maxImageDimension1D\": %u,\n", limits.maxImageDimension1D );
		printf( "\t\t\t\"maxImageDimension2D\": %u,\n", limits.maxImageDimension2D );
		printf( "\t\t\t\"maxImageDimension3D\": %u,\n", limits.maxImageDimension3D );
		printf( "\t\t\t\"maxImageDimensionCube\": %u,\n", limits.maxImageDimensionCube );
		printf( "\t\t\t\"maxInterpolationOffset\": %g,\n", limits.maxInterpolationOffset );
		printf( "\t\t\t\"maxMemoryAllocationCount\": %u,\n", limits.maxMemoryAllocationCount );
		printf( "\t\t\t\"maxPerStageDescriptorInputAttachments\": %u,\n", limits.maxPerStageDescriptorInputAttachments );
		printf( "\t\t\t\"maxPerStageDescriptorSampledImages\": %u,\n", limits.maxPerStageDescriptorSampledImages );
		printf( "\t\t\t\"maxPerStageDescriptorSamplers\": %u,\n", limits.maxPerStageDescriptorSamplers );
		printf( "\t\t\t\"maxPerStageDescriptorStorageBuffers\": %u,\n", limits.maxPerStageDescriptorStorageBuffers );
		printf( "\t\t\t\"maxPerStageDescriptorStorageImages\": %u,\n", limits.maxPerStageDescriptorStorageImages );
		printf( "\t\t\t\"maxPerStageDescriptorUniformBuffers\": %u,\n", limits.maxPerStageDescriptorUniformBuffers );
		printf( "\t\t\t\"maxPerStageResources\": %u,\n", limits.maxPerStageResources );
		printf( "\t\t\t\"maxPushConstantsSize\": %u,\n", limits.maxPushConstantsSize );
		printf( "\t\t\t\"maxSampleMaskWords\": %u,\n", limits.maxSampleMaskWords );
		printf( "\t\t\t\"maxSamplerAllocationCount\": %u,\n", limits.maxSamplerAllocationCount );
		printf( "\t\t\t\"maxSamplerAnisotropy\": %g,\n", limits.maxSamplerAnisotropy );
		printf( "\t\t\t\"maxSamplerLodBias\": %g,\n", limits.maxSamplerLodBias );
		printf( "\t\t\t\"maxStorageBufferRange\": %u,\n", limits.maxStorageBufferRange );
		printf( "\t\t\t\"maxTessellationControlPerPatchOutputComponents\": %u,\n", limits.maxTessellationControlPerPatchOutputComponents );
		printf( "\t\t\t\"maxTessellationControlPerVertexInputComponents\": %u,\n", limits.maxTessellationControlPerVertexInputComponents );
		printf( "\t\t\t\"maxTessellationControlPerVertexOutputComponents\": %u,\n", limits.maxTessellationControlPerVertexOutputComponents );
		printf( "\t\t\t\"maxTessellationControlTotalOutputComponents\": %u,\n", limits.maxTessellationControlTotalOutputComponents );
		printf( "\t\t\t\"maxTessellationEvaluationInputComponents\": %u,\n", limits.maxTessellationEvaluationInputComponents );
		printf( "\t\t\t\"maxTessellationEvaluationOutputComponents\": %u,\n", limits.maxTessellationEvaluationOutputComponents );
		printf( "\t\t\t\"maxTessellationGenerationLevel\": %u,\n", limits.maxTessellationGenerationLevel );
		printf( "\t\t\t\"maxTessellationPatchSize\": %u,\n", limits.maxTessellationPatchSize );
		printf( "\t\t\t\"maxTexelBufferElements\": %u,\n", limits.maxTexelBufferElements );
		printf( "\t\t\t\"maxTexelGatherOffset\": %u,\n", limits.maxTexelGatherOffset );
		printf( "\t\t\t\"maxTexelOffset\": %u,\n", limits.maxTexelOffset );
		printf( "\t\t\t\"maxUniformBufferRange\": %u,\n", limits.maxUniformBufferRange );
		printf( "\t\t\t\"maxVertexInputAttributeOffset\": %u,\n", limits.maxVertexInputAttributeOffset );
		printf( "\t\t\t\"maxVertexInputAttributes\": %u,\n", limits.maxVertexInputAttributes );
		printf( "\t\t\t\"maxVertexInputBindings\": %u,\n", limits.maxVertexInputBindings );
		printf( "\t\t\t\"maxVertexInputBindingStride\": %u,\n", limits.maxVertexInputBindingStride );
		printf( "\t\t\t\"maxVertexOutputComponents\": %u,\n", limits.maxVertexOutputComponents );
		printf( "\t\t\t\"maxViewportDimensions\": [\n" );
		printf( "\t\t\t\t%u,\n", limits.maxViewportDimensions[0] );
		printf( "\t\t\t\t%u\n", limits.maxViewportDimensions[1] );
		printf( "\t\t\t],\n" );
		printf( "\t\t\t\"maxViewports\": %u,\n", limits.maxViewports );
		printf( "\t\t\t\"minInterpolationOffset\": %g,\n", limits.minInterpolationOffset );
		printf( "\t\t\t\"minMemoryMapAlignment\": " PRINTF_SIZE_T_SPECIFIER ",\n", limits.minMemoryMapAlignment );
		printf( "\t\t\t\"minStorageBufferOffsetAlignment\": %llu,\n", ( unsigned long long )limits.minStorageBufferOffsetAlignment );
		printf( "\t\t\t\"minTexelBufferOffsetAlignment\": %llu,\n", ( unsigned long long )limits.minTexelBufferOffsetAlignment );
		printf( "\t\t\t\"minTexelGatherOffset\": %d,\n", limits.minTexelGatherOffset );
		printf( "\t\t\t\"minTexelOffset\": %d,\n", limits.minTexelOffset );
		printf( "\t\t\t\"minUniformBufferOffsetAlignment\": %llu,\n", ( unsigned long long )limits.minUniformBufferOffsetAlignment );
		printf( "\t\t\t\"mipmapPrecisionBits\": %u,\n", limits.mipmapPrecisionBits );
		printf( "\t\t\t\"nonCoherentAtomSize\": %llu,\n", ( unsigned long long )limits.nonCoherentAtomSize );
		printf( "\t\t\t\"optimalBufferCopyOffsetAlignment\": %llu,\n", ( unsigned long long )limits.optimalBufferCopyOffsetAlignment );
		printf( "\t\t\t\"optimalBufferCopyRowPitchAlignment\": %llu,\n", ( unsigned long long )limits.optimalBufferCopyRowPitchAlignment );
		printf( "\t\t\t\"pointSizeGranularity\": %g,\n", limits.pointSizeGranularity );
		printf( "\t\t\t\"pointSizeRange\": [\n" );
		printf( "\t\t\t\t%g,\n", limits.pointSizeRange[0] );
		printf( "\t\t\t\t%g\n", limits.pointSizeRange[1] );
		printf( "\t\t\t],\n" );
		printf( "\t\t\t\"sampledImageColorSampleCounts\": %u,\n", limits.sampledImageColorSampleCounts.value() );
		printf( "\t\t\t\"sampledImageDepthSampleCounts\": %u,\n", limits.sampledImageDepthSampleCounts.value() );
		printf( "\t\t\t\"sampledImageIntegerSampleCounts\": %u,\n", limits.sampledImageIntegerSampleCounts.value() );
		printf( "\t\t\t\"sampledImageStencilSampleCounts\": %u,\n", limits.sampledImageStencilSampleCounts.value() );
		printf( "\t\t\t\"sparseAddressSpaceSize\": %llu,\n", ( unsigned long long )limits.sparseAddressSpaceSize );
		printf( "\t\t\t\"standardSampleLocations\": %u,\n", limits.standardSampleLocations );
		printf( "\t\t\t\"storageImageSampleCounts\": %u,\n", limits.storageImageSampleCounts.value() );
		printf( "\t\t\t\"strictLines\": %u,\n", limits.strictLines );
		printf( "\t\t\t\"subPixelInterpolationOffsetBits\": %u,\n", limits.subPixelInterpolationOffsetBits );
		printf( "\t\t\t\"subPixelPrecisionBits\": %u,\n", limits.subPixelPrecisionBits );
		printf( "\t\t\t\"subTexelPrecisionBits\": %u,\n", limits.subTexelPrecisionBits );
		printf( "\t\t\t\"timestampComputeAndGraphics\": %u,\n", limits.timestampComputeAndGraphics );
		printf( "\t\t\t\"timestampPeriod\": %g,\n", limits.timestampPeriod );
		printf( "\t\t\t\"viewportBoundsRange\": [\n" );
		printf( "\t\t\t\t%g,\n", limits.viewportBoundsRange[0] );
		printf( "\t\t\t\t%g\n", limits.viewportBoundsRange[1] );
		printf( "\t\t\t],\n" );
		printf( "\t\t\t\"viewportSubPixelBits\": %u\n", limits.viewportSubPixelBits );
		printf( "\t\t}" );
	}
}

static void AppGpuDumpProps( const AppGpu * gpu
	, FILE *out )
{
	ashes::PhysicalDeviceProperties props;
#if 0
	if ( CheckExtensionEnabled( VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, gpu->inst->inst_extensions,
		gpu->inst->inst_extensions_count ) )
	{
		const VkPhysicalDeviceProperties *props2_const = &gpu->props2.properties;
		props = *props2_const;
	}
	else
#endif
	{
		props = gpu->obj->getProperties();
	}

	const uint32_t apiVersion = props.apiVersion;
	uint32_t major;
	uint32_t minor;
	uint32_t patch;
	ExtractVersion( apiVersion, &major, &minor, &patch );

	if ( html_output )
	{
		fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDeviceProperties</summary>\n" );
		fprintf( out, "\t\t\t\t\t\t<details><summary>apiVersion = <div class='val'>0x%" PRIxLEAST32 "</div>  (<div class='val'>%d.%d.%d</div>)</summary></details>\n", apiVersion, major, minor, patch );
		fprintf( out, "\t\t\t\t\t\t<details><summary>driverVersion = <div class='val'>%u</div> (<div class='val'>0x%" PRIxLEAST32 "</div>)</summary></details>\n", props.driverVersion, props.driverVersion );
		fprintf( out, "\t\t\t\t\t\t<details><summary>vendorID = <div class='val'>0x%04x</div></summary></details>\n", props.vendorID );
		fprintf( out, "\t\t\t\t\t\t<details><summary>deviceID = <div class='val'>0x%04x</div></summary></details>\n", props.deviceID );
		fprintf( out, "\t\t\t\t\t\t<details><summary>deviceType = %s</summary></details>\n", VkPhysicalDeviceTypeString( props.deviceType ).c_str() );
		fprintf( out, "\t\t\t\t\t\t<details><summary>deviceName = %s</summary></details>\n", props.deviceName.c_str() );
		fprintf( out, "\t\t\t\t\t</details>\n" );
	}
	else if ( human_readable_output )
	{
		printf( "VkPhysicalDeviceProperties:\n" );
		printf( "===========================\n" );
		printf( "\tapiVersion     = 0x%" PRIxLEAST32 "  (%d.%d.%d)\n", apiVersion, major, minor, patch );
		printf( "\tdriverVersion  = %u (0x%" PRIxLEAST32 ")\n", props.driverVersion, props.driverVersion );
		printf( "\tvendorID       = 0x%04x\n", props.vendorID );
		printf( "\tdeviceID       = 0x%04x\n", props.deviceID );
		printf( "\tdeviceType     = %s\n", VkPhysicalDeviceTypeString( props.deviceType ).c_str() );
		printf( "\tdeviceName     = %s\n", props.deviceName.c_str() );
	}
	if ( json_output )
	{
		printf( ",\n" );
		printf( "\t\"VkPhysicalDeviceProperties\": {\n" );
		printf( "\t\t\"apiVersion\": %u,\n", apiVersion );
		printf( "\t\t\"driverVersion\": %u,\n", props.driverVersion );
		printf( "\t\t\"vendorID\": %u,\n", props.vendorID );
		printf( "\t\t\"deviceID\": %u,\n", props.deviceID );
		printf( "\t\t\"deviceType\": %u,\n", int( props.deviceType ) );
		printf( "\t\t\"deviceName\": \"%s\",\n", props.deviceName.c_str() );
		printf( "\t\t\"pipelineCacheUUID\": [" );
		for ( uint32_t i = 0; i < ashes::UuidSize; ++i )
		{
			if ( i > 0 )
			{
				printf( "," );
			}
			printf( "\n" );
			printf( "\t\t\t%u", props.pipelineCacheUUID[i] );
		}
		printf( "\n" );
		printf( "\t\t]" );
	}

#if 0
	if ( CheckExtensionEnabled( VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, gpu->inst->inst_extensions,
		gpu->inst->inst_extensions_count ) )
	{
		AppDumpLimits( gpu->props2.properties.limits, out );
	}
	else
#endif
	{
		AppDumpLimits( gpu->props.limits, out );
	}

#if 0
	if ( CheckExtensionEnabled( VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, gpu->inst->inst_extensions,
		gpu->inst->inst_extensions_count ) )
	{
		AppDumpSparseProps( &gpu->props2.properties.sparseProperties, out );
	}
	else
#endif
	{
		AppDumpSparseProps( gpu->props.sparseProperties, out );
	}

	if ( json_output )
	{
		printf( "\n\t}" );
	}
#if 0

	if ( CheckExtensionEnabled( VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, gpu->inst->inst_extensions,
		gpu->inst->inst_extensions_count ) )
	{
		void *place = gpu->props2.pNext;
		while ( place )
		{
			struct VkStructureHeader *structure = ( struct VkStructureHeader * )place;
			if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_PROPERTIES_EXT &&
				CheckPhysicalDeviceExtensionIncluded( VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT *blend_op_adv_props =
					( VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT * )structure;
				if ( html_output )
				{
					fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDeviceBlendOperationAdvancedProperties</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>advancedBlendMaxColorAttachments                = <div "
						"class='val'>%u</div></summary></details>\n",
						blend_op_adv_props->advancedBlendMaxColorAttachments );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>advancedBlendIndependentBlend                   = <div "
						"class='val'>%u</div></summary></details>\n",
						blend_op_adv_props->advancedBlendIndependentBlend );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>advancedBlendNonPremultipliedSrcColor           = <div "
						"class='val'>%u</div></summary></details>\n",
						blend_op_adv_props->advancedBlendNonPremultipliedSrcColor );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>advancedBlendNonPremultipliedDstColor           = <div "
						"class='val'>%u</div></summary></details>\n",
						blend_op_adv_props->advancedBlendNonPremultipliedDstColor );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>advancedBlendCorrelatedOverlap                  = <div "
						"class='val'>%u</div></summary></details>\n",
						blend_op_adv_props->advancedBlendCorrelatedOverlap );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>advancedBlendAllOperations                      = <div "
						"class='val'>%u</div></summary></details>\n",
						blend_op_adv_props->advancedBlendAllOperations );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceBlendOperationAdvancedProperties:\n" );
					printf( "=================================================\n" );
					printf( "\tadvancedBlendMaxColorAttachments               = %u\n",
						blend_op_adv_props->advancedBlendMaxColorAttachments );
					printf( "\tadvancedBlendIndependentBlend                  = %u\n",
						blend_op_adv_props->advancedBlendIndependentBlend );
					printf( "\tadvancedBlendNonPremultipliedSrcColor          = %u\n",
						blend_op_adv_props->advancedBlendNonPremultipliedSrcColor );
					printf( "\tadvancedBlendNonPremultipliedDstColor          = %u\n",
						blend_op_adv_props->advancedBlendNonPremultipliedDstColor );
					printf( "\tadvancedBlendCorrelatedOverlap                 = %u\n",
						blend_op_adv_props->advancedBlendCorrelatedOverlap );
					printf( "\tadvancedBlendAllOperations                     = %u\n",
						blend_op_adv_props->advancedBlendAllOperations );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_POINT_CLIPPING_PROPERTIES_KHR &&
				CheckPhysicalDeviceExtensionIncluded( VK_KHR_MAINTENANCE2_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDevicePointClippingPropertiesKHR *pt_clip_props = ( VkPhysicalDevicePointClippingPropertiesKHR * )structure;
				if ( html_output )
				{
					fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDevicePointClippingProperties</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>pointClippingBehavior               = <div "
						"class='val'>%u</div></summary></details>\n",
						pt_clip_props->pointClippingBehavior );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDevicePointClippingProperties:\n" );
					printf( "========================================\n" );
					printf( "\tpointClippingBehavior               = %u\n", pt_clip_props->pointClippingBehavior );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES_KHR &&
				CheckPhysicalDeviceExtensionIncluded( VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDevicePushDescriptorPropertiesKHR *push_desc_props =
					( VkPhysicalDevicePushDescriptorPropertiesKHR * )structure;
				if ( html_output )
				{
					fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDevicePushDescriptorProperties</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>maxPushDescriptors                = <div "
						"class='val'>%u</div></summary></details>\n",
						push_desc_props->maxPushDescriptors );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDevicePushDescriptorProperties:\n" );
					printf( "=========================================\n" );
					printf( "\tmaxPushDescriptors               = %u\n", push_desc_props->maxPushDescriptors );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DISCARD_RECTANGLE_PROPERTIES_EXT &&
				CheckPhysicalDeviceExtensionIncluded( VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDeviceDiscardRectanglePropertiesEXT *discard_rect_props =
					( VkPhysicalDeviceDiscardRectanglePropertiesEXT * )structure;
				if ( html_output )
				{
					fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDeviceDiscardRectangleProperties</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>maxDiscardRectangles               = <div "
						"class='val'>%u</div></summary></details>\n",
						discard_rect_props->maxDiscardRectangles );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceDiscardRectangleProperties:\n" );
					printf( "===========================================\n" );
					printf( "\tmaxDiscardRectangles               = %u\n", discard_rect_props->maxDiscardRectangles );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES_KHR &&
				CheckPhysicalDeviceExtensionIncluded( VK_KHR_MULTIVIEW_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDeviceMultiviewPropertiesKHR *multiview_props = ( VkPhysicalDeviceMultiviewPropertiesKHR * )structure;
				if ( html_output )
				{
					fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDeviceMultiviewProperties</summary>\n" );
					fprintf(
						out,
						"\t\t\t\t\t\t<details><summary>maxMultiviewViewCount     = <div class='val'>%u</div></summary></details>\n",
						multiview_props->maxMultiviewViewCount );
					fprintf(
						out,
						"\t\t\t\t\t\t<details><summary>maxMultiviewInstanceIndex = <div class='val'>%u</div></summary></details>\n",
						multiview_props->maxMultiviewInstanceIndex );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceMultiviewProperties:\n" );
					printf( "====================================\n" );
					printf( "\tmaxMultiviewViewCount     = %u\n", multiview_props->maxMultiviewViewCount );
					printf( "\tmaxMultiviewInstanceIndex = %u\n", multiview_props->maxMultiviewInstanceIndex );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES_KHR )
			{
				VkPhysicalDeviceMaintenance3PropertiesKHR *maintenance3_props =
					( VkPhysicalDeviceMaintenance3PropertiesKHR * )structure;
				if ( html_output )
				{
					fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDeviceMaintenance3Properties</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>maxPerSetDescriptors    = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						maintenance3_props->maxPerSetDescriptors );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>maxMemoryAllocationSize = <div class='val'>%" PRIuLEAST64
						"</div></summary></details>\n",
						maintenance3_props->maxMemoryAllocationSize );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceMaintenance3Properties:\n" );
					printf( "=======================================\n" );
					printf( "\tmaxPerSetDescriptors    = %" PRIuLEAST32 "\n", maintenance3_props->maxPerSetDescriptors );
					printf( "\tmaxMemoryAllocationSize = %" PRIuLEAST64 "\n", maintenance3_props->maxMemoryAllocationSize );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES_KHR )
			{
				const VkPhysicalDeviceIDPropertiesKHR *id_props = ( VkPhysicalDeviceIDPropertiesKHR * )structure;
				if ( html_output )
				{
					fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDeviceIDProperties</summary>\n" );
					// Visual Studio 2013's printf does not support the "hh"
					// length modifier so cast the operands and use field width
					// "2" to fake it.
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>deviceUUID      = <div "
						"class='val'>%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x</div></summary></"
						"details>\n",
						( uint32_t )id_props->deviceUUID[0], ( uint32_t )id_props->deviceUUID[1], ( uint32_t )id_props->deviceUUID[2],
						( uint32_t )id_props->deviceUUID[3], ( uint32_t )id_props->deviceUUID[4], ( uint32_t )id_props->deviceUUID[5],
						( uint32_t )id_props->deviceUUID[6], ( uint32_t )id_props->deviceUUID[7], ( uint32_t )id_props->deviceUUID[8],
						( uint32_t )id_props->deviceUUID[9], ( uint32_t )id_props->deviceUUID[10],
						( uint32_t )id_props->deviceUUID[11], ( uint32_t )id_props->deviceUUID[12],
						( uint32_t )id_props->deviceUUID[13], ( uint32_t )id_props->deviceUUID[14],
						( uint32_t )id_props->deviceUUID[15] );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>driverUUID      = <div "
						"class='val'>%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x</div></summary></"
						"details>\n",
						( uint32_t )id_props->driverUUID[0], ( uint32_t )id_props->driverUUID[1], ( uint32_t )id_props->driverUUID[2],
						( uint32_t )id_props->driverUUID[3], ( uint32_t )id_props->driverUUID[4], ( uint32_t )id_props->driverUUID[5],
						( uint32_t )id_props->driverUUID[6], ( uint32_t )id_props->driverUUID[7], ( uint32_t )id_props->driverUUID[8],
						( uint32_t )id_props->driverUUID[9], ( uint32_t )id_props->driverUUID[10],
						( uint32_t )id_props->driverUUID[11], ( uint32_t )id_props->driverUUID[12],
						( uint32_t )id_props->driverUUID[13], ( uint32_t )id_props->driverUUID[14],
						( uint32_t )id_props->driverUUID[15] );
					fprintf( out, "\t\t\t\t\t\t<details><summary>deviceLUIDValid = <div class='val'>%s</div></summary></details>\n",
						id_props->deviceLUIDValid ? "true" : "false" );
					if ( id_props->deviceLUIDValid )
					{
						fprintf( out,
							"\t\t\t\t\t\t<details><summary>deviceLUID      = <div "
							"class='val'>%02x%02x%02x%02x-%02x%02x%02x%02x</div></summary></details>\n",
							( uint32_t )id_props->deviceLUID[0], ( uint32_t )id_props->deviceLUID[1],
							( uint32_t )id_props->deviceLUID[2], ( uint32_t )id_props->deviceLUID[3],
							( uint32_t )id_props->deviceLUID[4], ( uint32_t )id_props->deviceLUID[5],
							( uint32_t )id_props->deviceLUID[6], ( uint32_t )id_props->deviceLUID[7] );
						fprintf(
							out,
							"\t\t\t\t\t\t<details><summary>deviceNodeMask  = <div class='val'>0x%08x</div></summary></details>\n",
							id_props->deviceNodeMask );
					}
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceIDProperties:\n" );
					printf( "=========================================\n" );
					printf( "\tdeviceUUID      = %02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
						( uint32_t )id_props->deviceUUID[0], ( uint32_t )id_props->deviceUUID[1], ( uint32_t )id_props->deviceUUID[2],
						( uint32_t )id_props->deviceUUID[3], ( uint32_t )id_props->deviceUUID[4], ( uint32_t )id_props->deviceUUID[5],
						( uint32_t )id_props->deviceUUID[6], ( uint32_t )id_props->deviceUUID[7], ( uint32_t )id_props->deviceUUID[8],
						( uint32_t )id_props->deviceUUID[9], ( uint32_t )id_props->deviceUUID[10],
						( uint32_t )id_props->deviceUUID[11], ( uint32_t )id_props->deviceUUID[12],
						( uint32_t )id_props->deviceUUID[13], ( uint32_t )id_props->deviceUUID[14],
						( uint32_t )id_props->deviceUUID[15] );
					printf( "\tdriverUUID      = %02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
						( uint32_t )id_props->driverUUID[0], ( uint32_t )id_props->driverUUID[1], ( uint32_t )id_props->driverUUID[2],
						( uint32_t )id_props->driverUUID[3], ( uint32_t )id_props->driverUUID[4], ( uint32_t )id_props->driverUUID[5],
						( uint32_t )id_props->driverUUID[6], ( uint32_t )id_props->driverUUID[7], ( uint32_t )id_props->driverUUID[8],
						( uint32_t )id_props->driverUUID[9], ( uint32_t )id_props->driverUUID[10],
						( uint32_t )id_props->driverUUID[11], ( uint32_t )id_props->driverUUID[12],
						( uint32_t )id_props->driverUUID[13], ( uint32_t )id_props->driverUUID[14],
						( uint32_t )id_props->driverUUID[15] );
					printf( "\tdeviceLUIDValid = %s\n", id_props->deviceLUIDValid ? "true" : "false" );
					if ( id_props->deviceLUIDValid )
					{
						printf( "\tdeviceLUID      = %02x%02x%02x%02x-%02x%02x%02x%02x\n", ( uint32_t )id_props->deviceLUID[0],
							( uint32_t )id_props->deviceLUID[1], ( uint32_t )id_props->deviceLUID[2],
							( uint32_t )id_props->deviceLUID[3], ( uint32_t )id_props->deviceLUID[4],
							( uint32_t )id_props->deviceLUID[5], ( uint32_t )id_props->deviceLUID[6],
							( uint32_t )id_props->deviceLUID[7] );
						printf( "\tdeviceNodeMask  = 0x%08x\n", id_props->deviceNodeMask );
					}
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES_KHR &&
				CheckPhysicalDeviceExtensionIncluded( VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDeviceDriverPropertiesKHR *driver_props = ( VkPhysicalDeviceDriverPropertiesKHR * )structure;
				if ( html_output )
				{
					fprintf( out, "\n\t\t\t\t\t<details><summary>VkPhysicalDeviceDriverProperties</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>driverID   = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						driver_props->driverID );
					fprintf( out, "\t\t\t\t\t\t<details><summary>driverName = %s</summary></details>\n", driver_props->driverName );
					fprintf( out, "\t\t\t\t\t\t<details><summary>driverInfo = %s</summary></details>\n", driver_props->driverInfo );
					fprintf( out, "\t\t\t\t\t\t<details><summary>conformanceVersion:</summary></details>\n" );
					fprintf( out,
						"\t\t\t\t\t\t\t<details><summary>major    = <div class='val'>%" PRIuLEAST8
						"</div></summary></details>\n",
						driver_props->conformanceVersion.major );
					fprintf( out,
						"\t\t\t\t\t\t\t<details><summary>minor    = <div class='val'>%" PRIuLEAST8
						"</div></summary></details>\n",
						driver_props->conformanceVersion.minor );
					fprintf( out,
						"\t\t\t\t\t\t\t<details><summary>subminor = <div class='val'>%" PRIuLEAST8
						"</div></summary></details>\n",
						driver_props->conformanceVersion.subminor );
					fprintf( out,
						"\t\t\t\t\t\t\t<details><summary>patch    = <div class='val'>%" PRIuLEAST8
						"</div></summary></details>\n",
						driver_props->conformanceVersion.patch );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceDriverProperties:\n" );
					printf( "=================================\n" );
					printf( "\tdriverID   = %" PRIuLEAST32 "\n", driver_props->driverID );
					printf( "\tdriverName = %s\n", driver_props->driverName );
					printf( "\tdriverInfo = %s\n", driver_props->driverInfo );
					printf( "\tconformanceVersion:\n" );
					printf( "\t\tmajor    = %" PRIuLEAST8 "\n", driver_props->conformanceVersion.major );
					printf( "\t\tminor    = %" PRIuLEAST8 "\n", driver_props->conformanceVersion.minor );
					printf( "\t\tsubminor = %" PRIuLEAST8 "\n", driver_props->conformanceVersion.subminor );
					printf( "\t\tpatch    = %" PRIuLEAST8 "\n", driver_props->conformanceVersion.patch );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT_CONTROLS_PROPERTIES_KHR &&
				CheckPhysicalDeviceExtensionIncluded( VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDeviceFloatControlsPropertiesKHR *float_control_props =
					( VkPhysicalDeviceFloatControlsPropertiesKHR * )structure;
				if ( html_output )
				{
					fprintf( out, "\n\t\t\t\t\t<details><summary>VkPhysicalDeviceFloatControlsProperties</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>separateDenormSettings       = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_control_props->separateDenormSettings );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>separateRoundingModeSettings = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_control_props->separateRoundingModeSettings );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderSignedZeroInfNanPreserveFloat16 = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_control_props->shaderSignedZeroInfNanPreserveFloat16 );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderSignedZeroInfNanPreserveFloat32 = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_control_props->shaderSignedZeroInfNanPreserveFloat32 );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderSignedZeroInfNanPreserveFloat64 = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_control_props->shaderSignedZeroInfNanPreserveFloat64 );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderDenormPreserveFloat16           = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_control_props->shaderDenormPreserveFloat16 );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderDenormPreserveFloat32           = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_control_props->shaderDenormPreserveFloat32 );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderDenormPreserveFloat64           = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_control_props->shaderDenormPreserveFloat64 );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderDenormFlushToZeroFloat16        = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_control_props->shaderDenormFlushToZeroFloat16 );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderDenormFlushToZeroFloat32        = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_control_props->shaderDenormFlushToZeroFloat32 );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderDenormFlushToZeroFloat64        = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_control_props->shaderDenormFlushToZeroFloat64 );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderRoundingModeRTEFloat16          = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_control_props->shaderRoundingModeRTEFloat16 );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderRoundingModeRTEFloat32          = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_control_props->shaderRoundingModeRTEFloat32 );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderRoundingModeRTEFloat64          = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_control_props->shaderRoundingModeRTEFloat64 );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderRoundingModeRTZFloat16          = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_control_props->shaderRoundingModeRTZFloat16 );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderRoundingModeRTZFloat32          = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_control_props->shaderRoundingModeRTZFloat32 );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>shaderRoundingModeRTZFloat64          = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						float_control_props->shaderRoundingModeRTZFloat64 );
					fprintf( out, "\t\t\t\t\t</details>\n" );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceFloatControlsProperties:\n" );
					printf( "========================================\n" );
					printf( "\tseparateDenormSettings       = %" PRIuLEAST32 "\n", float_control_props->separateDenormSettings );
					printf( "\tseparateRoundingModeSettings = %" PRIuLEAST32 "\n",
						float_control_props->separateRoundingModeSettings );
					printf( "\tshaderSignedZeroInfNanPreserveFloat16 = %" PRIuLEAST32 "\n",
						float_control_props->shaderSignedZeroInfNanPreserveFloat16 );
					printf( "\tshaderSignedZeroInfNanPreserveFloat32 = %" PRIuLEAST32 "\n",
						float_control_props->shaderSignedZeroInfNanPreserveFloat32 );
					printf( "\tshaderSignedZeroInfNanPreserveFloat64 = %" PRIuLEAST32 "\n",
						float_control_props->shaderSignedZeroInfNanPreserveFloat64 );
					printf( "\tshaderDenormPreserveFloat16          = %" PRIuLEAST32 "\n",
						float_control_props->shaderDenormPreserveFloat16 );
					printf( "\tshaderDenormPreserveFloat32           = %" PRIuLEAST32 "\n",
						float_control_props->shaderDenormPreserveFloat32 );
					printf( "\tshaderDenormPreserveFloat64           = %" PRIuLEAST32 "\n",
						float_control_props->shaderDenormPreserveFloat64 );
					printf( "\tshaderDenormFlushToZeroFloat16        = %" PRIuLEAST32 "\n",
						float_control_props->shaderDenormFlushToZeroFloat16 );
					printf( "\tshaderDenormFlushToZeroFloat32        = %" PRIuLEAST32 "\n",
						float_control_props->shaderDenormFlushToZeroFloat32 );
					printf( "\tshaderDenormFlushToZeroFloat64        = %" PRIuLEAST32 "\n",
						float_control_props->shaderDenormFlushToZeroFloat64 );
					printf( "\tshaderRoundingModeRTEFloat16          = %" PRIuLEAST32 "\n",
						float_control_props->shaderRoundingModeRTEFloat16 );
					printf( "\tshaderRoundingModeRTEFloat32          = %" PRIuLEAST32 "\n",
						float_control_props->shaderRoundingModeRTEFloat32 );
					printf( "\tshaderRoundingModeRTEFloat64         = %" PRIuLEAST32 "\n",
						float_control_props->shaderRoundingModeRTEFloat64 );
					printf( "\tshaderRoundingModeRTZFloat16          = %" PRIuLEAST32 "\n",
						float_control_props->shaderRoundingModeRTZFloat16 );
					printf( "\tshaderRoundingModeRTZFloat32          = %" PRIuLEAST32 "\n",
						float_control_props->shaderRoundingModeRTZFloat32 );
					printf( "\tshaderRoundingModeRTZFloat64          = %" PRIuLEAST32 "\n",
						float_control_props->shaderRoundingModeRTZFloat64 );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PCI_BUS_INFO_PROPERTIES_EXT &&
				CheckPhysicalDeviceExtensionIncluded( VK_EXT_PCI_BUS_INFO_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDevicePCIBusInfoPropertiesEXT *pci_bus_properties = ( VkPhysicalDevicePCIBusInfoPropertiesEXT * )structure;
				if ( html_output )
				{
					fprintf( out, "\n\t\t\t\t\t<details><summary>VkPhysicalDevicePCIBusInfoProperties</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>pciDomain   = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						pci_bus_properties->pciDomain );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>pciBus      = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						pci_bus_properties->pciBus );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>pciDevice   = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						pci_bus_properties->pciDevice );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>pciFunction = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						pci_bus_properties->pciFunction );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDevicePCIBusInfoProperties\n" );
					printf( "====================================\n" );
					printf( "\tpciDomain   = %" PRIuLEAST32 "\n", pci_bus_properties->pciDomain );
					printf( "\tpciBus      = %" PRIuLEAST32 "\n", pci_bus_properties->pciBus );
					printf( "\tpciDevice   = %" PRIuLEAST32 "\n", pci_bus_properties->pciDevice );
					printf( "\tpciFunction = %" PRIuLEAST32 "\n", pci_bus_properties->pciFunction );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_PROPERTIES_EXT &&
				CheckPhysicalDeviceExtensionIncluded( VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDeviceTransformFeedbackPropertiesEXT *transform_feedback_properties =
					( VkPhysicalDeviceTransformFeedbackPropertiesEXT * )structure;
				if ( html_output )
				{
					fprintf( out, "\n\t\t\t\t\t<details><summary>VkPhysicalDeviceTransformFeedbackProperties</summary>\n" );
					fprintf(
						out,
						"\t\t\t\t\t\t<details><summary>maxTransformFeedbackStreams                = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						transform_feedback_properties->maxTransformFeedbackStreams );
					fprintf(
						out,
						"\t\t\t\t\t\t<details><summary>maxTransformFeedbackBuffers                = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						transform_feedback_properties->maxTransformFeedbackBuffers );
					fprintf(
						out,
						"\t\t\t\t\t\t<details><summary>maxTransformFeedbackBufferSize             = <div class='val'>%" PRIuLEAST64
						"</div></summary></details>\n",
						transform_feedback_properties->maxTransformFeedbackBufferSize );
					fprintf(
						out,
						"\t\t\t\t\t\t<details><summary>maxTransformFeedbackStreamDataSize         = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						transform_feedback_properties->maxTransformFeedbackStreamDataSize );
					fprintf(
						out,
						"\t\t\t\t\t\t<details><summary>maxTransformFeedbackBufferDataSize         = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						transform_feedback_properties->maxTransformFeedbackBufferDataSize );
					fprintf(
						out,
						"\t\t\t\t\t\t<details><summary>maxTransformFeedbackBufferDataStride       = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						transform_feedback_properties->maxTransformFeedbackBufferDataStride );
					fprintf(
						out,
						"\t\t\t\t\t\t<details><summary>transformFeedbackQueries                   = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						transform_feedback_properties->transformFeedbackQueries );
					fprintf(
						out,
						"\t\t\t\t\t\t<details><summary>transformFeedbackStreamsLinesTriangles     = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						transform_feedback_properties->transformFeedbackStreamsLinesTriangles );
					fprintf(
						out,
						"\t\t\t\t\t\t<details><summary>transformFeedbackRasterizationStreamSelect = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						transform_feedback_properties->transformFeedbackRasterizationStreamSelect );
					fprintf(
						out,
						"\t\t\t\t\t\t<details><summary>transformFeedbackDraw                      = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						transform_feedback_properties->transformFeedbackDraw );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceTransformFeedbackProperties\n" );
					printf( "===========================================\n" );
					printf( "\tmaxTransformFeedbackStreams                = %" PRIuLEAST32 "\n",
						transform_feedback_properties->maxTransformFeedbackStreams );
					printf( "\tmaxTransformFeedbackBuffers                = %" PRIuLEAST32 "\n",
						transform_feedback_properties->maxTransformFeedbackBuffers );
					printf( "\tmaxTransformFeedbackBufferSize             = %" PRIuLEAST64 "\n",
						transform_feedback_properties->maxTransformFeedbackBufferSize );
					printf( "\tmaxTransformFeedbackStreamDataSize         = %" PRIuLEAST32 "\n",
						transform_feedback_properties->maxTransformFeedbackStreamDataSize );
					printf( "\tmaxTransformFeedbackBufferDataSize         = %" PRIuLEAST32 "\n",
						transform_feedback_properties->maxTransformFeedbackBufferDataSize );
					printf( "\tmaxTransformFeedbackBufferDataStride       = %" PRIuLEAST32 "\n",
						transform_feedback_properties->maxTransformFeedbackBufferDataStride );
					printf( "\ttransformFeedbackQueries                   = %" PRIuLEAST32 "\n",
						transform_feedback_properties->transformFeedbackQueries );
					printf( "\ttransformFeedbackStreamsLinesTriangles     = %" PRIuLEAST32 "\n",
						transform_feedback_properties->transformFeedbackStreamsLinesTriangles );
					printf( "\ttransformFeedbackRasterizationStreamSelect = %" PRIuLEAST32 "\n",
						transform_feedback_properties->transformFeedbackRasterizationStreamSelect );
					printf( "\ttransformFeedbackDraw                      = %" PRIuLEAST32 "\n",
						transform_feedback_properties->transformFeedbackDraw );
				}
			}
			else if ( structure->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_PROPERTIES_EXT &&
				CheckPhysicalDeviceExtensionIncluded( VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME, gpu->device_extensions,
					gpu->device_extension_count ) )
			{
				VkPhysicalDeviceFragmentDensityMapPropertiesEXT *fragment_density_map_properties =
					( VkPhysicalDeviceFragmentDensityMapPropertiesEXT * )structure;
				if ( html_output )
				{
					fprintf( out, "\n\t\t\t\t\t<details><summary>VkPhysicalDeviceFragmentDensityMapProperties</summary>\n" );
					fprintf( out, "\t\t\t\t\t\t<details><summary>minFragmentDensityTexelSize</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t\t<details><summary>width = <div class='val'>%" PRIuLEAST32 "</div></summary></details>\n",
						fragment_density_map_properties->minFragmentDensityTexelSize.width );
					fprintf( out,
						"\t\t\t\t\t\t\t<details><summary>height = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						fragment_density_map_properties->minFragmentDensityTexelSize.height );
					fprintf( out, "\t\t\t\t\t\t<details><summary>maxFragmentDensityTexelSize</summary>\n" );
					fprintf( out,
						"\t\t\t\t\t\t\t<details><summary>width = <div class='val'>%" PRIuLEAST32 "</div></summary></details>\n",
						fragment_density_map_properties->maxFragmentDensityTexelSize.width );
					fprintf( out,
						"\t\t\t\t\t\t\t<details><summary>height = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						fragment_density_map_properties->maxFragmentDensityTexelSize.height );
					fprintf( out,
						"\t\t\t\t\t\t<details><summary>fragmentDensityInvocations = <div class='val'>%" PRIuLEAST32
						"</div></summary></details>\n",
						fragment_density_map_properties->fragmentDensityInvocations );
				}
				else if ( human_readable_output )
				{
					printf( "\nVkPhysicalDeviceFragmentDensityMapProperties\n" );
					printf( "============================================\n" );
					printf( "\tminFragmentDensityTexelSize\n" );
					printf( "\t\twidth = %" PRIuLEAST32 "\n", fragment_density_map_properties->minFragmentDensityTexelSize.width );
					printf( "\t\theight = %" PRIuLEAST32 "\n", fragment_density_map_properties->minFragmentDensityTexelSize.height );
					printf( "\tmaxFragmentDensityTexelSize\n" );
					printf( "\t\twidth = %" PRIuLEAST32 "\n", fragment_density_map_properties->maxFragmentDensityTexelSize.width );
					printf( "\t\theight = %" PRIuLEAST32 "\n", fragment_density_map_properties->maxFragmentDensityTexelSize.height );
					printf( "\tfragmentDensityInvocations = %" PRIuLEAST32 "\n",
						fragment_density_map_properties->fragmentDensityInvocations );
				}
			}
			place = structure->pNext;
		}
	}
#endif
	fflush( out );
	fflush( stdout );
}

// Compare function for sorting extensions by name
static int CompareExtensionName( const  ashes::ExtensionProperties & a
	, const  ashes::ExtensionProperties & b )
{
	auto lhs = a.extensionName;
	auto rhs = b.extensionName;
	return lhs < rhs;
}

// Compare function for sorting layers by name
static int CompareLayerName( const LayerExtensionList & a, const LayerExtensionList & b )
{
	auto lhs = a.layer_properties.layerName;
	auto rhs = b.layer_properties.layerName;
	return lhs < rhs;
}

static void AppDumpExtensions( const char *indent
	, const char *layer_name
	, std::vector< ashes::ExtensionProperties > & extension_properties
	, FILE *out )
{
	auto extension_count = uint32_t( extension_properties.size() );

	if ( html_output )
	{
		fprintf( out, "\t\t\t%s<details><summary>", indent );
	}
	if ( layer_name && ( strlen( layer_name ) > 0 ) )
	{
		if ( html_output )
		{
			fprintf( out, "%s Extensions", layer_name );
		}
		else if ( human_readable_output )
		{
			printf( "%s%s Extensions", indent, layer_name );
		}
	}
	else
	{
		if ( html_output )
		{
			fprintf( out, "Extensions" );
		}
		else if ( human_readable_output )
		{
			printf( "%sExtensions", indent );
		}
	}
	if ( html_output )
	{
		fprintf( out, "\tcount = <div class='val'>%d</div></summary>", extension_count );
		if ( extension_count > 0 )
		{
			fprintf( out, "\n" );
		}
	}
	else if ( human_readable_output )
	{
		printf( "\tcount = %d\n", extension_count );
	}

	const bool is_device_type = strcmp( layer_name, "Device" ) == 0;
	if ( is_device_type && json_output )
	{
		printf( ",\n" );
		printf( "\t\"ArrayOfVkExtensionProperties\": [" );
	}

	std::sort( extension_properties.begin()
		, extension_properties.end()
		, CompareExtensionName );

	uint32_t i = 0;
	for ( auto & ext_prop : extension_properties )
	{
		if ( html_output )
		{
			fprintf( out, "\t\t\t\t%s<details><summary>", indent );
			fprintf( out, "<div class='type'>%s</div>: extension revision <div class='val'>%d</div>", ext_prop.extensionName.c_str(),
				ext_prop.specVersion );
			fprintf( out, "</summary></details>\n" );
		}
		else if ( human_readable_output )
		{
			printf( "%s\t", indent );
			printf( "%-36s: extension revision %2d\n", ext_prop.extensionName.c_str(), ext_prop.specVersion );
		}
		if ( is_device_type && json_output )
		{
			if ( i > 0 )
			{
				printf( "," );
			}
			printf( "\n" );
			printf( "\t\t{\n" );
			printf( "\t\t\t\"extensionName\": \"%s\",\n", ext_prop.extensionName.c_str() );
			printf( "\t\t\t\"specVersion\": %u\n", ext_prop.specVersion );
			printf( "\t\t}" );
		}
		++i;
	}
	if ( html_output )
	{
		if ( extension_count > 0 )
		{
			fprintf( out, "\t\t\t%s</details>\n", indent );
		}
		else
		{
			fprintf( out, "</details>\n" );
		}
	}
	if ( is_device_type && json_output )
	{
		if ( extension_count > 0 )
		{
			printf( "\n\t" );
		}
		printf( "]" );
	}

	fflush( out );
	fflush( stdout );
}

static void AppGpuDumpQueueProps( ashes::QueueFamilyProperties const & props
	, uint32_t id
	, FILE *out )
{
	if ( html_output )
	{
		fprintf( out, "\t\t\t\t\t<details><summary>VkQueueFamilyProperties[<div class='val'>%d</div>]</summary>\n", id );
		fprintf( out, "\t\t\t\t\t\t<details><summary>queueFlags = " );
	}
	else if ( human_readable_output )
	{
		printf( "VkQueueFamilyProperties[%d]:\n", id );
		printf( "===========================\n" );
		printf( "\tqueueFlags         = " );
	}
	if ( html_output || human_readable_output )
	{
		std::string sep = "";  // separator character
		if ( ashes::checkFlag( props.queueFlags, ashes::QueueFlag::eGraphics ) )
		{
			fprintf( out, "GRAPHICS" );
			sep = " | ";
		}
		if ( ashes::checkFlag( props.queueFlags, ashes::QueueFlag::eCompute ) )
		{
			fprintf( out, "%sCOMPUTE", sep.c_str() );
			sep = " | ";
		}
		if ( ashes::checkFlag( props.queueFlags, ashes::QueueFlag::eTransfer ) )
		{
			fprintf( out, "%sTRANSFER", sep.c_str() );
			sep = " | ";
		}
		if ( ashes::checkFlag( props.queueFlags, ashes::QueueFlag::eSparseBinding ) )
		{
			fprintf( out, "%sSPARSE", sep.c_str() );
		}
	}

	if ( html_output )
	{
		fprintf( out, "</summary></details>\n" );
		fprintf( out, "\t\t\t\t\t\t<details><summary>queueCount         = <div class='val'>%u</div></summary></details>\n", props.queueCount );
		fprintf( out, "\t\t\t\t\t\t<details><summary>timestampValidBits = <div class='val'>%u</div></summary></details>\n", props.timestampValidBits );
		fprintf( out, "\t\t\t\t\t\t<details><summary>minImageTransferGranularity = (<div class='val'>%d</div>, <div class='val'>%d</div>, <div class='val'>%d</div>)</summary></details>\n",
			props.minImageTransferGranularity.width, props.minImageTransferGranularity.height, props.minImageTransferGranularity.depth );
		fprintf( out, "\t\t\t\t\t</details>\n" );
	}
	else if ( human_readable_output )
	{
		printf( "\n" );
		printf( "\tqueueCount         = %u\n", props.queueCount );
		printf( "\ttimestampValidBits = %u\n", props.timestampValidBits );
		printf( "\tminImageTransferGranularity = (%d, %d, %d)\n", props.minImageTransferGranularity.width,
			props.minImageTransferGranularity.height, props.minImageTransferGranularity.depth );
	}
	if ( json_output )
	{
		printf( "\t\t{\n" );
		printf( "\t\t\t\"minImageTransferGranularity\": {\n" );
		printf( "\t\t\t\t\"depth\": %u,\n", props.minImageTransferGranularity.depth );
		printf( "\t\t\t\t\"height\": %u,\n", props.minImageTransferGranularity.height );
		printf( "\t\t\t\t\"width\": %u\n", props.minImageTransferGranularity.width );
		printf( "\t\t\t},\n" );
		printf( "\t\t\t\"queueCount\": %u,\n", props.queueCount );
		printf( "\t\t\t\"queueFlags\": %u,\n", props.queueFlags.value() );
		printf( "\t\t\t\"timestampValidBits\": %u\n", props.timestampValidBits );
		printf( "\t\t}" );
	}

	fflush( out );
	fflush( stdout );
}

// This prints a number of bytes in a human-readable format according to prefixes of the International System of Quantities (ISQ),
// defined in ISO/IEC 80000. The prefixes used here are not SI prefixes, but rather the binary prefixes based on powers of 1024
// (kibi-, mebi-, gibi- etc.).
#define kBufferSize 32

static char *HumanReadable( const size_t sz )
{
	const char prefixes[] = "KMGTPEZY";
	char buf[kBufferSize];
	int which = -1;
	double result = ( double )sz;
	while ( result > 1024 && which < 7 )
	{
		result /= 1024;
		++which;
	}

	char unit[] = "\0i";
	if ( which >= 0 )
	{
		unit[0] = prefixes[which];
	}
	snprintf( buf, kBufferSize, "%.2f %sB", result, unit );
	return strndup( buf, kBufferSize );
}

static void AppGpuDumpMemoryProps( AppGpu const * gpu
	, FILE *out )
{
	ashes:: PhysicalDeviceMemoryProperties props;
#if 0
	if ( CheckExtensionEnabled( VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, gpu->inst->inst_extensions,
		gpu->inst->inst_extensions_count ) )
	{
		const VkPhysicalDeviceMemoryProperties *props2_const = &gpu->memory_props2.memoryProperties;
		props = *props2_const;
	}
	else
#endif
	{
		props = gpu->obj->getMemoryProperties();
	}

	auto memoryHeapCount = uint32_t( props.memoryHeaps.size() );
	auto memoryTypeCount = uint32_t( props.memoryTypes.size() );

	if ( html_output )
	{
		fprintf( out, "\t\t\t\t\t<details><summary>VkPhysicalDeviceMemoryProperties</summary>\n" );
		fprintf( out, "\t\t\t\t\t\t<details><summary>memoryHeapCount = <div class='val'>%u</div></summary>", memoryHeapCount );
		if ( memoryHeapCount > 0 )
		{
			fprintf( out, "\n" );
		}
	}
	else if ( human_readable_output )
	{
		printf( "VkPhysicalDeviceMemoryProperties:\n" );
		printf( "=================================\n" );
		printf( "\tmemoryHeapCount       = %u\n", memoryHeapCount );
	}
	if ( json_output )
	{
		printf( ",\n" );
		printf( "\t\"VkPhysicalDeviceMemoryProperties\": {\n" );
		printf( "\t\t\"memoryHeaps\": [" );
	}
	for ( uint32_t i = 0; i < memoryHeapCount; ++i )
	{
		const size_t memSize = size_t( props.memoryHeaps[i].size );
		char *mem_size_human_readable = HumanReadable( memSize );

		if ( html_output )
		{
			fprintf( out, "\t\t\t\t\t\t\t<details><summary>memoryHeaps[<div class='val'>%u</div>]</summary>\n", i );
			fprintf( out, "\t\t\t\t\t\t\t\t<details><summary>size = <div class='val'>" PRINTF_SIZE_T_SPECIFIER "</div> (<div class='val'>0x%" PRIxLEAST64 "</div>) (<div class='val'>%s</div>)</summary></details>\n",
				( size_t )memSize, memSize, mem_size_human_readable );
		}
		else if ( human_readable_output )
		{
			printf( "\tmemoryHeaps[%u] :\n", i );
			printf( "\t\tsize          = " PRINTF_SIZE_T_SPECIFIER " (0x%" PRIxLEAST64 ") (%s)\n", ( size_t )memSize, memSize,
				mem_size_human_readable );
		}
		free( mem_size_human_readable );

		const ashes::MemoryHeapFlags heap_flags = props.memoryHeaps[i].flags;
		if ( html_output )
		{
			fprintf( out, "\t\t\t\t\t\t\t\t<details open><summary>flags</summary>\n" );
			fprintf( out, "\t\t\t\t\t\t\t\t\t<details><summary>" );
			fprintf( out, checkFlag( heap_flags, ashes::MemoryHeapFlag::eDeviceLocal ) ? "<div class='type'>VK_MEMORY_HEAP_DEVICE_LOCAL_BIT</div>" : "None" );
			fprintf( out, "</summary></details>\n" );
			fprintf( out, "\t\t\t\t\t\t\t\t</details>\n" );
			fprintf( out, "\t\t\t\t\t\t\t</details>\n" );
		}
		else if ( human_readable_output )
		{
			printf( "\t\tflags:\n\t\t\t" );
			printf( checkFlag( heap_flags, ashes::MemoryHeapFlag::eDeviceLocal ) ? "VK_MEMORY_HEAP_DEVICE_LOCAL_BIT\n" : "None\n" );
		}
		if ( json_output )
		{
			if ( i > 0 )
			{
				printf( "," );
			}
			printf( "\n" );
			printf( "\t\t\t{\n" );
			printf( "\t\t\t\t\"flags\": %u,\n", heap_flags.value() );
			printf( "\t\t\t\t\"size\": " PRINTF_SIZE_T_SPECIFIER "\n", ( size_t )memSize );
			printf( "\t\t\t}" );
		}
	}
	if ( html_output )
	{
		if ( memoryHeapCount > 0 )
		{
			fprintf( out, "\t\t\t\t\t\t" );
		}
		fprintf( out, "</details>\n" );
	}
	if ( json_output )
	{
		if ( memoryHeapCount > 0 )
		{
			printf( "\n\t\t" );
		}
		printf( "]" );
	}

	if ( html_output )
	{
		fprintf( out, "\t\t\t\t\t\t<details><summary>memoryTypeCount = <div class='val'>%u</div></summary>", memoryTypeCount );
		if ( memoryTypeCount > 0 )
		{
			fprintf( out, "\n" );
		}
	}
	else if ( human_readable_output )
	{
		printf( "\tmemoryTypeCount       = %u\n", memoryTypeCount );
	}
	if ( json_output )
	{
		printf( ",\n" );
		printf( "\t\t\"memoryTypes\": [" );
	}
	for ( uint32_t i = 0; i < memoryTypeCount; ++i )
	{
		if ( html_output )
		{
			fprintf( out, "\t\t\t\t\t\t\t<details><summary>memoryTypes[<div class='val'>%u</div>]</summary>\n", i );
			fprintf( out, "\t\t\t\t\t\t\t\t<details><summary>heapIndex = <div class='val'>%u</div></summary></details>\n", props.memoryTypes[i].heapIndex );
			fprintf( out, "\t\t\t\t\t\t\t\t<details open><summary>propertyFlags = <div class='val'>0x%" PRIxLEAST32 "</div></summary>", props.memoryTypes[i].propertyFlags.value() );
			if ( props.memoryTypes[i].propertyFlags == 0 )
			{
				fprintf( out, "</details>\n" );
			}
			else
			{
				fprintf( out, "\n" );
			}
		}
		else if ( human_readable_output )
		{
			printf( "\tmemoryTypes[%u] :\n", i );
			printf( "\t\theapIndex     = %u\n", props.memoryTypes[i].heapIndex );
			printf( "\t\tpropertyFlags = 0x%" PRIxLEAST32 ":\n", props.memoryTypes[i].propertyFlags.value() );
		}
		if ( json_output )
		{
			if ( i > 0 )
			{
				printf( "," );
			}
			printf( "\n" );
			printf( "\t\t\t{\n" );
			printf( "\t\t\t\t\"heapIndex\": %u,\n", props.memoryTypes[i].heapIndex );
			printf( "\t\t\t\t\"propertyFlags\": %u\n", props.memoryTypes[i].propertyFlags.value() );
			printf( "\t\t\t}" );
		}

		// Print each named flag to html or std output if it is set
		const auto flags = props.memoryTypes[i].propertyFlags;
		if ( html_output )
		{
			if ( checkFlag( flags, ashes::MemoryPropertyFlag::eDeviceLocal ) )     fprintf( out, "\t\t\t\t\t\t\t\t\t<details><summary><div class='type'>VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT</div></summary></details>\n" );
			if ( checkFlag( flags, ashes::MemoryPropertyFlag::eHostVisible ) )     fprintf( out, "\t\t\t\t\t\t\t\t\t<details><summary><div class='type'>VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT</div></summary></details>\n" );
			if ( checkFlag( flags, ashes::MemoryPropertyFlag::eHostCoherent ) )    fprintf( out, "\t\t\t\t\t\t\t\t\t<details><summary><div class='type'>VK_MEMORY_PROPERTY_HOST_COHERENT_BIT</div></summary></details>\n" );
			if ( checkFlag( flags, ashes::MemoryPropertyFlag::eHostCached ) )      fprintf( out, "\t\t\t\t\t\t\t\t\t<details><summary><div class='type'>VK_MEMORY_PROPERTY_HOST_CACHED_BIT</div></summary></details>\n" );
			if ( checkFlag( flags, ashes::MemoryPropertyFlag::eLazilyAllocated ) ) fprintf( out, "\t\t\t\t\t\t\t\t\t<details><summary><div class='type'>VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT</div></summary></details>\n" );
			if ( props.memoryTypes[i].propertyFlags > 0 )         fprintf( out, "\t\t\t\t\t\t\t\t</details>\n" );
			fprintf( out, "\t\t\t\t\t\t\t</details>\n" );
		}
		else if ( human_readable_output )
		{
			if ( checkFlag( flags, ashes::MemoryPropertyFlag::eDeviceLocal ) )     printf( "\t\t\tVK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT\n" );
			if ( checkFlag( flags, ashes::MemoryPropertyFlag::eHostVisible ) )     printf( "\t\t\tVK_MEMORY_PROPERTY_HOST_VISIBLE_BIT\n" );
			if ( checkFlag( flags, ashes::MemoryPropertyFlag::eHostCoherent ) )    printf( "\t\t\tVK_MEMORY_PROPERTY_HOST_COHERENT_BIT\n" );
			if ( checkFlag( flags, ashes::MemoryPropertyFlag::eHostCached ) )      printf( "\t\t\tVK_MEMORY_PROPERTY_HOST_CACHED_BIT\n" );
			if ( checkFlag( flags, ashes::MemoryPropertyFlag::eLazilyAllocated ) ) printf( "\t\t\tVK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT\n" );
		}
	}
	if ( html_output )
	{
		if ( memoryTypeCount > 0 )
		{
			fprintf( out, "\t\t\t\t\t\t" );
		}
		fprintf( out, "</details>\n" );
		fprintf( out, "\t\t\t\t\t</details>\n" );
	}
	if ( json_output )
	{
		if ( memoryTypeCount > 0 )
		{
			printf( "\n\t\t" );
		}
		printf( "]\n" );
		printf( "\t}" );
	}

	fflush( out );
	fflush( stdout );
}
// clang-format on

static void AppGpuDump( AppGpu * gpu
	, FILE *out )
{
	if ( html_output )
	{
		fprintf( out, "\t\t\t<details><summary>Device Properties and Extensions</summary>\n" );
		fprintf( out, "\t\t\t\t<details><summary>GPU%u</summary>\n", gpu->id );
	}
	else if ( human_readable_output )
	{
		printf( "\nDevice Properties and Extensions :\n" );
		printf( "==================================\n" );
		printf( "GPU%u\n", gpu->id );
	}

	AppGpuDumpProps( gpu, out );
	if ( html_output )
	{
		AppDumpExtensions( "\t\t", "Device", gpu->device_extensions, out );
	}
	else if ( human_readable_output )
	{
		printf( "\n" );
		AppDumpExtensions( "", "Device", gpu->device_extensions, out );
		printf( "\n" );
	}

	if ( json_output )
	{
		printf( ",\n" );
		printf( "\t\"ArrayOfVkQueueFamilyProperties\": [" );
	}
	auto queueProps = gpu->obj->getQueueFamilyProperties();
	for ( uint32_t i = 0; i < queueProps.size(); ++i )
	{
		if ( json_output )
		{
			if ( i > 0 )
			{
				printf( "," );
			}
			printf( "\n" );
		}
		AppGpuDumpQueueProps( queueProps[i], i, out );
		if ( human_readable_output )
		{
			printf( "\n" );
		}
	}
	if ( json_output )
	{
		if ( !queueProps.empty() )
		{
			printf( "\n\t" );
		}
		printf( "]" );
	}

	AppGpuDumpMemoryProps( gpu, out );
	if ( human_readable_output )
	{
		printf( "\n" );
	}

	AppGpuDumpFeatures( gpu->obj->getFeatures(), out );
	if ( human_readable_output )
	{
		printf( "\n" );
	}

	AppDevDump( gpu, out );
	if ( html_output )
	{
		fprintf( out, "\t\t\t\t</details>\n" );
		fprintf( out, "\t\t\t</details>\n" );
	}
}

#ifdef _WIN32
// Enlarges the console window to have a large scrollback size.
static void ConsoleEnlarge()
{
	const HANDLE console_handle = GetStdHandle( STD_OUTPUT_HANDLE );

	// make the console window bigger
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD buffer_size;
	if ( GetConsoleScreenBufferInfo( console_handle, &csbi ) )
	{
		buffer_size.X = csbi.dwSize.X + 30;
		buffer_size.Y = 20000;
		SetConsoleScreenBufferSize( console_handle, buffer_size );
	}

	SMALL_RECT r;
	r.Left = r.Top = 0;
	r.Right = csbi.dwSize.X - 1 + 30;
	r.Bottom = 50;
	SetConsoleWindowInfo( console_handle, true, &r );

	// change the console window title
	SetConsoleTitle( TEXT( APP_SHORT_NAME ) );
}
#endif

void print_usage( char *argv0 )
{
	printf( "\nvulkaninfo - Summarize Vulkan information in relation to the current environment.\n\n" );
	printf( "USAGE: %s [options]\n\n", argv0 );
	printf( "OPTIONS:\n" );
	printf( "-h, --help            Print this help.\n" );
	printf( "--html                Produce an html version of vulkaninfo output, saved as\n" );
	printf( "                      \"vulkaninfo.html\" in the directory in which the command is\n" );
	printf( "                      run.\n" );
	printf( "-j, --json            Produce a json version of vulkaninfo output to standard\n" );
	printf( "                      output.\n" );
	printf( "--json=<gpu-number>   For a multi-gpu system, a single gpu can be targetted by\n" );
	printf( "                      specifying the gpu-number associated with the gpu of \n" );
	printf( "                      interest. This number can be determined by running\n" );
	printf( "                      vulkaninfo without any options specified.\n\n" );
}

int main_impl( int argc, char **argv )
{
	AppInstance inst;
	FILE *out = stdout;

#ifdef _WIN32
	if ( ConsoleIsExclusive() ) ConsoleEnlarge();
#endif

	// Then we check in the command line if the user has wanted a specific plugin to be used.
	std::string rendererName = ProcessCommandLine( argc, argv, &out );

	// First, we need to retrieve the Ashes plugins
	InstanceFactory factory;
	auto plugins = list_plugins( factory );
	inst.plugin = factory.findPlugin( rendererName );

	AppCreateInstance( &inst );

	if ( html_output )
	{
		out = fopen( "vulkaninfo.html", "w" );
		PrintHtmlHeader( out );
		fprintf( out, "\t\t\t<details><summary>" );
	}
	else if ( human_readable_output )
	{
		printf( "=========\n" );
		printf( "ASHESINFO\n" );
		printf( "=========\n\n" );
	}
	if ( html_output || human_readable_output )
	{
		fprintf( out, "Vulkan Instance Version: " );
	}
	if ( html_output )
	{
		fprintf( out, "<div class='val'>%d.%d.%d</div></summary></details>\n", inst.vulkan_major, inst.vulkan_minor,
			inst.vulkan_patch );
		fprintf( out, "\t\t\t<br />\n" );
	}
	else if ( human_readable_output )
	{
		printf( "%d.%d.%d\n\n", inst.vulkan_major, inst.vulkan_minor, inst.vulkan_patch );
	}

	auto ashgpus = inst.instance->enumeratePhysicalDevices();
	uint32_t gpu_count = 0u;
	std::vector< AppGpu > gpus;

	for ( auto & gpu : ashgpus )
	{
		AppGpu app_gpu;
		AppGpuInit( &app_gpu, &inst, gpu_count++, std::move( gpu ) );
		gpus.emplace_back( std::move( app_gpu ) );

		if ( human_readable_output )
		{
			printf( "\n\n" );
		}
	}

	// If json output, confirm the desired gpu exists
	if ( json_output )
	{
		if ( selected_gpu >= gpu_count )
		{
			selected_gpu = 0;
		}
		PrintJsonHeader( inst.vulkan_major, inst.vulkan_minor, inst.vulkan_patch );
	}

	if ( human_readable_output )
	{
		printf( "Instance Extensions:\n" );
		printf( "====================\n" );
	}
	AppDumpExtensions( "", "Instance", inst.global_extensions, out );

	//---Layer-Device-Extensions---
	if ( html_output )
	{
		fprintf( out, "\t\t\t<details><summary>Layers: count = <div class='val'>%d</div></summary>", uint32_t( inst.global_layers.size() ) );
		if ( !inst.global_layers.empty() )
		{
			fprintf( out, "\n" );
		}
	}
	else if ( human_readable_output )
	{
		printf( "Layers: count = %d\n", uint32_t( inst.global_layers.size() ) );
		printf( "=======\n" );
	}
	if ( json_output && ( !inst.global_layers.empty() ) )
	{
		printf( ",\n" );
		printf( "\t\"ArrayOfVkLayerProperties\": [" );
	}

	std::sort( inst.global_layers.begin()
		, inst.global_layers.end()
		, CompareLayerName );

	for ( uint32_t i = 0; i < inst.global_layers.size(); ++i )
	{
		uint32_t layer_major, layer_minor, layer_patch;
		char spec_version[64], layer_version[64];
		auto const *layer_prop = &inst.global_layers[i].layer_properties;

		ExtractVersion( layer_prop->specVersion, &layer_major, &layer_minor, &layer_patch );
		snprintf( spec_version, sizeof( spec_version ), "%d.%d.%d", layer_major, layer_minor, layer_patch );
		snprintf( layer_version, sizeof( layer_version ), "%d", layer_prop->implementationVersion );

		if ( html_output )
		{
			fprintf( out, "\t\t\t\t<details><summary>" );
			fprintf( out, "<div class='type'>%s</div> (%s) Vulkan version <div class='val'>%s</div>, ", layer_prop->layerName.c_str(),
				( char * )layer_prop->description.c_str(), spec_version );
			fprintf( out, "layer version <div class='val'>%s</div></summary>\n", layer_version );
			AppDumpExtensions( "\t\t", "Layer", inst.global_layers[i].extension_properties,
				out );
		}
		else if ( human_readable_output )
		{
			printf( "%s (%s) Vulkan version %s, layer version %s\n", layer_prop->layerName.c_str(), ( char * )layer_prop->description.c_str(),
				spec_version, layer_version );
			AppDumpExtensions( "\t", "Layer", inst.global_layers[i].extension_properties,
				out );
		}
		if ( json_output )
		{
			if ( i > 0 )
			{
				printf( "," );
			}
			printf( "\n" );
			printf( "\t\t{\n" );
			printf( "\t\t\t\"layerName\": \"%s\",\n", layer_prop->layerName.c_str() );
			printf( "\t\t\t\"specVersion\": %u,\n", layer_prop->specVersion );
			printf( "\t\t\t\"implementationVersion\": %u,\n", layer_prop->implementationVersion );
			printf( "\t\t\t\"description\": \"%s\"\n", layer_prop->description.c_str() );
			printf( "\t\t}" );
		}

		if ( html_output )
		{
			fprintf( out, "\t\t\t\t\t<details><summary>Devices count = <div class='val'>%d</div></summary>\n", gpu_count );
		}
		else if ( human_readable_output )
		{
			printf( "\tDevices \tcount = %d\n", gpu_count );
		}

		char const *layer_name = inst.global_layers[i].layer_properties.layerName.c_str();

		for ( uint32_t j = 0; j < gpu_count; ++j )
		{
			if ( html_output )
			{
				fprintf( out, "\t\t\t\t\t\t<details><summary>" );
				fprintf( out, "GPU id: <div class='val'>%u</div> (%s)</summary></details>\n", j, gpus[j].props.deviceName.c_str() );
			}
			else if ( human_readable_output )
			{
				printf( "\t\tGPU id       : %u (%s)\n", j, gpus[j].props.deviceName.c_str() );
			}
			ashes::ExtensionPropertiesArray props;
			AppGetPhysicalDeviceLayerExtensions( &gpus[j], layer_name, props );
			if ( html_output )
			{
				AppDumpExtensions( "\t\t\t", "Layer-Device", props, out );
			}
			else if ( human_readable_output )
			{
				AppDumpExtensions( "\t\t", "Layer-Device", props, out );
			}
		}

		if ( html_output )
		{
			fprintf( out, "\t\t\t\t\t</details>\n" );
			fprintf( out, "\t\t\t\t</details>\n" );
		}
		else if ( human_readable_output )
		{
			printf( "\n" );
		}
	}

	if ( html_output )
	{
		fprintf( out, "\t\t\t</details>\n" );
	}
	if ( json_output && ( !inst.global_layers.empty() ) )
	{
		printf( "\n\t]" );
	}

	fflush( out );
	fflush( stdout );
	//-----------------------------

	if ( html_output )
	{
		fprintf( out, "\t\t\t<details><summary>Presentable Surfaces</summary>" );
		if ( gpu_count > 0 )
		{
			fprintf( out, "\n" );
		}
		else
		{
			fprintf( out, "</details>\n" );
		}
	}
	else if ( human_readable_output )
	{
		printf( "Presentable Surfaces:\n" );
		printf( "=====================\n" );
	}
	inst.width = 256;
	inst.height = 256;
	int format_count = 0;
	int present_mode_count = 0;

#if ASHES_XCB || ASHES_XLIB
	bool has_display = true;
	const char *display_var = getenv( "DISPLAY" );
	if ( display_var == NULL || strlen( display_var ) == 0 )
	{
		fprintf( stderr, "'DISPLAY' environment variable not set... skipping surface info\n" );
		fflush( stderr );
		has_display = false;
	}
#endif

#if ASHES_WAYLAND
	struct wl_display *wayland_display = wl_display_connect( NULL );
	bool has_wayland_display = false;
	if ( wayland_display != NULL )
	{
		wl_display_disconnect( wayland_display );
		has_wayland_display = true;
	}
#endif

//--WIN32--
#if ASHES_WIN32
	struct SurfaceExtensionInfo surface_ext_win32;
	surface_ext_win32.name = ashes::KHR_WIN32_SURFACE_EXTENSION_NAME;
	surface_ext_win32.create_window = AppCreateWin32Window;
	surface_ext_win32.create_surface = AppCreateWin32Surface;
	surface_ext_win32.destroy_window = AppDestroyWin32Window;
	AppDumpSurfaceExtension( &inst, gpus, &surface_ext_win32, &format_count, &present_mode_count, out );
#endif
//--XCB--
#if ASHES_XCB
	struct SurfaceExtensionInfo surface_ext_xcb;
	surface_ext_xcb.name = ashes::KHR_XCB_SURFACE_EXTENSION_NAME;
	surface_ext_xcb.create_window = AppCreateXcbWindow;
	surface_ext_xcb.create_surface = AppCreateXcbSurface;
	surface_ext_xcb.destroy_window = AppDestroyXcbWindow;
	if ( has_display )
	{
		AppDumpSurfaceExtension( &inst, gpus, &surface_ext_xcb, &format_count, &present_mode_count, out );
	}
#endif
//--XLIB--
#if ASHES_XLIB
	struct SurfaceExtensionInfo surface_ext_xlib;
	surface_ext_xlib.name = ashes::KHR_XLIB_SURFACE_EXTENSION_NAME;
	surface_ext_xlib.create_window = AppCreateXlibWindow;
	surface_ext_xlib.create_surface = AppCreateXlibSurface;
	surface_ext_xlib.destroy_window = AppDestroyXlibWindow;
	if ( has_display )
	{
		AppDumpSurfaceExtension( &inst, gpus, &surface_ext_xlib, &format_count, &present_mode_count, out );
	}
#endif
//--MACOS--
#if ASHES_MACOS
	struct SurfaceExtensionInfo surface_ext_macos;
	surface_ext_macos.name = ashes::MVK_MACOS_SURFACE_EXTENSION_NAME;
	surface_ext_macos.create_window = AppCreateMacOSWindow;
	surface_ext_macos.create_surface = AppCreateMacOSSurface;
	surface_ext_macos.destroy_window = AppDestroyMacOSWindow;
	AppDumpSurfaceExtension( &inst, gpus, &surface_ext_macos, &format_count, &present_mode_count, out );
#endif
//--WAYLAND--
#if ASHES_WAYLAND
	struct SurfaceExtensionInfo surface_ext_wayland;
	surface_ext_wayland.name = ashes::KHR_WAYLAND_SURFACE_EXTENSION_NAME;
	surface_ext_wayland.create_window = AppCreateWaylandWindow;
	surface_ext_wayland.create_surface = AppCreateWaylandSurface;
	surface_ext_wayland.destroy_window = AppDestroyWaylandWindow;
	if ( has_wayland_display )
	{
		AppDumpSurfaceExtension( &inst, gpus, &surface_ext_wayland, &format_count, &present_mode_count, out );
	}
#endif

	// TODO: Android
	if ( !format_count && !present_mode_count )
	{
		if ( html_output )
		{
			fprintf( out, "\t\t\t\t<details><summary>None found</summary></details>\n" );
		}
		else if ( human_readable_output )
		{
			printf( "None found\n\n" );
		}
	}

	if ( html_output )
	{
		fprintf( out, "\t\t\t</details>\n" );
	}
	//---------
#if 0

	if ( CheckExtensionEnabled( VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME, inst.inst_extensions, inst.inst_extensions_count ) )
	{
		PFN_vkEnumeratePhysicalDeviceGroupsKHR vkEnumeratePhysicalDeviceGroupsKHR =
			( PFN_vkEnumeratePhysicalDeviceGroupsKHR )vkGetInstanceProcAddr( inst.instance, "vkEnumeratePhysicalDeviceGroupsKHR" );

		uint32_t group_count;
		err = vkEnumeratePhysicalDeviceGroupsKHR( inst.instance, &group_count, NULL );
		if ( err )
		{
			ERR_EXIT( err );
		}

		VkPhysicalDeviceGroupProperties *groups = malloc( sizeof( groups[0] ) * group_count );
		if ( !groups )
		{
			ERR_EXIT( VK_ERROR_OUT_OF_HOST_MEMORY );
		}

		err = vkEnumeratePhysicalDeviceGroupsKHR( inst.instance, &group_count, groups );
		if ( err )
		{
			ERR_EXIT( err );
		}

		if ( html_output )
		{
			fprintf( out, "\t\t\t<details><summary>Groups</summary>\n" );
		}
		else if ( human_readable_output )
		{
			printf( "\nGroups :\n" );
			printf( "========\n" );
		}

		for ( uint32_t i = 0; i < group_count; ++i )
		{
			AppGroupDump( &groups[i], i, &inst, out );
			if ( human_readable_output )
			{
				printf( "\n\n" );
			}
		}

		if ( html_output )
		{
			fprintf( out, "\t\t\t</details>\n" );
		}

		free( groups );
	}
#endif

	if ( html_output )
	{
		fprintf( out, "\t\t\t<details><summary>Device Properties and Extensions</summary>\n" );
	}

	for ( uint32_t i = 0; i < gpu_count; ++i )
	{
		if ( json_output && selected_gpu != i )
		{
// Toggle json_output to allow html output without json output
			json_output = false;
			AppGpuDump( &gpus[i], out );
			json_output = true;
		}
		else
		{
			AppGpuDump( &gpus[i], out );
		}
		if ( human_readable_output )
		{
			printf( "\n\n" );
		}
	}

	if ( html_output )
	{
		fprintf( out, "\t\t\t</details>\n" );
	}

	for ( uint32_t i = 0; i < gpu_count; ++i )
	{
		AppGpuDestroy( &gpus[i] );
	}

	AppDestroyInstance( &inst );

	if ( html_output )
	{
		PrintHtmlFooter( out );
		fflush( out );
		fclose( out );
	}
	if ( json_output )
	{
		printf( "\n}\n" );
	}

	fflush( stdout );

	return 0;
}

int main( int argc, char **argv )
{
	int result = EXIT_SUCCESS;

	try
	{
		result = main_impl( argc, argv );
	}
	catch ( std::exception & exc )
	{
		std::cerr << exc.what() << std::endl;
		result = EXIT_FAILURE;
	}
	catch ( ... )
	{
		std::cerr << "Unknown unhandled exception" << std::endl;
		result = EXIT_FAILURE;
	}

	return result;
}
