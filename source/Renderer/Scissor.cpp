/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#include "Scissor.hpp"

namespace renderer
{
	Scissor::Scissor( int32_t x
		, int32_t y
		, uint32_t width
		, uint32_t height )
		: m_scissor{ x, y, width, height }
	{
	}
}
