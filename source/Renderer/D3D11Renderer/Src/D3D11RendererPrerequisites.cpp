#include "D3D11RendererPrerequisites.hpp"

#include <Core/Exception.hpp>

#if defined __GNUG__
#   ifdef DEFINE_GUID
#       undef DEFINE_GUID
#   endif
#   define DEFINE_GUID( id, a, b, c, d, e, f, g, h, i, j, k ) const GUID id = { a, b, c, { d, e, f, g, h, i, j, k } }

DEFINE_GUID( IID_ID3D11DeviceChild, 0x1841e5c8, 0x16b0, 0x489b, 0xbc, 0xc8, 0x44, 0xcf, 0xb0, 0xd5, 0xde, 0xae );
DEFINE_GUID( IID_ID3D11DepthStencilState, 0x03823efb, 0x8d8f, 0x4e1c, 0x9a, 0xa2, 0xf6, 0x4b, 0xb2, 0xcb, 0xfd, 0xf1 );
DEFINE_GUID( IID_ID3D11BlendState, 0x75b68faa, 0x347d, 0x4159, 0x8f, 0x45, 0xa0, 0x64, 0x0f, 0x01, 0xcd, 0x9a );
DEFINE_GUID( IID_ID3D11RasterizerState, 0x9bb4ab81, 0xab1a, 0x4d8f, 0xb5, 0x06, 0xfc, 0x04, 0x20, 0x0b, 0x6e, 0xe7 );
DEFINE_GUID( IID_ID3D11Resource, 0xdc8e63f3, 0xd12b, 0x4952, 0xb4, 0x7b, 0x5e, 0x45, 0x02, 0x6a, 0x86, 0x2d );
DEFINE_GUID( IID_ID3D11Buffer, 0x48570b85, 0xd1ee, 0x4fcd, 0xa2, 0x50, 0xeb, 0x35, 0x07, 0x22, 0xb0, 0x37 );
DEFINE_GUID( IID_ID3D11Texture1D, 0xf8fb5c27, 0xc6b3, 0x4f75, 0xa4, 0xc8, 0x43, 0x9a, 0xf2, 0xef, 0x56, 0x4c );
DEFINE_GUID( IID_ID3D11Texture2D, 0x6f15aaf2, 0xd208, 0x4e89, 0x9a, 0xb4, 0x48, 0x95, 0x35, 0xd3, 0x4f, 0x9c );
DEFINE_GUID( IID_ID3D11Texture3D, 0x037e866e, 0xf56d, 0x4357, 0xa8, 0xaf, 0x9d, 0xab, 0xbe, 0x6e, 0x25, 0x0e );
DEFINE_GUID( IID_ID3D11View, 0x839d1216, 0xbb2e, 0x412b, 0xb7, 0xf4, 0xa9, 0xdb, 0xeb, 0xe0, 0x8e, 0xd1 );
DEFINE_GUID( IID_ID3D11ShaderResourceView, 0xb0e06fe0, 0x8192, 0x4e1a, 0xb1, 0xca, 0x36, 0xd7, 0x41, 0x47, 0x10, 0xb2 );
DEFINE_GUID( IID_ID3D11RenderTargetView, 0xdfdba067, 0x0b8d, 0x4865, 0x87, 0x5b, 0xd7, 0xb4, 0x51, 0x6c, 0xc1, 0x64 );
DEFINE_GUID( IID_ID3D11DepthStencilView, 0x9fdac92a, 0x1876, 0x48c3, 0xaf, 0xad, 0x25, 0xb9, 0x4f, 0x84, 0xa9, 0xb6 );
DEFINE_GUID( IID_ID3D11UnorderedAccessView, 0x28acf509, 0x7f5c, 0x48f6, 0x86, 0x11, 0xf3, 0x16, 0x01, 0x0a, 0x63, 0x80 );
DEFINE_GUID( IID_ID3D11VertexShader, 0x3b301d64, 0xd678, 0x4289, 0x88, 0x97, 0x22, 0xf8, 0x92, 0x8b, 0x72, 0xf3 );
DEFINE_GUID( IID_ID3D11HullShader, 0x8e5c6061, 0x628a, 0x4c8e, 0x82, 0x64, 0xbb, 0xe4, 0x5c, 0xb3, 0xd5, 0xdd );
DEFINE_GUID( IID_ID3D11DomainShader, 0xf582c508, 0x0f36, 0x490c, 0x99, 0x77, 0x31, 0xee, 0xce, 0x26, 0x8c, 0xfa );
DEFINE_GUID( IID_ID3D11GeometryShader, 0x38325b96, 0xeffb, 0x4022, 0xba, 0x02, 0x2e, 0x79, 0x5b, 0x70, 0x27, 0x5c );
DEFINE_GUID( IID_ID3D11PixelShader, 0xea82e40d, 0x51dc, 0x4f33, 0x93, 0xd4, 0xdb, 0x7c, 0x91, 0x25, 0xae, 0x8c );
DEFINE_GUID( IID_ID3D11ComputeShader, 0x4f5b196e, 0xc2bd, 0x495e, 0xbd, 0x01, 0x1f, 0xde, 0xd3, 0x8e, 0x49, 0x69 );
DEFINE_GUID( IID_ID3D11InputLayout, 0xe4819ddc, 0x4cf0, 0x4025, 0xbd, 0x26, 0x5d, 0xe8, 0x2a, 0x3e, 0x07, 0xb7 );
DEFINE_GUID( IID_ID3D11SamplerState, 0xda6fea51, 0x564c, 0x4487, 0x98, 0x10, 0xf0, 0xd0, 0xf9, 0xb4, 0xe3, 0xa5 );
DEFINE_GUID( IID_ID3D11Asynchronous, 0x4b35d0cd, 0x1e15, 0x4258, 0x9c, 0x98, 0x1b, 0x13, 0x33, 0xf6, 0xdd, 0x3b );
DEFINE_GUID( IID_ID3D11Query, 0xd6c00747, 0x87b7, 0x425e, 0xb8, 0x4d, 0x44, 0xd1, 0x08, 0x56, 0x0a, 0xfd );
DEFINE_GUID( IID_ID3D11Predicate, 0x9eb576dd, 0x9f77, 0x4d86, 0x81, 0xaa, 0x8b, 0xab, 0x5f, 0xe4, 0x90, 0xe2 );
DEFINE_GUID( IID_ID3D11Counter, 0x6e8c49fb, 0xa371, 0x4770, 0xb4, 0x40, 0x29, 0x08, 0x60, 0x22, 0xb7, 0x41 );
DEFINE_GUID( IID_ID3D11ClassInstance, 0xa6cd7faa, 0xb0b7, 0x4a2f, 0x94, 0x36, 0x86, 0x62, 0xa6, 0x57, 0x97, 0xcb );
DEFINE_GUID( IID_ID3D11ClassLinkage, 0xddf57cba, 0x9543, 0x46e4, 0xa1, 0x2b, 0xf2, 0x07, 0xa0, 0xfe, 0x7f, 0xed );
DEFINE_GUID( IID_ID3D11CommandList, 0xa24bc4d1, 0x769e, 0x43f7, 0x80, 0x13, 0x98, 0xff, 0x56, 0x6c, 0x18, 0xe2 );
DEFINE_GUID( IID_ID3D11DeviceContext, 0xc0bfa96c, 0xe089, 0x44fb, 0x8e, 0xaf, 0x26, 0xf8, 0x79, 0x61, 0x90, 0xda );
DEFINE_GUID( IID_ID3D11Device, 0xdb6f6ddb, 0xac77, 0x4e88, 0x82, 0x53, 0x81, 0x9d, 0xf9, 0xbb, 0xf1, 0x40 );
DEFINE_GUID( IID_IDXGIFactory, 0x7b7166ec, 0x21c7, 0x44ae, 0xb2, 0x1a, 0xc9, 0xae, 0x32, 0x1a, 0xe3, 0x69 );
#endif

namespace d3d11_renderer
{
	std::string getLastErrorText()
	{
		LPSTR error = nullptr;
		std::string result;

		if ( ::FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
			, nullptr
			, GetLastError()
			, 0
			, ( LPSTR )&error
			, 0
			, nullptr ) != 0 )
		{
			result += error;
			LocalFree( error );
		}

		return result;
	}

	bool checkError( HRESULT hResult, char const * const text )
	{
		struct Error
		{
			std::string name;
			renderer::Result result;
		};
#if !defined( DXGI_ERROR_INVALID_CALL )
#	define DXGI_ERROR_INVALID_CALL D3DERR_INVALIDCALL
#endif
#if !defined( DXGI_ERROR_WAS_STILL_DRAWING )
#	define DXGI_ERROR_WAS_STILL_DRAWING D3DERR_WASSTILLDRAWING
#endif
		static std::map< HRESULT, Error > Errors
		{
			{ D3D11_ERROR_FILE_NOT_FOUND, { "The file was not found.", renderer::Result::eErrorRenderer } },
			{ D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS, { "There are too many unique instances of a particular type of state object.", renderer::Result::eErrorRenderer } },
			{ D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS, { "There are too many unique instances of a particular type of view object.", renderer::Result::eErrorRenderer } },
			{ D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD, { "The first call to ID3D11DeviceContext::Map after either ID3D11Device::CreateDeferredContext or ID3D11DeviceContext::FinishCommandList per Resource was not D3D11_MAP_WRITE_DISCARD.", renderer::Result::eErrorRenderer } },
			{ E_FAIL, { "Attempted to create a device with the debug layer enabled and the layer is not installed.", renderer::Result::eErrorRenderer } },
			{ E_INVALIDARG, { "An invalid parameter was passed to the returning function.", renderer::Result::eErrorRenderer } },
			{ E_OUTOFMEMORY, { "Direct3D could not allocate sufficient memory to complete the call.", renderer::Result::eErrorOutOfDeviceMemory } },
			{ E_NOTIMPL, { "The method call isn't implemented with the passed parameter combination.", renderer::Result::eErrorRenderer } },
			{ S_FALSE, { "Alternate success value, indicating a successful but nonstandard completion( the precise meaning depends on context ).", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_INVALID_CALL, { "The method call is invalid.For example, a method's parameter may not be a valid pointer.", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_ACCESS_DENIED, { "You tried to use a resource to which you did not have the required access privileges.This error is most typically caused when you write to a shared resource with read - only access.", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_ACCESS_LOST, { "The desktop duplication interface is invalid.The desktop duplication interface typically becomes invalid when a different type of image is displayed on the desktop.", renderer::Result::eErrorRenderer } },
#if defined( DXGI_ERROR_ALREADY_EXISTS )
			{ DXGI_ERROR_ALREADY_EXISTS, { "The desired element already exists.This is returned by DXGIDeclareAdapterRemovalSupport if it is not the first time that the function is called.", renderer::Result::eErrorRenderer } },
#endif
			{ DXGI_ERROR_CANNOT_PROTECT_CONTENT, { "DXGI can't provide content protection on the swap chain. This error is typically caused by an older driver, or when you use a swap chain that is incompatible with content protection.", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_DEVICE_HUNG, { "The application's device failed due to badly formed commands sent by the application. This is an design-time issue that should be investigated and fixed.", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_DEVICE_REMOVED, { "The video card has been physically removed from the system, or a driver upgrade for the video card has occurred.The application should destroy and recreate the device.For help debugging the problem, call ID3D10Device::GetDeviceRemovedReason.", renderer::Result::eErrorDeviceLost } },
			{ DXGI_ERROR_DEVICE_RESET, { "The device failed due to a badly formed command.This is a run - time issue; The application should destroy and recreate the device.", renderer::Result::eErrorDeviceLost } },
			{ DXGI_ERROR_DRIVER_INTERNAL_ERROR, { "The driver encountered a problem and was put into the device removed state.", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_FRAME_STATISTICS_DISJOINT, { "An event( for example, a power cycle ) interrupted the gathering of presentation statistics.", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE, { "The application attempted to acquire exclusive ownership of an output, but failed because some other application( or device within the application ) already acquired ownership.", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_MORE_DATA, { "The buffer supplied by the application is not big enough to hold the requested data.", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_NAME_ALREADY_EXISTS, { "The supplied name of a resource in a call to IDXGIResource1::CreateSharedHandle is already associated with some other resource.", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_NONEXCLUSIVE, { "A global counter resource is in use, and the Direct3D device can't currently use the counter resource.", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_NOT_CURRENTLY_AVAILABLE, { "The resource or request is not currently available, but it might become available later.", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_NOT_FOUND, { "When calling IDXGIObject::GetPrivateData, the GUID passed in is not recognized as one previously passed to IDXGIObject::SetPrivateData or IDXGIObject::SetPrivateDataInterface.When calling IDXGIFactory::EnumAdapters or IDXGIAdapter::EnumOutputs, the enumerated ordinal is out of range.", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED, { "Reserved", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_REMOTE_OUTOFMEMORY, { "Reserved", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE, { "The DXGI output( monitor ) to which the swap chain content was restricted is now disconnected or changed.", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_SDK_COMPONENT_MISSING, { "The operation depends on an SDK component that is missing or mismatched.", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_SESSION_DISCONNECTED, { "The Remote Desktop Services session is currently disconnected.", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_UNSUPPORTED, { "The requested functionality is not supported by the device or the driver.", renderer::Result::eErrorRenderer } },
			{ DXGI_ERROR_WAIT_TIMEOUT, { "The time - out interval elapsed before the next desktop frame was available.", renderer::Result::eTimeout } },
			{ DXGI_ERROR_WAS_STILL_DRAWING, { "The GPU was busy at the moment when a call was made to perform an operation, and did not execute or schedule the operation.", renderer::Result::eErrorRenderer } },
			{ S_OK, { "No error occurred.", renderer::Result::eSuccess } },
		};
		bool result = hResult == S_OK;

		if ( !result )
		{
			std::stringstream stream;
			stream.imbue( std::locale{ "C" } );
			stream << "Error calling [" << text << "]: " << std::hex << hResult;
			LPTSTR error = 0;
			auto it = Errors.find( hResult );
			renderer::Result result{ renderer::Result::eErrorRenderer };

			if ( it != Errors.end() )
			{
				stream << "\n    Direct3D: " << it->second.name;
				result = it->second.result;
			}

			stream << "\n    Windows: " << getLastErrorText();
			renderer::Logger::logError( stream.str() );
			throw renderer::Exception{ result, stream.str() };
		}

		return result;
	}

	std::string toString( std::wstring const & text )
	{
		BOOL used = FALSE;
		auto convertedSize = WideCharToMultiByte( CP_UTF8
			, 0
			, text.c_str()
			, UINT( text.size() )
			, nullptr
			, 0
			, nullptr
			, nullptr );
		std::string result;

		if ( convertedSize > 0 )
		{
			std::vector< char > res( convertedSize + 1, '\0' );

			if ( !WideCharToMultiByte( CP_UTF8
				, 0
				, text.c_str()
				, UINT( text.size() )
				, res.data()
				, UINT( res.size() )
				, nullptr
				, nullptr ) )
			{
				renderer::Logger::logError( getLastErrorText() );
			}
			else
			{
				result += res.data();
			}
		}
		else
		{
			renderer::Logger::logError( getLastErrorText() );
		}

		return result;
	}
}
