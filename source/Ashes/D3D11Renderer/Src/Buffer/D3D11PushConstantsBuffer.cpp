/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "D3D11PushConstantsBuffer.hpp"
#include "Core/D3D11Device.hpp"

namespace d3d11_renderer
{
	PushConstantsBuffer::PushConstantsBuffer( Device const & device
		, uint32_t location
		, ashes::ShaderStageFlags stageFlags
		, ashes::PushConstantArray const & variables )
		: ashes::PushConstantsBufferBase{ location, stageFlags, variables }
		, m_buffer{ device
			, 1u
			, getSize()
			, ashes::BufferTarget::eTransferSrc
			, ashes::MemoryPropertyFlag::eHostVisible }
	{
	}

	PushConstantsBuffer::~PushConstantsBuffer()
	{
	}
}
