/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Core/D3D11DeviceContextLock.hpp"

namespace ashes::d3d11
{
	struct Context
		: public NonCopyable
	{
		Context( D3D_FEATURE_LEVEL pfeatureLevel
			, VkDevice pdevice )
			: Context{ pfeatureLevel, pdevice, getImmediateContext( pdevice ) }
		{
		}

		Context( D3D_FEATURE_LEVEL pfeatureLevel
			, VkDevice pdevice
			, DeviceContextLock pcontext )
			: device{ pdevice }
			, context{ std::move( pcontext ) }
			, featureLevel{ pfeatureLevel }
		{
			auto hr = context->QueryInterface( __uuidof( ID3D11DeviceContext1 )
				, reinterpret_cast< void ** >( &context1 ) );
			checkError( device, hr, "QueryInterface<ID3D11DeviceContext1>" );
		}

		~Context()noexcept
		{
			safeRelease( context1 );
		}

		VkDevice device{};
		DeviceContextLock context;
		ID3D11DeviceContext1 * context1{};
		D3D_FEATURE_LEVEL featureLevel{};
		mutable std::map< UINT, ID3D11UnorderedAccessView * > uavs{};
		mutable std::vector< ID3D11UnorderedAccessView * > rawUavs{};
		mutable UINT uavStart{};

	private:
		static DeviceContextLock getImmediateContext( VkDevice device );
	};

	class CommandBase
	{
	public:
		CommandBase( CommandBase const & ) = default;
		CommandBase( CommandBase && )noexcept = default;
		CommandBase & operator=( CommandBase const & ) = default;
		CommandBase & operator=( CommandBase && )noexcept = default;

		explicit CommandBase( VkDevice device );
		virtual ~CommandBase()noexcept = default;

		virtual void apply( Context const & context )const = 0;
		virtual void remove( Context const & context )const
		{
		}

		virtual void fillContext( Context & context )const
		{
		}

		virtual CommandPtr clone()const = 0;

		VkDevice getDevice()const noexcept
		{
			return m_device;
		}

	private:
		VkDevice m_device;
	};
}
