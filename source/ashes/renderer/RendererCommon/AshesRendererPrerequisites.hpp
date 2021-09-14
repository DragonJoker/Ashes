/*
See LICENSE file in root folder
*/
#pragma once

#include <ashes/ashes.hpp>

#include <cassert>
#include <map>
#include <memory>
#include <string>

#ifndef NDEBUG
#	define declareDebugVariable( Type, Name, Value )\
	mutable Type Name{ Value }
#	define setDebugValue( Name, Value )\
	Name = Value;
#	define assertDebugValue( Name, Value )\
	assert( Name == Value )
#else
#	define declareDebugVariable( Type, Name, Value )
#	define setDebugValue( Name, Value )
#	define assertDebugValue( Name, Value )
#endif

namespace ashes
{
	VkPipelineColorBlendStateCreateInfo const & getDeactivatedColorBlendState()noexcept;
	VkPipelineDepthStencilStateCreateInfo const & getDeactivatedDepthStencilState()noexcept;
	VkPipelineDynamicStateCreateInfo const & getDeactivatedDynamicState()noexcept;
	VkPipelineInputAssemblyStateCreateInfo const & getDeactivatedInputAssemblyState()noexcept;
	VkPipelineMultisampleStateCreateInfo const & getDeactivatedMultisampleState()noexcept;
	VkPipelineRasterizationStateCreateInfo const & getDeactivatedRasterizationState()noexcept;
	VkPipelineTessellationStateCreateInfo const & getDeactivatedTessellationState()noexcept;
	VkPipelineViewportStateCreateInfo const & getDeactivatedViewportState()noexcept;
}
