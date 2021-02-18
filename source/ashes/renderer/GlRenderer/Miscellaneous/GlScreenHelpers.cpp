#include "GlScreenHelpers.hpp"

#include <ashes/common/Hash.hpp>

#if _WIN32
#	include <Windows.h>
#elif __linux__
#	include <X11/Xlib.h>
#	include <X11/extensions/Xrandr.h>
#endif

namespace ashes::gl
{
	namespace
	{
		size_t makeKey( uint32_t width
			, uint32_t height
			, uint32_t refreshRate )
		{
			auto result = std::hash< uint32_t >{}( width );
			hashCombine( result, height );
			hashCombine( result, refreshRate );
			return result;
		}

#if _WIN32

		uint32_t countDisplays()
		{
			return uint32_t( ::GetSystemMetrics( SM_CMONITORS ) );
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

		bool getScreenData( uint32_t index
			, VkDisplayPropertiesKHR & data
			, std::string & name
			, std::vector< VkDisplayModeParametersKHR > & displayModesParams )
		{
			DISPLAY_DEVICEA displayDevice{};
			displayDevice.cb = sizeof( DISPLAY_DEVICEA );

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
					listScreenResolutions( data, displayModesParams );
					return true;
				}

				::DeleteDC( hdc );
			}

			return false;
		}

#elif __linux__

		uint32_t countDisplays()
		{
			auto dpy = ::XOpenDisplay( nullptr );
			auto result = uint32_t( ::XScreenCount( dpy ) );
			::XCloseDisplay( dpy );
			return result;
		}

		void listScreenResolutions( Display * dpy
			, uint32_t index
			, VkDisplayPropertiesKHR & displayProps
			, std::vector< VkDisplayModeParametersKHR > & params )
		{
			auto root = XRootWindow (dpy, index);
			auto sc = XRRGetScreenInfo (dpy, root);
			Rotation currentRotation;
			auto rotation = XRRConfigRotations( sc, &currentRotation );

			if ( checkFlag( rotation, Rotation( RR_Rotate_0 ) ) )
			{
				displayProps.supportedTransforms |= VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			}

			if ( checkFlag( rotation, Rotation( RR_Rotate_90 ) ) )
			{
				displayProps.supportedTransforms |= VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR;
			}

			if ( checkFlag( rotation, Rotation( RR_Rotate_180 ) ) )
			{
				displayProps.supportedTransforms |= VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR;
			}

			if ( checkFlag( rotation, Rotation( RR_Rotate_270 ) ) )
			{
				displayProps.supportedTransforms |= VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR;
			}

			int nsize{ 0 };
			auto sizes = XRRConfigSizes( sc, &nsize );
			std::set< size_t > hashedParams;

			for ( int sz = 0; sz < nsize; sz++ )
			{
				int nrate{ 0 };
				auto rates = XRRConfigRates( sc, sz, &nrate );

				for ( int rate = 0; rate < nrate; rate++ )
				{
					auto key = makeKey( sizes[sz].width
						, sizes[sz].height
						, rates[rate] );

					if ( hashedParams.insert( key ).second )
					{
						params.push_back(
							{
								{ uint32_t( sizes[sz].width ), uint32_t( sizes[sz].height ) },
								uint32_t( rates[rate] * 1000 )
							});
					}
				}
			}

			XRRFreeScreenConfigInfo( sc );
		}

		bool getScreenData( uint32_t index
			, VkDisplayPropertiesKHR & data
			, std::string & name
			, std::vector< VkDisplayModeParametersKHR > & params )
		{
			auto dpy = ::XOpenDisplay( nullptr );
			
			if ( dpy )
			{
				auto screen = XScreenOfDisplay( dpy, index );

				if ( screen )
				{
					int w = ::XWidthMMOfScreen( screen );
					int h = ::XHeightMMOfScreen( screen );
					data.physicalDimensions = { uint32_t( w ), uint32_t( h ) };
					w = ::XWidthOfScreen( screen );
					h = ::XHeightOfScreen( screen );
					data.physicalResolution = { uint32_t( w ), uint32_t( h ) };
					listScreenResolutions( dpy, index, data, params );
					::XCloseDisplay( dpy );
					return true;
				}

				::XCloseDisplay( dpy );
			}

			return false;
		}

#elif __APPLE__

		uint32_t countDisplays()
		{
			return 0u;
		}

		bool getScreenData( uint32_t index
			, VkDisplayPropertiesKHR & data
			, std::string & name
			, std::vector< VkDisplayModeParametersKHR > & displayModesParams )
		{
			return false;
		}

#endif
	}

	uint32_t getScreenCount()
	{
		return countDisplays();
	}

	void getScreenDesc( uint32_t index
		, std::string & name
		, VkDisplayPropertiesKHR & displayProps
		, std::vector< VkDisplayModeParametersKHR > & displayModesParams )
	{
		getScreenData( index
			, displayProps
			, name
			, displayModesParams );
	}
}
