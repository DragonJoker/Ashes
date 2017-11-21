#include "VertexLayout.hpp"

namespace renderer
{
	VertexLayout::VertexLayout( uint32_t bindingSlot )
		: m_layout{ std::make_shared< vk::VertexLayout >( bindingSlot ) }
	{
	}
}
