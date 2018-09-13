/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "D3D11PushConstantsBuffer.hpp"
#include "Core/D3D11Device.hpp"

namespace d3d11_renderer
{
	PushConstantsBuffer::PushConstantsBuffer( Device const & device
		, uint32_t location
		, renderer::ShaderStageFlags stageFlags
		, renderer::PushConstantArray const & variables )
		: renderer::PushConstantsBufferBase{ location, stageFlags, variables }
		, m_buffer{ device
			, 1u
			, getSize()
			, renderer::BufferTarget::eTransferSrc
			, renderer::MemoryPropertyFlag::eHostVisible }
	{
	}

	PushConstantsBuffer::~PushConstantsBuffer()
	{
	}
}
