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
	};
}
