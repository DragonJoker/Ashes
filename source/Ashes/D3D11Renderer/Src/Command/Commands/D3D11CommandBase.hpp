/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include "Core/D3D11Device.hpp"

namespace d3d11_renderer
{
	struct Context
	{
		Context( Device const & device )
			: Context{ getImmediateContext( device ) }
		{
		}

		Context( ID3D11DeviceContext * context )
			: context{ context }
		{
			auto hr = context->QueryInterface( __uuidof( ID3D11DeviceContext1 )
				, reinterpret_cast< void ** >( &context1 ) );
			dxCheckError( hr, "QueryInterface<ID3D11DeviceContext1>" );
			hr = context->QueryInterface( __uuidof( ID3D11DeviceContext2 )
				, reinterpret_cast< void ** >( &context2 ) );
			dxCheckError( hr, "QueryInterface<ID3D11DeviceContext2>" );
		}

		~Context()
		{
			safeRelease( context2 );
			safeRelease( context1 );
			safeRelease( context );
		}

		ID3D11DeviceContext * context;
		ID3D11DeviceContext1 * context1;
		ID3D11DeviceContext2 * context2;
		WriteDescriptorSetBindingArray uavs;

	private:
		static inline ID3D11DeviceContext * getImmediateContext( Device const & device )
		{
			ID3D11DeviceContext * result;
			device.getDevice()->GetImmediateContext( &result );
			return result;
		}
	};

	class CommandBase
	{
	public:
		CommandBase( Device const & device );
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
		Device const & m_device;
	};
}
