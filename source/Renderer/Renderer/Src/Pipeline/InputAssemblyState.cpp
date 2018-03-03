/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Pipeline/InputAssemblyState.hpp"

namespace renderer
{
	InputAssemblyState::InputAssemblyState( PrimitiveTopology topology
		, bool primitiveRestartEnable )
		: m_topology{ topology }
		, m_primitiveRestartEnable{ primitiveRestartEnable }
	{
	}
}
