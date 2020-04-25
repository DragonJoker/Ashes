/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	struct Context
	{
		Context( D3D_FEATURE_LEVEL featureLevel
			, VkDevice device )
			: Context{ featureLevel, device, getImmediateContext( device ) }
		{
		}

		Context( D3D_FEATURE_LEVEL featureLevel
			, VkDevice device
			, ID3D11DeviceContext * context )
			: device{ device }
			, context{ context }
			, featureLevel{ featureLevel }
		{
			auto hr = context->QueryInterface( __uuidof( ID3D11DeviceContext1 )
				, reinterpret_cast< void ** >( &context1 ) );
			checkError( device, hr, "QueryInterface<ID3D11DeviceContext1>" );
		}

		~Context()
		{
			safeRelease( context1 );
			safeRelease( context );
		}

		VkDevice device{};
		ID3D11DeviceContext * context{};
		ID3D11DeviceContext1 * context1{};
		LayoutBindingWritesArray uavs;
		D3D_FEATURE_LEVEL featureLevel{};

	private:
		static ID3D11DeviceContext * getImmediateContext( VkDevice device );
	};

	class CommandBase
	{
	public:
		CommandBase( VkDevice device );
		virtual ~CommandBase()noexcept;

		virtual void apply( Context const & context )const = 0;
		virtual void remove( Context const & context )const
		{
		}
		virtual void fillContext( Context & context )const
		{
		}
		virtual CommandPtr clone()const = 0;

	protected:
		VkDevice m_device;
	};
}
