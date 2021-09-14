/*
See LICENSE file in root folder
*/
#include "AshesRendererPrerequisites.hpp"

namespace ashes
{
	VkPipelineColorBlendStateCreateInfo const & getDeactivatedColorBlendState()noexcept
	{
		static VkPipelineColorBlendStateCreateInfo const value
		{
			VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			nullptr,
			0u,
			VK_FALSE,
			VK_LOGIC_OP_COPY,
			0u,			// Not sure this is valid
			nullptr,	// ...
			{ 0.0f, 0.0f, 0.0f, 0.0f }
		};
		return value;
	}

	VkPipelineDepthStencilStateCreateInfo const & getDeactivatedDepthStencilState()noexcept
	{
		static VkPipelineDepthStencilStateCreateInfo const value
		{
			VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			nullptr,
			0u,
			VK_FALSE,
			VK_FALSE,
			VK_COMPARE_OP_ALWAYS,
			VK_FALSE,
			VK_FALSE,
			{
				VK_STENCIL_OP_KEEP,
				VK_STENCIL_OP_KEEP,
				VK_STENCIL_OP_KEEP,
				VK_COMPARE_OP_ALWAYS,
				0xFFFFFFFFu,
				0xFFFFFFFFu,
				0u,
		},
			{
				VK_STENCIL_OP_KEEP,
				VK_STENCIL_OP_KEEP,
				VK_STENCIL_OP_KEEP,
				VK_COMPARE_OP_ALWAYS,
				0xFFFFFFFFu,
				0xFFFFFFFFu,
				0u,
			},
			0.0f,
			1.0f,
		};
		return value;
	}

	VkPipelineDynamicStateCreateInfo const & getDeactivatedDynamicState()noexcept
	{
		static VkPipelineDynamicStateCreateInfo const value
		{
			VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			nullptr,
			0u,
			0u,
			nullptr,
		};
		return value;
	}

	VkPipelineInputAssemblyStateCreateInfo const & getDeactivatedInputAssemblyState()noexcept
	{
		static VkPipelineInputAssemblyStateCreateInfo const value
		{
			VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			nullptr,
			0u,
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VK_FALSE,
		};
		return value;
	}

	VkPipelineMultisampleStateCreateInfo const & getDeactivatedMultisampleState()noexcept
	{
		static VkPipelineMultisampleStateCreateInfo const value
		{
			VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			nullptr,
			0u,
			VK_SAMPLE_COUNT_1_BIT,
			VK_FALSE,
			1.0f,
			nullptr,
			VK_FALSE,
			VK_FALSE,
		};
		return value;
	}

	VkPipelineRasterizationStateCreateInfo const & getDeactivatedRasterizationState()noexcept
	{
		static VkPipelineRasterizationStateCreateInfo const value
		{
			VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			nullptr,
			0u,
			VK_FALSE,
			VK_TRUE,	// rasterizerDiscardEnable.
			VK_POLYGON_MODE_FILL,
			VK_CULL_MODE_BACK_BIT,
			VK_FRONT_FACE_COUNTER_CLOCKWISE,
			VK_FALSE,
			0.0f,
			0.0f,
			0.0f,
			1.0f,
		};
		return value;
	}

	VkPipelineTessellationStateCreateInfo const & getDeactivatedTessellationState()noexcept
	{
		static VkPipelineTessellationStateCreateInfo const value
		{
			VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
			nullptr,
			0u,
			0u,
		};
		return value;
	}

	VkPipelineViewportStateCreateInfo const & getDeactivatedViewportState()noexcept
	{
		static VkPipelineViewportStateCreateInfo const value
		{
			VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			nullptr,
			0u,
			0u,
			nullptr,
			0u,
			nullptr,
		};
		return value;
	}
}
