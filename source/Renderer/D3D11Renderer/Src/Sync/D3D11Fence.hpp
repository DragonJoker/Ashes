/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include <Sync/Fence.hpp>

namespace d3d11_renderer
{
	class Fence
		: public renderer::Fence
	{
	public:
		Fence( Device const & device
			, renderer::FenceCreateFlags flags = 0 );
		~Fence();

		renderer::WaitResult wait( uint64_t timeout )const override;
		void reset()const override;

		inline ID3D11Fence * getFence()const
		{
			return m_fence;
		}

	private:
		ID3D11DeviceContext4 * m_context{ nullptr };
		ID3D11Fence * m_fence{ nullptr };
	};
}
