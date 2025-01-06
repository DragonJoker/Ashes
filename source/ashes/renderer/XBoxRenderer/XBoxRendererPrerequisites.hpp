/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#ifndef ___XBOXRenderer_Prerequisites_HPP___
#define ___XBOXRenderer_Prerequisites_HPP___
#pragma once

#pragma warning( push )
#pragma warning( disable:4005 )	// Redéfinition de macrao
#pragma warning( disable:4311 )
#pragma warning( disable:4312 )

#if defined __GNUG__
#   define __in
#   define __inout
#   define __out
#   define __in_opt
#   define __inout_opt
#   define __out_opt
#   define __deref_out_range(A,B)
#   define __in_bcount(a)
#   define __in_ecount(a)
#   define __in_bcount_opt(a)
#   define __in_ecount_opt(a)
#   define __out_bcount(a)
#   define __out_ecount(a)
#   define __out_bcount_opt(a)
#   define __out_ecount_opt(a)
#   define __out_ecount_part_opt(a,b)
#   define __deref_out
#   define __deref_out_opt
#   define __RPC__deref_out
#   define _COM_Outptr_
#   define _Out_
#   define _Out_opt_
#   define _In_
#   define _In_opt_
#   define _In_z_
#   define _In_opt_z_
#   define _In_reads_bytes_(x)
#   define _Out_writes_bytes_(x)
#   define _In_reads_(x)
#   define _Out_writes_(x)
#   define _Outptr_result_z_
#   define _Outptr_result_opt_z_
#   define _Outptr_
#   define _Outptr_opt_
#   define _Inout_
#   define _Inout_opt_
#   define _Outptr_result_maybenull_
#   define _Outptr_opt_result_maybenull_
#   define WINAPI_INLINE

typedef uint8_t UINT8;
// to prevent includes of DxGI.h
#	define __WIDL_DXGI_H
#endif

#include <ashes/ashes.hpp>

#include <Windows.h>
#include <initguid.h>

#pragma warning( push )
#pragma warning( disable: 4365 )
#pragma warning( disable: 5219 )
#include <d3d11.h>
#include <d3d11_2.h>

#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#pragma warning( pop )

#include "renderer/XBoxRenderer/Enum/XBoxBlendFactor.hpp"
#include "renderer/XBoxRenderer/Enum/XBoxBlendOp.hpp"
#include "renderer/XBoxRenderer/Enum/XBoxBufferTarget.hpp"
#include "renderer/XBoxRenderer/Enum/XBoxCompareOp.hpp"
#include "renderer/XBoxRenderer/Enum/XBoxCullModeFlag.hpp"
#include "renderer/XBoxRenderer/Enum/XBoxFilter.hpp"
#include "renderer/XBoxRenderer/Enum/XBoxFormat.hpp"
#include "renderer/XBoxRenderer/Enum/XBoxImageUsageFlag.hpp"
#include "renderer/XBoxRenderer/Enum/XBoxIndexType.hpp"
#include "renderer/XBoxRenderer/Enum/XBoxMemoryMapFlag.hpp"
#include "renderer/XBoxRenderer/Enum/XBoxMemoryPropertyFlag.hpp"
#include "renderer/XBoxRenderer/Enum/XBoxPolygonMode.hpp"
#include "renderer/XBoxRenderer/Enum/XBoxPrimitiveTopology.hpp"
#include "renderer/XBoxRenderer/Enum/XBoxStencilOp.hpp"
#include "renderer/XBoxRenderer/Enum/XBoxVertexInputRate.hpp"
#include "renderer/XBoxRenderer/Pipeline/XBoxColourBlendState.hpp"
#include "renderer/XBoxRenderer/Pipeline/XBoxDepthStencilState.hpp"
#include "renderer/XBoxRenderer/Pipeline/XBoxRasterisationState.hpp"
#include "renderer/XBoxRenderer/Pipeline/XBoxScissor.hpp"
#include "renderer/XBoxRenderer/Pipeline/XBoxSpecialisationInfo.hpp"
#include "renderer/XBoxRenderer/Pipeline/XBoxStencilOpState.hpp"
#include "renderer/XBoxRenderer/Pipeline/XBoxViewport.hpp"

#include <renderer/RendererCommon/AshesRendererPrerequisites.hpp>
#include <renderer/RendererCommon/Helper/ConstantFormat.hpp>

#include <ashes/common/ArrayView.hpp>
#include <ashes/common/Format.hpp>
#include <ashes/common/FlagCombination.hpp>
#include <ashes/common/Signal.hpp>

#include <array>
#include <cassert>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#if defined( _WIN32 ) && !defined( XBoxRenderer_STATIC )
#	ifdef XBoxRenderer_EXPORTS
#		define XBoxRenderer_API __declspec( dllexport )
#	else
#		define XBoxRenderer_API __declspec( dllimport )
#	endif
#else
#	define XBoxRenderer_API
#endif

#ifdef min
#	undef min
#	undef max
#	undef abs
#endif

#if defined __GNUG__

#	ifndef __uuidof
#		define __uuidof(x) IID_##x
#	endif

#   ifdef DECLARE_GUID
#       undef DECLARE_GUID
#   endif
#   define DECLARE_GUID( id, a, b, c, d, e, f, g, h, i, j, k ) extern const GUID id;

DECLARE_GUID( IID_ID3D11DeviceChild, 0x1841e5c8, 0x16b0, 0x489b, 0xbc, 0xc8, 0x44, 0xcf, 0xb0, 0xd5, 0xde, 0xae );
DECLARE_GUID( IID_ID3D11DepthStencilState, 0x03823efb, 0x8d8f, 0x4e1c, 0x9a, 0xa2, 0xf6, 0x4b, 0xb2, 0xcb, 0xfd, 0xf1 );
DECLARE_GUID( IID_ID3D11BlendState, 0x75b68faa, 0x347d, 0x4159, 0x8f, 0x45, 0xa0, 0x64, 0x0f, 0x01, 0xcd, 0x9a );
DECLARE_GUID( IID_ID3D11RasterizerState, 0x9bb4ab81, 0xab1a, 0x4d8f, 0xb5, 0x06, 0xfc, 0x04, 0x20, 0x0b, 0x6e, 0xe7 );
DECLARE_GUID( IID_ID3D11Resource, 0xdc8e63f3, 0xd12b, 0x4952, 0xb4, 0x7b, 0x5e, 0x45, 0x02, 0x6a, 0x86, 0x2d );
DECLARE_GUID( IID_ID3D11Buffer, 0x48570b85, 0xd1ee, 0x4fcd, 0xa2, 0x50, 0xeb, 0x35, 0x07, 0x22, 0xb0, 0x37 );
DECLARE_GUID( IID_ID3D11Texture1D, 0xf8fb5c27, 0xc6b3, 0x4f75, 0xa4, 0xc8, 0x43, 0x9a, 0xf2, 0xef, 0x56, 0x4c );
DECLARE_GUID( IID_ID3D11Texture2D, 0x6f15aaf2, 0xd208, 0x4e89, 0x9a, 0xb4, 0x48, 0x95, 0x35, 0xd3, 0x4f, 0x9c );
DECLARE_GUID( IID_ID3D11Texture3D, 0x037e866e, 0xf56d, 0x4357, 0xa8, 0xaf, 0x9d, 0xab, 0xbe, 0x6e, 0x25, 0x0e );
DECLARE_GUID( IID_ID3D11View, 0x839d1216, 0xbb2e, 0x412b, 0xb7, 0xf4, 0xa9, 0xdb, 0xeb, 0xe0, 0x8e, 0xd1 );
DECLARE_GUID( IID_ID3D11ShaderResourceView, 0xb0e06fe0, 0x8192, 0x4e1a, 0xb1, 0xca, 0x36, 0xd7, 0x41, 0x47, 0x10, 0xb2 );
DECLARE_GUID( IID_ID3D11RenderTargetView, 0xdfdba067, 0x0b8d, 0x4865, 0x87, 0x5b, 0xd7, 0xb4, 0x51, 0x6c, 0xc1, 0x64 );
DECLARE_GUID( IID_ID3D11DepthStencilView, 0x9fdac92a, 0x1876, 0x48c3, 0xaf, 0xad, 0x25, 0xb9, 0x4f, 0x84, 0xa9, 0xb6 );
DECLARE_GUID( IID_ID3D11UnorderedAccessView, 0x28acf509, 0x7f5c, 0x48f6, 0x86, 0x11, 0xf3, 0x16, 0x01, 0x0a, 0x63, 0x80 );
DECLARE_GUID( IID_ID3D11VertexShader, 0x3b301d64, 0xd678, 0x4289, 0x88, 0x97, 0x22, 0xf8, 0x92, 0x8b, 0x72, 0xf3 );
DECLARE_GUID( IID_ID3D11HullShader, 0x8e5c6061, 0x628a, 0x4c8e, 0x82, 0x64, 0xbb, 0xe4, 0x5c, 0xb3, 0xd5, 0xdd );
DECLARE_GUID( IID_ID3D11DomainShader, 0xf582c508, 0x0f36, 0x490c, 0x99, 0x77, 0x31, 0xee, 0xce, 0x26, 0x8c, 0xfa );
DECLARE_GUID( IID_ID3D11GeometryShader, 0x38325b96, 0xeffb, 0x4022, 0xba, 0x02, 0x2e, 0x79, 0x5b, 0x70, 0x27, 0x5c );
DECLARE_GUID( IID_ID3D11PixelShader, 0xea82e40d, 0x51dc, 0x4f33, 0x93, 0xd4, 0xdb, 0x7c, 0x91, 0x25, 0xae, 0x8c );
DECLARE_GUID( IID_ID3D11ComputeShader, 0x4f5b196e, 0xc2bd, 0x495e, 0xbd, 0x01, 0x1f, 0xde, 0xd3, 0x8e, 0x49, 0x69 );
DECLARE_GUID( IID_ID3D11InputLayout, 0xe4819ddc, 0x4cf0, 0x4025, 0xbd, 0x26, 0x5d, 0xe8, 0x2a, 0x3e, 0x07, 0xb7 );
DECLARE_GUID( IID_ID3D11SamplerState, 0xda6fea51, 0x564c, 0x4487, 0x98, 0x10, 0xf0, 0xd0, 0xf9, 0xb4, 0xe3, 0xa5 );
DECLARE_GUID( IID_ID3D11Asynchronous, 0x4b35d0cd, 0x1e15, 0x4258, 0x9c, 0x98, 0x1b, 0x13, 0x33, 0xf6, 0xdd, 0x3b );
DECLARE_GUID( IID_ID3D11Query, 0xd6c00747, 0x87b7, 0x425e, 0xb8, 0x4d, 0x44, 0xd1, 0x08, 0x56, 0x0a, 0xfd );
DECLARE_GUID( IID_ID3D11Predicate, 0x9eb576dd, 0x9f77, 0x4d86, 0x81, 0xaa, 0x8b, 0xab, 0x5f, 0xe4, 0x90, 0xe2 );
DECLARE_GUID( IID_ID3D11Counter, 0x6e8c49fb, 0xa371, 0x4770, 0xb4, 0x40, 0x29, 0x08, 0x60, 0x22, 0xb7, 0x41 );
DECLARE_GUID( IID_ID3D11ClassInstance, 0xa6cd7faa, 0xb0b7, 0x4a2f, 0x94, 0x36, 0x86, 0x62, 0xa6, 0x57, 0x97, 0xcb );
DECLARE_GUID( IID_ID3D11ClassLinkage, 0xddf57cba, 0x9543, 0x46e4, 0xa1, 0x2b, 0xf2, 0x07, 0xa0, 0xfe, 0x7f, 0xed );
DECLARE_GUID( IID_ID3D11CommandList, 0xa24bc4d1, 0x769e, 0x43f7, 0x80, 0x13, 0x98, 0xff, 0x56, 0x6c, 0x18, 0xe2 );
DECLARE_GUID( IID_ID3D11DeviceContext, 0xc0bfa96c, 0xe089, 0x44fb, 0x8e, 0xaf, 0x26, 0xf8, 0x79, 0x61, 0x90, 0xda );
DECLARE_GUID( IID_ID3D11Device, 0xdb6f6ddb, 0xac77, 0x4e88, 0x82, 0x53, 0x81, 0x9d, 0xf9, 0xbb, 0xf1, 0x40 );
DECLARE_GUID( IID_IDXGIFactory, 0x7b7166ec, 0x21c7, 0x44ae, 0xb2, 0x1a, 0xc9, 0xae, 0x32, 0x1a, 0xe3, 0x69 );
#endif

#if !defined( NDEBUG )
#	if !defined( D3D_DEBUG_INFO )
#		define D3D_DEBUG_INFO
#	endif
#endif

#if !defined( NDEBUG )
#	define dxRenderer_DebugNames 0
#else
#	define dxRenderer_DebugNames 0
#endif

#if dxRenderer_DebugNames
#	if defined( _MSC_VER )
#		define dxDebugName( obj, type )\
			if( obj )\
			{\
				char name[1024]{ 0 };\
				uint64_t address = uint64_t( obj );\
				sprintf_s( name, "%30s [0x%0I64X]", #type, address );\
				obj->SetPrivateData( WKPDID_D3DDebugObjectName, UINT( strnlen( name, 1024 ) ), name );\
				std::clog << name << "\n";\
			}
#	else
#		define dxDebugName( obj, type )\
			if( obj )\
			{\
				char name[1024] = { 0 };\
				uint64_t address = uint64_t( obj );\
				sprintf( name, "%30s [0x%016X]", #type, address );\
				obj->SetPrivateData( WKPDID_D3DDebugObjectName, UINT( strnlen( name, 1024 ) ), name );\
				std::clog << name << "\n";\
			}
#	endif
#else
#	define dxDebugName( obj, type )
#endif

namespace ashes::xbox
{
	class NonCopyable
	{
	private:
		NonCopyable( NonCopyable const & ) = delete;
		NonCopyable( NonCopyable && )noexcept = delete;
		NonCopyable & operator=( NonCopyable const & ) = delete;
		NonCopyable & operator=( NonCopyable && )noexcept = delete;

	protected:
		NonCopyable()noexcept = default;
		~NonCopyable()noexcept = default;
	};

	struct AdapterInfo
	{
		IDXGIAdapter * adapter{ nullptr };
		IDXGIAdapter1 * adapter1{ nullptr };
		IDXGIAdapter2 * adapter2{ nullptr };
		IDXGIOutput * output{ nullptr };
		D3D_FEATURE_LEVEL featureLevel;
	};

	struct DebugLabel
	{
		std::array< float, 4u > color;
		std::string labelName;
	};

	class ExtensionNotPresentException
		: public ashes::Exception
	{
	public:
		explicit ExtensionNotPresentException( std::string const & name )
			: Exception{ VK_ERROR_EXTENSION_NOT_PRESENT, name }
		{
		}
	};

	struct DepthBias
	{
		float constantFactor;
		float clamp;
		float slopeFactor;
	};
	inline bool operator==( DepthBias const & lhs, DepthBias const & rhs )
	{
		return lhs.constantFactor == rhs.constantFactor
			&& lhs.clamp == rhs.clamp
			&& lhs.slopeFactor == rhs.slopeFactor;
	}
	inline bool operator!=( DepthBias const & lhs, DepthBias const & rhs )
	{
		return !( lhs == rhs );
	}

	struct DepthBounds
	{
		float minBound;
		float maxBound;
	};
	inline bool operator==( DepthBounds const & lhs, DepthBounds const & rhs )
	{
		return lhs.minBound == rhs.minBound
			&& lhs.maxBound == rhs.maxBound;
	}
	inline bool operator!=( DepthBounds const & lhs, DepthBounds const & rhs )
	{
		return !( lhs == rhs );
	}

	struct Context;

	class Attribute;
	class Buffer;
	class BufferView;
	class CommandBase;
	class CommandBuffer;
	class CommandPool;
	class DescriptorPool;
	class DescriptorSet;
	class DescriptorSetLayout;
	class DescriptorSetLayoutBinding;
	class Device;
	class DeviceMemory;
	class FrameBuffer;
	class GeometryBuffers;
	class ObjectMemory;
	class Pipeline;
	class PipelineLayout;
	class PhysicalDevice;
	class QueryPool;
	class Queue;
	class Instance;
	class RenderPass;
	class RenderSubpass;
	class Sampler;
	class Semaphore;
	class ShaderModule;
	class Surface;
	class SwapChain;
	class Image;
	class ImageView;
	class VertexBufferBase;
	class VertexLayout;

	class SamplerYcbcrConversion;
	class DescriptorUpdateTemplate;
	class DisplayKHR;
	class DisplayModeKHR;
	class ObjectTableNVX;
	class IndirectCommandsLayoutNVX;
	class DebugUtilsMessengerEXT;
	class ValidationCacheEXT;

	using Action = std::function< void( Context const & ) >;
	using ActionArray = std::vector< Action >;

	using AttributeArray = std::vector< Attribute >;

	using CommandPtr = std::unique_ptr< CommandBase >;
	using CommandPoolPtr = std::unique_ptr< CommandPool >;
	using PipelinePtr = std::unique_ptr< Pipeline >;
	using PhysicalDevicePtr = std::unique_ptr< PhysicalDevice >;
	using QueuePtr = std::unique_ptr< Queue >;
	using RenderSubpassPtr = std::unique_ptr< RenderSubpass >;
	using SurfacePtr = std::unique_ptr< Surface >;
	using ImageViewPtr = std::unique_ptr< ImageView >;

	using BufferCRef = std::reference_wrapper< Buffer const >;
	using CommandBufferCRef = std::reference_wrapper< CommandBuffer const >;
	using DescriptorSetCRef = std::reference_wrapper< DescriptorSet const >;
	using DescriptorSetLayoutCRef = std::reference_wrapper< DescriptorSetLayout const >;
	using RenderSubpassCRef = std::reference_wrapper< RenderSubpass const >;
	using SemaphoreCRef = std::reference_wrapper< Semaphore const >;
	using SwapChainCRef = std::reference_wrapper< SwapChain const >;
	using TextureCRef = std::reference_wrapper< Image const >;
	using ImageViewCRef = std::reference_wrapper< ImageView const >;
	using VertexLayoutCRef = std::reference_wrapper< VertexLayout const >;
	using VertexBufferCRef = std::reference_wrapper< VertexBufferBase const >;

	using CommandArray = std::vector< CommandPtr >;

	template< typename T >
	void safeRelease( T *& toRelease )noexcept
	{
		if ( toRelease )
		{
			toRelease->Release();
			toRelease = nullptr;
		}
	}

	template< typename Dst, typename Src >
	std::vector< std::reference_wrapper< Dst const > > staticCast( std::vector< std::reference_wrapper< Src const > > const & src )
	{
		std::vector< std::reference_wrapper< Dst const > > result;

		for ( auto & s : src )
		{
			result.emplace_back( static_cast< Dst const & >( s.get() ) );
		}

		return result;
	}

	struct PushConstantsDesc
	{
		VkShaderStageFlags stageFlags;
		uint32_t offset;
		uint32_t size;
		std::vector< uint8_t > data;
	};

	struct PushConstantsBuffer
	{
		VkBuffer ubo;
		UINT location;
		PushConstantsDesc data;
		VkDeviceMemory memory;
	};

	struct VbosBinding
	{
		UINT startIndex;
		std::vector< VkBuffer > buffers;
		std::vector< ID3D11Buffer * > d3dBuffers;
		std::vector< UINT > offsets;
		std::vector< UINT > strides;
	};

	using VbosBindingArray = std::vector< VbosBinding >;

	struct LayoutBindingWrites
	{
		VkDescriptorSetLayoutBinding binding;
		VkWriteDescriptorSetArray writes;
	};
	using LayoutBindingWritesArray = std::vector< LayoutBindingWrites * >;
	using LayoutBindingWritesMap = std::map< uint32_t, LayoutBindingWrites >;

	std::string getLastErrorText();
	bool checkError( VkDevice device, HRESULT hResult, char const * const text );
	std::string toString( std::wstring const & text );
	uint32_t deduceMemoryType( uint32_t typeBits
		, VkMemoryPropertyFlags requirements );

#if VK_EXT_debug_utils
	ID3D11DeviceChild * getObject( uint64_t object
		, VkObjectType objectType );
#endif
#if VK_EXT_debug_report || VK_EXT_debug_marker
	ID3D11DeviceChild * getObject( uint64_t object
		, VkDebugReportObjectTypeEXT objectType );
#endif

	using DeviceMemoryDestroyFunc = std::function< void( VkDeviceMemory ) >;
	using DeviceMemoryDestroySignal = Signal< DeviceMemoryDestroyFunc >;
	using DeviceMemoryDestroyConnection = SignalConnection< DeviceMemoryDestroySignal >;
}

#pragma warning( pop )

#endif
