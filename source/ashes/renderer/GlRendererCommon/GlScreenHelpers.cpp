#include "GlScreenHelpers.hpp"

#include <common/Hash.hpp>

#if _WIN32
#	include <Windows.h>
#elif __linux__
#	include "Platform/GlEglContext.hpp"
#	include "Platform/GlWaylandContext.hpp"
#	include "Platform/GlXcbContext.hpp"
#	if ASHES_USE_XLIB_EGL
#		include "Platform/GlX11EglContext.hpp"
#	else
#		include "Platform/GlX11Context.hpp"
#	endif
#endif

namespace ashes::gl
{
#if _WIN32

	namespace
	{
		bool getScreenData( uint32_t index
			, VkDisplayPropertiesKHR & data
			, std::string & name )
		{
			DISPLAY_DEVICE displayDevice{};
			displayDevice.cb = sizeof( DISPLAY_DEVICE );

			if ( ::EnumDisplayDevicesA( nullptr, index, &displayDevice, EDD_GET_DEVICE_INTERFACE_NAME ) )
			{
				name = displayDevice.DeviceName;
				data.displayName = name.c_str();
				auto hdc = ::CreateDCA( "DISPLAY", displayDevice.DeviceName, nullptr, nullptr );

				if ( hdc )
				{
					int w = ::GetDeviceCaps( hdc, HORZSIZE );
					int h = ::GetDeviceCaps( hdc, VERTSIZE );
					data.physicalDimensions = { uint32_t( w ), uint32_t( h ) };
					w = ::GetDeviceCaps( hdc, HORZRES );
					h = ::GetDeviceCaps( hdc, VERTRES );
					data.physicalResolution = { uint32_t( w ), uint32_t( h ) };
					::DeleteDC( hdc );
					return true;
				}

				::DeleteDC( hdc );
			}

			return false;
		}

		uint32_t countDisplays()
		{
			return uint32_t( ::GetSystemMetrics( SM_CMONITORS ) );
		}
	}

	size_t makeKey( DWORD width
		, DWORD height
		, DWORD refreshRate )
	{
		auto result = std::hash< DWORD >{}( width );
		hashCombine( result, height );
		hashCombine( result, refreshRate );
		return result;
	}

	void listScreenResolutions( VkDisplayPropertiesKHR & displayProps
		, std::vector< VkDisplayModeParametersKHR > & params )
	{
		int modeIndex = 0;
		DEVMODEA devMode;
		std::set< size_t > hashedParams;

		while ( ::EnumDisplaySettingsA( displayProps.displayName, modeIndex, &devMode ) )
		{
			if ( checkFlag( devMode.dmFields, DM_DISPLAYORIENTATION ) )
			{
				if ( devMode.dmDisplayOrientation == DMDO_DEFAULT )
				{
					displayProps.supportedTransforms |= VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
				}
				else if ( devMode.dmDisplayOrientation == DMDO_90 )
				{
					displayProps.supportedTransforms |= VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR;
				}
				else if ( devMode.dmDisplayOrientation == DMDO_180 )
				{
					displayProps.supportedTransforms |= VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR;
				}
				else if ( devMode.dmDisplayOrientation == DMDO_270 )
				{
					displayProps.supportedTransforms |= VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR;
				}
			}

			if ( checkFlag( devMode.dmFields, DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL ) )
			{
				auto key = makeKey( devMode.dmPelsWidth
					, devMode.dmPelsHeight
					, devMode.dmDisplayFrequency );

				if ( hashedParams.insert( key ).second )
				{
					params.push_back(
						{
							{ devMode.dmPelsWidth, devMode.dmPelsHeight },
							devMode.dmDisplayFrequency * 1000,
						} );
				}
			}

			++modeIndex;
		}
	}

#endif

	uint32_t getScreenCount()
	{
		return countDisplays();
	}

	void getScreenDesc( uint32_t index
		, std::string & name
		, VkDisplayPropertiesKHR & displayProps
		, std::vector< VkDisplayModeParametersKHR > & displayModesParams )
	{
		getScreenData( index, displayProps, name );
		listScreenResolutions( displayProps, displayModesParams );
	}
}
