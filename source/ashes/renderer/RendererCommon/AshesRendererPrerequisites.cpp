/*
See LICENSE file in root folder
*/
#include "AshesRendererPrerequisites.hpp"

namespace ashes
{
	inline VkPipelineColorBlendStateCreateInfo constexpr DeactivatedColorBlendState{ VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO
		, nullptr
		, 0u
		, VK_FALSE
		, VK_LOGIC_OP_COPY
		, 0u // Not sure this is valid
		, nullptr // ...
		, { 0.0f, 0.0f, 0.0f, 0.0f } };
	inline VkPipelineDepthStencilStateCreateInfo constexpr DeactivatedDepthStencilState{ VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO
		, nullptr
		, 0u
		, VK_FALSE
		, VK_FALSE
		, VK_COMPARE_OP_ALWAYS
		, VK_FALSE
		, VK_FALSE
		, { VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_ALWAYS, 0xFFFFFFFFu, 0xFFFFFFFFu, 0u }
		, { VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_ALWAYS, 0xFFFFFFFFu, 0xFFFFFFFFu, 0u }
		, 0.0f
		, 1.0f };
	inline VkPipelineDynamicStateCreateInfo constexpr DeactivatedDynamicState{ VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO
		, nullptr
		, 0u
		, 0u
		, nullptr };
	inline VkPipelineInputAssemblyStateCreateInfo constexpr DeactivatedInputAssemblyState{ VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO
		, nullptr
		, 0u
		, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
		, VK_FALSE };
	inline VkPipelineMultisampleStateCreateInfo constexpr DeactivatedMultisampleState{ VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO
		, nullptr
		, 0u
		, VK_SAMPLE_COUNT_1_BIT
		, VK_FALSE
		, 1.0f
		, nullptr
		, VK_FALSE
		, VK_FALSE };
	inline VkPipelineRasterizationStateCreateInfo constexpr DeactivatedRasterizationState{ VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO
		, nullptr
		, 0u
		, VK_FALSE
		, VK_TRUE	// rasterizerDiscardEnable
		, VK_POLYGON_MODE_FILL
		, VK_CULL_MODE_BACK_BIT
		, VK_FRONT_FACE_COUNTER_CLOCKWISE
		, VK_FALSE
		, 0.0f
		, 0.0f
		, 0.0f
		, 1.0f };
	inline VkPipelineTessellationStateCreateInfo constexpr DeactivatedTessellationState{ VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO
		, nullptr
		, 0u
		, 0u };
	inline VkPipelineViewportStateCreateInfo constexpr DeactivatedViewportState{ VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO
		, nullptr
		, 0u
		, 0u
		, nullptr
		, 0u
		, nullptr };

	VkPipelineColorBlendStateCreateInfo const & getDeactivatedColorBlendState()noexcept
	{
		return DeactivatedColorBlendState;
	}

	VkPipelineDepthStencilStateCreateInfo const & getDeactivatedDepthStencilState()noexcept
	{
		return DeactivatedDepthStencilState;
	}

	VkPipelineDynamicStateCreateInfo const & getDeactivatedDynamicState()noexcept
	{
		return DeactivatedDynamicState;
	}

	VkPipelineInputAssemblyStateCreateInfo const & getDeactivatedInputAssemblyState()noexcept
	{
		return DeactivatedInputAssemblyState;
	}

	VkPipelineMultisampleStateCreateInfo const & getDeactivatedMultisampleState()noexcept
	{
		return DeactivatedMultisampleState;
	}

	VkPipelineRasterizationStateCreateInfo const & getDeactivatedRasterizationState()noexcept
	{
		return DeactivatedRasterizationState;
	}

	VkPipelineTessellationStateCreateInfo const & getDeactivatedTessellationState()noexcept
	{
		return DeactivatedTessellationState;
	}

	VkPipelineViewportStateCreateInfo const & getDeactivatedViewportState()noexcept
	{
		return DeactivatedViewportState;
	}
}
