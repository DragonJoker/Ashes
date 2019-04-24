/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class DispatchIndirectCommand
		: public CommandBase
	{
	public:
		DispatchIndirectCommand( VkDevice device
			, VkBuffer buffer
			, uint32_t offset );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		Buffer const & m_buffer;
		uint32_t m_offset;
	};
}
