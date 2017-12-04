#include "VertexLayout.hpp"

namespace renderer
{
	VertexLayout::VertexLayout( uint32_t bindingSlot
		, uint32_t stride )
		: m_layout{ std::make_shared< vk::VertexLayout >( bindingSlot
			, stride ) }
	{
	}
}
