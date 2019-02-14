/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace d3d11_renderer
{
	class DispatchIndirectCommand
		: public CommandBase
	{
	public:
		DispatchIndirectCommand( Device const & device
			, ashes::BufferBase const & buffer
			, uint32_t offset );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		Buffer const & m_buffer;
		uint32_t m_offset;
	};
}
