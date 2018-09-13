/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Sync/D3D11Fence.hpp"

#include "Core/D3D11Device.hpp"

namespace d3d11_renderer
{
	Fence::Fence( Device const & device
		, renderer::FenceCreateFlags flags )
		: renderer::Fence{ device, flags }
	{
#if defined( __ID3D11Device5_INTERFACE_DEFINED__ )

		ID3D11DeviceContext * context;
		device.getDevice()->GetImmediateContext( &context );
		auto hr = context->QueryInterface( __uuidof( ID3D11DeviceContext4 )
			, reinterpret_cast< void ** >( &m_context ) );

		if ( dxCheckError( hr, "QueryInterface<ID3D11DeviceContext4>" ) )
		{
			hr = device.getDevice5()->CreateFence( UINT64( flags )
				, D3D11_FENCE_FLAG_NONE
				, __uuidof( ID3D11Fence )
				, reinterpret_cast< void ** >( &m_fence ) );
		}

		safeRelease( context );

#endif
	}

	Fence::~Fence()
	{
		safeRelease( m_fence );
		safeRelease( m_context );
	}

	renderer::WaitResult Fence::wait( uint64_t timeout )const
	{
#if defined( __ID3D11Device5_INTERFACE_DEFINED__ )

		auto before = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds wait{};
		bool end = false;

		while ( !end && uint64_t( wait.count() ) < timeout )
		{
			end = m_fence->GetCompletedValue() != 0;
			wait = std::chrono::duration_cast< std::chrono::nanoseconds >( std::chrono::high_resolution_clock::now() - before );
			std::this_thread::sleep_for( std::chrono::nanoseconds{ 1000 } );
		}

		return end
			? renderer::WaitResult::eSuccess
			: renderer::WaitResult::eTimeOut;

#else

		return renderer::WaitResult::eSuccess;

#endif
	}

	void Fence::reset()const
	{
#if defined( __ID3D11Device5_INTERFACE_DEFINED__ )

		m_context->Signal( m_fence, 0u );

#endif
	}
}
