/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_CommandBuffer_HPP___
#define ___AshesPP_CommandBuffer_HPP___
#pragma once

#include "ashespp/Buffer/PushConstantsBuffer.hpp"

namespace ashes
{
	/**
	*\brief
	*	A command buffer.
	*/
	class CommandBuffer
	{
	private:
		CommandBuffer( CommandBuffer const & ) = delete;
		CommandBuffer & operator=( CommandBuffer const & ) = delete;

	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] pool
		*	The parent command buffer pool.
		*\param[in] level
		*	The Vulkan command buffer level.
		*/
		CommandBuffer( Device const & device
			, CommandPool const & pool
			, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] pool
		*	The parent command buffer pool.
		*\param[in] level
		*	The Vulkan command buffer level.
		*/
		CommandBuffer( Device const & device
			, std::string const & debugName
			, CommandPool const & pool
			, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY );
		/**
		*\brief
		*	Destructor.
		*/
		~CommandBuffer();
		/**
		*\brief
		*	Starts recording the command buffer.
		*/
		void begin( VkCommandBufferBeginInfo const & info )const;
		/**
		*\brief
		*	Starts recording the command buffer.
		*\param[in] flags
		*	The usage flags for the command buffer.
		*/
		inline void begin( VkCommandBufferUsageFlags flags = 0u )const
		{
			begin( VkCommandBufferBeginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, nullptr, flags, nullptr } );
		}
		/**
		*\brief
		*	Starts recording the command buffer as a secondary command buffer.
		*\param[in] flags
		*	The usage flags for the command buffer.
		*\param[in] inheritanceInfo
		*	The inheritance informations.
		*/
		inline void begin( VkCommandBufferUsageFlags flags
			, VkCommandBufferInheritanceInfo const & inheritanceInfo )const
		{
			begin( VkCommandBufferBeginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, nullptr, flags, &inheritanceInfo } );
		}
		/**
		*\brief
		*	Ends the command buffer recording.
		*/
		void end()const;
		/**
		*\brief
		*	Reset a command buffer to the initial state.
		*\param[in] flags
		*	The flags controlling the reset operation.
		*/
		void reset( VkCommandBufferResetFlags flags = 0u )const;
		/**
		*\brief
		*	Begins a new render pass.
		*/
		void beginRenderPass( VkRenderPassBeginInfo const & beginInfo
			, VkSubpassContents contents )const;
		/**
		*\brief
		*	Begins a new render pass.
		*\param[in] renderPass
		*	The render pass to begin.
		*\param[in] frameBuffer
		*	The framebuffer containing the attachments that are used with the render pass.
		*\param[in] clearValues
		*	The clear values for each attachment that needs to be cleared.
		*\param[in] contents
		*	Specifies how the commands in the first subpass will be provided.
		*/
		void beginRenderPass( RenderPass const & renderPass
			, FrameBuffer const & frameBuffer
			, VkClearValueArray const & clearValues
			, VkSubpassContents contents )const;
		/**
		*\brief
		*	Transition to the next subpass of a render pass.
		*\param[in] contents
		*	Specifies how the commands in the next subpass will be provided.
		*/
		void nextSubpass( VkSubpassContents contents )const;
		/**
		*\brief
		*	End the current render pass.
		*/
		void endRenderPass()const;
		/**
		*\brief
		*	Execute a secondary command buffer.
		*\param[in] commands
		*	The secondary command buffers, which are recorded to execute in the order they are listed in the array.
		*/
		void executeCommands( CommandBufferCRefArray const & commands )const;
		/**
		*\brief
		*	Clear a color image.
		*\param[in] image
		*	The image to clear.
		*\param[in] colour
		*	The clear colour.
		*/
		void clear( ImageView const & image
			, VkClearColorValue const & colour )const;
		/**
		*\brief
		*	Clear a depth and/or stencil image.
		*\param[in] image
		*	The image to clear.
		*\param[in] value
		*	The clear value.
		*/
		void clear( ImageView const & image
			, VkClearDepthStencilValue const & value )const;
		/**
		*\brief
		*	Clear regions within currently bound framebuffer attachments.
		*\param[in] clearAttachments
		*	The attachments to clear and the clear values to use.
		*\param[in] clearRects
		*	The regions within each selected attachment to clear.
		*/
		void clearAttachments( VkClearAttachmentArray const & clearAttachments
			, VkClearRectArray const & clearRects );
		/**
		*\brief
		*	Defines a memory dependency between commands that were submitted before it, and those submitted after it.
		*\param[in] after
		*	Specifies the pipeline stages that must be ended before the barrier.
		*\param[in] before
		*	Specifies the pipeline stages that can be started after the barrier.
		*\param[in] transitionBarrier
		*	Describes the transition.
		*/
		void pipelineBarrier( VkPipelineStageFlags after
			, VkPipelineStageFlags before
			, VkDependencyFlags dependencyFlags
			, VkMemoryBarrierArray const & memoryBarriers
			, VkBufferMemoryBarrierArray const & bufferMemoryBarriers
			, VkImageMemoryBarrierArray const & imaegMemoryBarriers )const;
		/**
		*\brief
		*	Defines a memory dependency between commands that were submitted before it, and those submitted after it.
		*\param[in] after
		*	Specifies the pipeline stages that must be ended before the barrier.
		*\param[in] before
		*	Specifies the pipeline stages that can be started after the barrier.
		*\param[in] transitionBarrier
		*	Describes the transition.
		*/
		void memoryBarrier( VkPipelineStageFlags after
			, VkPipelineStageFlags before
			, VkBufferMemoryBarrier const & transitionBarrier )const;
		/**
		*\brief
		*	Defines a memory dependency between commands that were submitted before it, and those submitted after it.
		*\param[in] after
		*	Specifies the pipeline stages that must be ended before the barrier.
		*\param[in] before
		*	Specifies the pipeline stages that can be started after the barrier.
		*\param[in] transitionBarrier
		*	Describes the transition.
		*/
		void memoryBarrier( VkPipelineStageFlags after
			, VkPipelineStageFlags before
			, VkImageMemoryBarrier const & transitionBarrier )const;
		/**
		*\brief
		*	Binds a pipeline to the command buffer.
		*\param[in] pipeline
		*	The pipeline to be bound.
		*\param[in] bindingPoint
		*	Specifies whether to bind to the compute or graphics bind point.
		*/
		void bindPipeline( Pipeline const & pipeline
			, VkPipelineBindPoint bindingPoint )const;
		/**
		*\brief
		*	Binds a pipeline to the command buffer.
		*\param[in] pipeline
		*	The pipeline to be bound.
		*\param[in] bindingPoint
		*	Specifies whether to bind to the compute or graphics bind point.
		*/
		void bindPipeline( GraphicsPipeline const & pipeline )const;
		/**
		*\brief
		*	Binds a compute pipeline to the command buffer.
		*\param[in] pipeline
		*	The pipeline to be bound.
		*\param[in] bindingPoint
		*	Specifies whether to bind to the compute or graphics bind point.
		*/
		void bindPipeline( ComputePipeline const & pipeline )const;
		/**
		*\brief
		*	Binds vertex buffers to the command buffer.
		*\param[in] firstBinding
		*	The index of the first vertex input binding whose state is updated by the command.
		*\param[in] buffers
		*	The array of buffer handles.
		*\param[in] offsets
		*	The array of buffer offsets.
		*/
		void bindVertexBuffers( uint32_t firstBinding
			, BufferCRefArray const & buffers
			, UInt64Array offsets )const;
		/**
		*\brief
		*	Binds an index buffer to the command buffer.
		*\param[in] buffer
		*	The buffer being bound.
		*\param[in] offset
		*	The starting offset in bytes within buffer used in index buffer address calculations.
		*\param[in] indexType
		*	Tells whether the indices are treated as 16 bits or 32 bits.
		*/
		void bindIndexBuffer( BufferBase const & buffer
			, uint64_t offset
			, VkIndexType indexType )const;
		/**
		*\brief
		*	Binds descriptor sets to the command buffer.
		*\param[in] descriptorSet
		*	The descriptor sets.
		*\param[in] layout
		*	The pipeline layout used to program the binding.
		*\param[in] dynamicOffsets
		*	The dynamic offsets for dynamic buffers.
		*\param[in] bindingPoint
		*	Indicates whether the descriptor wil be used by graphics or compute pipeline.
		*/
		void bindDescriptorSets( DescriptorSetCRefArray const & descriptorSet
			, PipelineLayout const & layout
			, UInt32Array const & dynamicOffsets
			, VkPipelineBindPoint bindingPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS )const;
		/**
		*\brief
		*	Defines the currently bound pipeline viewport.
		*\remarks
		*	This action is possible only if the viewport is dynamic, in the pipeline.
		*\param[in] firstViewport
		*	The first viewport index.
		*\param[in] viewports
		*	The viewports.
		*/
		void setViewport( uint32_t firstViewport
			, VkViewportArray const & viewports )const;
		/**
		*\brief
		*	Defines the currently bound pipeline viewport.
		*\remarks
		*	This action is possible only if the viewport is dynamic, in the pipeline.
		*\param[in] viewport
		*	The viewport.
		*/
		inline void setViewport( VkViewport const & viewport )const
		{
			setViewport( 0u, VkViewportArray{ viewport } );
		}
		/**
		*\brief
		*	Defines the currently bound pipeline scissor.
		*\remarks
		*	This action is possible only if the scissor is dynamic, in the pipeline.
		*\param[in] firstScissor
		*	The first scissor index.
		*\param[in] scissors
		*	The scissors.
		*/
		void setScissor( uint32_t firstScissor
			, VkScissorArray const & scissors )const;
		/**
		*\brief
		*	Defines the currently bound pipeline scissor.
		*\remarks
		*	This action is possible only if the scissor is dynamic, in the pipeline.
		*\param[in] scissor
		*	The scissor.
		*/
		inline void setScissor( VkRect2D const & scissor )const
		{
			setScissor( 0u, VkScissorArray{ scissor } );
		}
		/**
		*\brief
		*	Draws some vertices.
		*\param[in] vtxCount
		*	The vertex count.
		*\param[in] instCount
		*	The instances count.
		*\param[in] firstVertex
		*	The first vertex index.
		*\param[in] firstInstance
		*	The first instance index.
		*/
		void draw( uint32_t vtxCount
			, uint32_t instCount = 1u
			, uint32_t firstVertex = 0u
			, uint32_t firstInstance = 0u )const;
		/**
		*\brief
		*	Draws some indexed vertices.
		*\param[in] indexCount
		*	The index count.
		*\param[in] instCount
		*	The instances count.
		*\param[in] firstIndex
		*	The first index index.
		*\param[in] vertexOffset
		*	The offset added to the vertex index, before indexing the vertex buffer.
		*\param[in] firstInstance
		*	The first instance index.
		*/
		void drawIndexed( uint32_t indexCount
			, uint32_t instCount = 1u
			, uint32_t firstIndex = 0u
			, uint32_t vertexOffset = 0u
			, uint32_t firstInstance = 0u )const;
		/**
		*\brief
		*	Perform an indirect draw.
		*\param[in] buffer
		*	The buffer containing draw parameters.
		*\param[in] offset
		*	The byte offset into \p buffer where parameters begin.
		*\param[in] drawCount
		*	The number of draws to execute, and can be zero.
		*\param[in] stride
		*	The byte stride between successive sets of draw parameters.
		*/
		void drawIndirect( BufferBase const & buffer
			, uint32_t offset
			, uint32_t drawCount
			, uint32_t stride )const;
		/**
		*\brief
		*	Perform an indexed indirect draw.
		*\param[in] buffer
		*	The buffer containing draw parameters.
		*\param[in] offset
		*	The byte offset into \p buffer where parameters begin.
		*\param[in] drawCount
		*	The number of draws to execute, and can be zero.
		*\param[in] stride
		*	The byte stride between successive sets of draw parameters.
		*/
		void drawIndexedIndirect( BufferBase const & buffer
			, uint32_t offset
			, uint32_t drawCount
			, uint32_t stride )const;
		/**
		*\brief
		*	Copies data from a buffer to an image.
		*\param[in] copyInfo
		*	The copy informations.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination image.
		*/
		void copyToImage( VkBufferImageCopyArray const & copyInfo
			, BufferBase const & src
			, Image const & dst )const;
		/**
		*\brief
		*	Copies data from a buffer to an image.
		*\param[in] copyInfo
		*	The copy informations.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination image.
		*/
		void copyToBuffer( VkBufferImageCopyArray const & copyInfo
			, Image const & src
			, BufferBase const & dst )const;
		/**
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] copyInfo
		*	The copy informations.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*/
		void copyBuffer( VkBufferCopy const & copyInfo
			, BufferBase const & src
			, BufferBase const & dst )const;
		/**
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] copyInfo
		*	The copy informations.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*/
		void copyBuffer( VkBufferCopyArray const & copyInfo
			, BufferBase const & src
			, BufferBase const & dst )const;
		/**
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] copyInfo
		*	The copy informations.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*/
		void updateBuffer( BufferBase const & dst
			, VkDeviceSize dstOffset
			, ArrayView< uint8_t > const & data )const;
		/**
		*\brief
		*	Copies data from an image to another one.
		*\param[in] copyInfo
		*	The copy informations.
		*\param[in] src
		*	The source image.
		*\param[in] srcLayout
		*	The image layout wanted for the source image.
		*\param[in] dst
		*	The destination image.
		*\param[in] dstLayout
		*	The image layout wanted for the destination image.
		*/
		void copyImage( VkImageCopy const & copyInfo
			, Image const & src
			, VkImageLayout srcLayout
			, Image const & dst
			, VkImageLayout dstLayout )const;
		/**
		*\brief
		*	Copies data from an image to another one.
		*\param[in] copyInfo
		*	The copy informations.
		*\param[in] src
		*	The source image.
		*\param[in] srcLayout
		*	The image layout wanted for the source image.
		*\param[in] dst
		*	The destination image.
		*\param[in] dstLayout
		*	The image layout wanted for the destination image.
		*/
		void copyImage( VkImageCopyArray copyInfo
			, Image const & src
			, VkImageLayout srcLayout
			, Image const & dst
			, VkImageLayout dstLayout )const;
		/**
		*\brief
		*	Copy regions of an image to another one.
		*\param[in] regions
		*	The regions to blit.
		*\param[in] srcImage
		*	The source image.
		*\param[in] srcLayout
		*	The source image layout.
		*\param[in] dstImage
		*	The destination image.
		*\param[in] dstLayout
		*	The source image layout.
		*\param[in] filter
		*	The filter applied if the blit requires scaling.
		*/
		void blitImage( Image const & srcImage
			, VkImageLayout srcLayout
			, Image const & dstImage
			, VkImageLayout dstLayout
			, std::vector< VkImageBlit > const & regions
			, VkFilter filter )const;
		/**
		*\brief
		*	Resets a range of queries in a query pool.
		*\param[in] pool
		*	The query pool.
		*\param[in] firstQuery
		*	The first query index.
		*\param[in] queryCount
		*	The number of queries (starting at \p firstQuery).
		*/
		void resetQueryPool( QueryPool const & pool
			, uint32_t firstQuery
			, uint32_t queryCount )const;
		/**
		*\brief
		*	Begins a query.
		*\param[in] pool
		*	The query pool.
		*\param[in] query
		*	The query index.
		*\param[in] flags
		*	The constraints on the types of queries that can be performed.
		*/
		void beginQuery( QueryPool const & pool
			, uint32_t query
			, VkQueryControlFlags flags )const;
		/**
		*\brief
		*	Ends a query.
		*\param[in] pool
		*	The query pool.
		*\param[in] query
		*	The query index.
		*/
		void endQuery( QueryPool const & pool
			, uint32_t query )const;
		/**
		*\brief
		*	Writes a timestamp to given query result.
		*\param[in] pipelineStage
		*	Specifies the stage of the pipeline.
		*\param[in] pool
		*	The query pool.
		*\param[in] query
		*	The query index.
		*/
		void writeTimestamp( VkPipelineStageFlagBits pipelineStage
			, QueryPool const & pool
			, uint32_t query )const;
		/**
		*\brief
		*	Updates the values of push constants.
		*\param[in] layout
		*	The pipeline layout used to program the push constants updates.
		*\param[in] stageFlags
		*	The impacted shader stages.
		*\param[in] offset
		*	The range offset (in bytes).
		*\param[in] size
		*	The range size (in bytes).
		*\param[in] data
		*	The push constants data.
		*/
		void pushConstants( PipelineLayout const & layout
			, VkShaderStageFlags stageFlags
			, uint32_t offset
			, uint32_t size
			, void const * data )const;
		/**
		*\brief
		*	Dispatch compute work items.
		*\param[in] groupCountX, groupCountY, groupCountZ
		*	The number of local workgroups to dispatch to the X, Y, and Z dimensions.
		*/
		void dispatch( uint32_t groupCountX
			, uint32_t groupCountY
			, uint32_t groupCountZ )const;
		/**
		*\brief
		*	Dispatch compute work items using indirect parameters.
		*\param[in] buffer
		*	The buffer containing dispatch parameters.
		*\param[in] offset
		*	The byte offset into \p buffer where parameters begin.
		*/
		void dispatchIndirect( BufferBase const & buffer
			, uint32_t offset )const;
		/**
		*\brief
		*	Specifies the lines width, if this is a dynamic state in the pipeline.
		*\param[in] width
		*	The wanted width.
		*/
		void setLineWidth( float width )const;
		/**
		*\brief
		*	Specifies the lines width, if this is a dynamic state in the pipeline.
		*\param[in] constantFactor
		*	The scalar factor controlling the constant depth value added to each fragment.
		*\param[in] clamp
		*	The maximum (or minimum) depth bias of a fragment.
		*\param[in] slopeFactor
		*	The scalar factor applied to a fragment’s slope in depth bias calculations.
		*/
		void setDepthBias( float constantFactor
			, float clamp
			, float slopeFactor )const;
		/**
		*\brief
		*	Sets an event to signaled state.
		*\param[in] event
		*	The event to be signaled.
		*\param[in] stageMask
		*	The source stage mask used to determine when the event is signaled.
		*/
		void setEvent( Event const & event
			, VkPipelineStageFlags stageMask )const;
		/**
		*\brief
		*	Sets an event to signaled state.
		*\param[in] event
		*	The event to be signaled.
		*\param[in] stageMask
		*	The source stage mask used to determine when the event is signaled.
		*/
		void resetEvent( Event const & event
			, VkPipelineStageFlags stageMask )const;
		/**
		*\brief
		*	Wait for one or more events to be signaled on a device.
		*\param[in] events
		*	The events to wait on.
		*\param[in] srcStageMask
		*	The source stage mask.
		*\param[in] dstStageMask
		*	destination stage mask.
		*\param[in] bufferMemoryBarriers
		*	An array of buffer memory barriers.
		*\param[in] imageMemoryBarriers
		*	An array of image memory barriers.
		*/
		void waitEvents( EventCRefArray const & events
			, VkPipelineStageFlags srcStageMask
			, VkPipelineStageFlags dstStageMask
			, VkMemoryBarrierArray const & memoryBarriers
			, VkBufferMemoryBarrierArray const & bufferMemoryBarriers
			, VkImageMemoryBarrierArray const & imageMemoryBarriers )const;
		/**
		*\brief
		*	Binds a vertex buffer to the command buffer.
		*\param[in] binding
		*	The index of the vertex input binding whose state is updated by the command.
		*\param[in] buffer
		*	The buffer handle.
		*\param[in] offset
		*	The array of buffer offset.
		*/
		void bindVertexBuffer( uint32_t binding
			, BufferBase const & buffer
			, uint64_t offset )const;
		/**
		*\brief
		*	Copies data from a buffer to an image.
		*\param[in] copyInfo
		*	The copy informations.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination image.
		*/
		void copyToImage( VkBufferImageCopy const & copyInfo
			, BufferBase const & src
			, Image const & dst )const;
		/**
		*\brief
		*	Copies data from a buffer to an image.
		*\param[in] copyInfo
		*	The copy informations.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination image.
		*/
		void copyToBuffer( VkBufferImageCopy const & copyInfo
			, Image const & src
			, BufferBase const & dst )const;
		/**
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*\param[in] size
		*	The byte size of the data to copy.
		*\param[in] offset
		*	The offset in the source buffer.
		*/
		void copyBuffer( BufferBase const & src
			, BufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
		/**
		*\brief
		*	Binds a descriptor set to the command buffer.
		*\param[in] descriptorSet
		*	The descriptor set.
		*\param[in] layout
		*	The pipeline layout used to program the binding.
		*\param[in] dynamicOffsets
		*	The dynamic offsets for dynamic buffers.
		*\param[in] bindingPoint
		*	Indicates whether the descriptor wil be used by graphics or compute pipeline.
		*/
		inline void bindDescriptorSet( DescriptorSet const & descriptorSet
			, PipelineLayout const & layout
			, UInt32Array const & dynamicOffsets
			, VkPipelineBindPoint bindingPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS )const
		{
			bindDescriptorSets( DescriptorSetCRefArray{ descriptorSet }
				, layout
				, dynamicOffsets
				, bindingPoint );
		}
		/**
		*\brief
		*	Binds a descriptor set to the command buffer.
		*\param[in] descriptorSet
		*	The descriptor set.
		*\param[in] layout
		*	The pipeline layout used to program the binding.
		*\param[in] bindingPoint
		*	Indicates whether the descriptor wil be used by graphics or compute pipeline.
		*/
		inline void bindDescriptorSet( DescriptorSet const & descriptorSet
			, PipelineLayout const & layout
			, VkPipelineBindPoint bindingPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS )const
		{
			bindDescriptorSets( DescriptorSetCRefArray{ descriptorSet }
				, layout
				, UInt32Array{}
				, bindingPoint );
		}
		/**
		*\brief
		*	Binds descriptor sets to the command buffer.
		*\param[in] descriptorSet
		*	The descriptor sets.
		*\param[in] layout
		*	The pipeline layout used to program the binding.
		*\param[in] bindingPoint
		*	Indicates whether the descriptor wil be used by graphics or compute pipeline.
		*/
		inline void bindDescriptorSets( DescriptorSetCRefArray const & descriptorSet
			, PipelineLayout const & layout
			, VkPipelineBindPoint bindingPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS )const
		{
			bindDescriptorSets( descriptorSet
				, layout
				, UInt32Array{}
				, bindingPoint );
		}
		/**
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*\param[in] size
		*	The byte size of the data to copy.
		*\param[in] offset
		*	The offset in the source buffer.
		*/
		void copyBuffer( BufferBase const & src
			, VertexBufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
		/**
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*\param[in] size
		*	The byte size of the data to copy.
		*\param[in] offset
		*	The offset in the source buffer.
		*/
		void copyBuffer( VertexBufferBase const & src
			, BufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
		/**
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*\param[in] size
		*	The byte size of the data to copy.
		*\param[in] offset
		*	The offset in the source buffer.
		*/
		void copyBuffer( VertexBufferBase const & src
			, VertexBufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
		/**
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*\param[in] size
		*	The byte size of the data to copy.
		*\param[in] offset
		*	The offset in the source buffer.
		*/
		void copyBuffer( BufferBase const & src
			, UniformBuffer const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
		/**
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*\param[in] size
		*	The byte size of the data to copy.
		*\param[in] offset
		*	The offset in the source buffer.
		*/
		void copyBuffer( UniformBuffer const & src
			, BufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
		/**
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*\param[in] size
		*	The byte size of the data to copy.
		*\param[in] offset
		*	The offset in the source buffer.
		*/
		void copyBuffer( UniformBuffer const & src
			, UniformBuffer const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
		/**
		*\brief
		*	Copies data from an image to another one.
		*\param[in] src
		*	The source image.
		*\param[in] dst
		*	The destination image.
		*/
		void copyImage( ImageView const & src
			, ImageView const & dst )const;
		/**
		*\brief
		*	Updates the values of push constants.
		*\param[in] layout
		*	The pipeline layout used to program the push constants updates.
		*\param[in] pcb
		*	The push constants buffer.
		*/
		void pushConstants( PipelineLayout const & layout
			, PushConstantsBufferBase const & pcb )const;
		/**
		*\brief
		*	Updates the values of push constants.
		*\param[in] layout
		*	The pipeline layout used to program the push constants updates.
		*\param[in] pcb
		*	The push constants buffer.
		*/
		template< typename T >
		inline void pushConstants( PipelineLayout const & layout
			, PushConstantsBuffer< T > const & pcb )const
		{
			pushConstants( layout, pcb.getBuffer() );
		}
#if VK_EXT_debug_utils
		/**
		*\brief
		*	Begins a command buffer label.
		*\param[in] labelInfo
		*	The parameters of the label to begin.
		*/
		void beginDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const;
		/**
		*\brief
		*	Ends the command label.
		*/
		void endDebugUtilsLabel()const;
		/**
		*\brief
		*	Inserts a command label.
		*\param[in] labelInfo
		*	The parameters of the label to begin.
		*/
		void insertDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const;
#endif
#if VK_EXT_debug_marker
		/**
		*\brief
		*	Begins a command buffer label.
		*\param[in] labelInfo
		*	The parameters of the label to begin.
		*/
		void debugMarkerBegin( VkDebugMarkerMarkerInfoEXT const & labelInfo )const;
		/**
		*\brief
		*	Ends the command label.
		*/
		void debugMarkerEnd()const;
		/**
		*\brief
		*	Inserts a command label.
		*\param[in] labelInfo
		*	The parameters of the label to begin.
		*/
		void debugMarkerInsert( VkDebugMarkerMarkerInfoEXT const & labelInfo )const;
#endif
#if VK_EXT_debug_utils || VK_EXT_debug_marker
		/**
		*\brief
		*	Begins a command buffer label.
		*\param[in] labelInfo
		*	The parameters of the label to begin.
		*/
		void beginDebugBlock( DebugBlockInfo const & labelInfo )const;
		/**
		*\brief
		*	Ends the command label.
		*/
		void endDebugBlock()const;
		/**
		*\brief
		*	Inserts a command label.
		*\param[in] labelInfo
		*	The parameters of the label to begin.
		*/
		void insertDebugBlock( DebugBlockInfo const & labelInfo )const;
#endif
		/**
		*\brief
		*	VkCommandBuffer implicit cast operator.
		*/
		inline operator VkCommandBuffer const & ()const
		{
			return m_internal;
		}

	private:
		Device const & m_device;
		CommandPool const & m_pool;
		VkCommandBuffer m_internal{ VK_NULL_HANDLE };
	};
}

#endif
