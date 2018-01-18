/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "RenderPass/FrameBuffer.hpp"

#include "Command/Queue.hpp"
#include "Image/Texture.hpp"
#include "RenderPass/RenderPass.hpp"

namespace renderer
{
	FrameBuffer::FrameBuffer()
	{
	}

	FrameBuffer::FrameBuffer( RenderPass const & renderPass
		, IVec2 const & dimensions
		, TextureCRefArray const & textures )
	{
	}
}
