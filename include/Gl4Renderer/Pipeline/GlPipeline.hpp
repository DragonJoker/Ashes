/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Pipeline_HPP___
#define ___GlRenderer_Pipeline_HPP___
#pragma once

#include "Gl4Renderer/Buffer/GlGeometryBuffers.hpp"
#include "Gl4Renderer/Shader/GlShaderDesc.hpp"
#include "Gl4Renderer/Shader/GlShaderProgram.hpp"

#include <algorithm>
#include <unordered_map>

namespace ashes::gl4
{
	/**
	*\brief
	*	Un pipeline de rendu.
	*/
	class Pipeline
	{
	public:
		/**
		*name
		*	Construction / Destruction.
		*/
		/**@{*/
		Pipeline( VkDevice device
			, VkGraphicsPipelineCreateInfo createInfo );
		Pipeline( VkDevice device
			, VkComputePipelineCreateInfo createInfo );
		GeometryBuffers * findGeometryBuffers( VboBindings const & vbos
			, IboBinding const & ibo )const;
		GeometryBuffersRef createGeometryBuffers( VboBindings vbos
			, IboBinding const & ibo
			, VkIndexType type )const;
		~Pipeline();
		PushConstantsDesc findPushConstantBuffer( PushConstantsDesc const & pushConstants )const;
		/**@}*/
		/**
		*\return
		*	Le InputAssemblyState.
		*/
		inline bool hasInputAssemblyState()const
		{
			return bool( m_inputAssemblyState );
		}
		/**
		*\return
		*	Le InputAssemblyState.
		*/
		inline auto const & getInputAssemblyState()const
		{
			assert( hasInputAssemblyState() );
			return m_inputAssemblyState.value();
		}
		/**
		*\return
		*	Le ColourBlendState.
		*/
		inline bool hasColourBlendState()const
		{
			return bool( m_colorBlendState );
		}
		/**
		*\return
		*	Le ColourBlendState.
		*/
		inline auto const & getColourBlendState()const
		{
			assert( hasColourBlendState() );
			return m_colorBlendState.value();
		}
		/**
		*\return
		*	Le RasterisationState.
		*/
		inline bool hasRasterisationState()const
		{
			return bool( m_rasterizationState );
		}
		/**
		*\return
		*	Le RasterisationState.
		*/
		inline auto const & getRasterisationState()const
		{
			assert( hasRasterisationState() );
			return m_rasterizationState.value();
		}
		/**
		*\return
		*	Le DepthStencilState.
		*/
		inline bool hasDepthStencilState()const
		{
			return bool( m_depthStencilState );
		}
		/**
		*\return
		*	Le DepthStencilState.
		*/
		inline auto const & getDepthStencilState()const
		{
			assert( hasDepthStencilState() );
			return m_depthStencilState.value();
		}
		/**
		*\return
		*	Le MultisampleState.
		*/
		inline bool hasMultisampleState()const
		{
			return bool( m_multisampleState );
		}
		/**
		*\return
		*	Le MultisampleState.
		*/
		inline auto const & getMultisampleState()const
		{
			assert( hasMultisampleState() );
			return m_multisampleState.value();
		}
		/**
		*\return
		*	Le TessellationState.
		*/
		inline bool hasTessellationState()const
		{
			return bool( m_tessellationState );
		}
		/**
		*\return
		*	Le TessellationState.
		*/
		inline auto const & getTessellationState()const
		{
			assert( hasTessellationState() );
			return m_tessellationState.value();
		}
		/**
		*\return
		*	Le VertexInputState.
		*/
		inline bool hasVertexInputState()const
		{
			return bool( m_vertexInputState );
		}
		/**
		*\return
		*	Le VertexInputState.
		*/
		inline auto const & getVertexInputState()const
		{
			assert( hasVertexInputState() );
			return m_vertexInputState.value();
		}
		/**
		*\return
		*	Le Viewport.
		*/
		inline bool hasViewportState()const
		{
			return bool( m_viewportState );
		}
		/**
		*\return
		*	Le Viewport.
		*/
		inline auto const & getViewportState()const
		{
			assert( hasViewportState() );
			return m_viewportState.value();
		}
		/**
		*\return
		*	\p true si le Viewport est défini.
		*/
		inline bool hasViewport()const
		{
			return hasViewportState()
				&& m_viewportState.value().viewportCount > 0;
		}
		/**
		*\return
		*	\p true si le Scissor est défini.
		*/
		inline bool hasScissor()const
		{
			return hasViewportState()
				&& m_viewportState.value().scissorCount > 0;
		}
		/**
		*\return
		*	Les Viewports.
		*/
		inline VkViewportArray const & getViewports()const
		{
			return m_viewports;
		}
		/**
		*\return
		*	Les Scissors.
		*/
		inline VkScissorArray const & getScissors()const
		{
			return m_scissors;
		}
		/**
		*\return
		*	Le PipelineLayout.
		*/
		inline VkPipelineLayout getLayout()const
		{
			return m_layout;
		}
		/**
		*\return
		*	Le ShaderProgram.
		*/
		inline GLuint getProgram()const
		{
			return m_program.getProgram();
		}
		/**
		*\return
		*	Le hash du VertexInputState.
		*/
		inline size_t getVertexInputStateHash()const
		{
			return m_vertexInputStateHash;
		}
		/**
		*\return
		*	\p true si l'état dynamique est dans la liste d'états dynamiques.
		*/
		inline bool hasDynamicStateEnable( VkDynamicState state )const
		{
			if ( bool( m_dynamicState ) )
			{
				auto view = makeArrayView( m_dynamicState.value().pDynamicStates
					, m_dynamicState.value().dynamicStateCount );
				return view.end() != std::find( view.begin()
					, view.end()
					, state );
			}

			return false;
		}

	private:
		VkDevice m_device;
		VkPipelineCreateFlags m_flags;
		VkPipelineShaderStageCreateInfoArray m_stages;
		VkVertexInputBindingDescriptionArray m_vertexBindingDescriptions;
		VkVertexInputAttributeDescriptionArray m_vertexAttributeDescriptions;
		Optional< VkPipelineVertexInputStateCreateInfo > m_vertexInputState;
		Optional< VkPipelineInputAssemblyStateCreateInfo > m_inputAssemblyState;
		Optional< VkPipelineTessellationStateCreateInfo > m_tessellationState;
		VkViewportArray m_viewports;
		VkScissorArray m_scissors;
		Optional< VkPipelineViewportStateCreateInfo > m_viewportState;
		Optional< VkPipelineRasterizationStateCreateInfo > m_rasterizationState;
		Optional< VkSampleMask > m_sampleMask;
		Optional< VkPipelineMultisampleStateCreateInfo > m_multisampleState;
		Optional< VkPipelineDepthStencilStateCreateInfo > m_depthStencilState;
		VkPipelineColorBlendAttachmentStateArray m_colorBlendAttachments;
		Optional< VkPipelineColorBlendStateCreateInfo > m_colorBlendState;
		VkDynamicStateArray m_dynamicStates;
		Optional< VkPipelineDynamicStateCreateInfo > m_dynamicState;
		VkPipelineLayout m_layout;
		VkRenderPass m_renderPass;
		uint32_t m_subpass;
		VkPipeline m_basePipelineHandle;
		int32_t m_basePipelineIndex;
		PushConstantsDesc m_constantsPcb;
		ShaderProgram m_program;
		mutable std::vector< std::pair< size_t, GeometryBuffersPtr > > m_geometryBuffers;
		mutable std::unordered_map< GLuint, BufferDestroyConnection > m_connections;
		size_t m_vertexInputStateHash;
	};
}

#endif
