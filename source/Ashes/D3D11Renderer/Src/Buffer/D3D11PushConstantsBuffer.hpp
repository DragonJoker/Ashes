/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___D3D11Renderer_PushConstantsBuffer_HPP___
#define ___D3D11Renderer_PushConstantsBuffer_HPP___
#pragma once

#include "D3D11UniformBuffer.hpp"
#include "D3D11Buffer.hpp"

#include <Buffer/PushConstantsBuffer.hpp>

namespace d3d11_renderer
{
	class PushConstantsBuffer
		: public ashes::PushConstantsBufferBase
	{
	public:
		PushConstantsBuffer( Device const & device
			, uint32_t location
			, ashes::ShaderStageFlags stageFlags
			, ashes::PushConstantArray const & variables );
		~PushConstantsBuffer();

		inline ID3D11Buffer * getBuffer()const
		{
			return static_cast< Buffer const & >( m_buffer.getBuffer() ).getBuffer();
		}

	private:
		UniformBuffer m_buffer;
	};
}

#endif
