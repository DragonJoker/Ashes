/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Command/Commands/D3D11CommandBase.hpp"

#include <array>

namespace ashes::d3d11
{
	struct BlitPipeline
	{
		VkDevice device;
		VkDescriptorSetLayout descriptorLayout;
		VkPipelineLayout pipelineLayout;
		UInt32Array spirv;
		VkShaderModule shader;
		VkPipeline pipeline;

		BlitPipeline( Device const & device
			, VkFormat src
			, VkFormat dst );
		~BlitPipeline();
	};

	class BlitImageCommand
		: public CommandBase
	{
	public:
		struct Attachment
		{
			Attachment( VkDevice device
				, VkImageSubresourceLayers & subresource
				, VkImage image
				, uint32_t layer
				, bool dest );
			~Attachment();

			VkDevice device;
			VkImageView view;
			ID3D11Resource * image;
			UINT subResourceIndex;
		};
		struct LayerCopy
		{
			LayerCopy( VkDevice device
				, VkDescriptorPool pool
				, VkDescriptorSetLayout descriptorLayout
				, VkBuffer ubo
				, VkDeviceSize offset
				, VkDeviceSize range
				, VkSampler sampler
				, VkImageBlit region
				, VkImage srcImage
				, VkImage dstImage
				, uint32_t layer );

			VkOffset3D dstOffset;
			D3D11_BOX srcBox;
			D3D11_BOX dstBox;
			Attachment src;
			Attachment dst;
			VkDescriptorSet set;
			uint8_t * buffer;
			struct BlitBox
			{
				std::array< float, 4u > srcBox;
				std::array< float, 4u > dstBox;
			};
			BlitBox blitBox;
		};

	public:
		BlitImageCommand( VkCommandPool pool
			, VkCommandBuffer cb
			, VkDevice device
			, BlitPipeline const & pipeline
			, VkImage srcImage
			, VkImage dstImage
			, ArrayView< VkImageBlit const > const & regions
			, VkFilter filter );
		~BlitImageCommand()override;

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		void doInitialiseStretchUbo( VkDescriptorSetLayout descriptorLayout
			, uint32_t count );
		void doInitialiseStretches( VkDescriptorSetLayout descriptorLayout
			, uint32_t count );
		VkImage doUpdateTmpSrc();
		void doUpdateTmpDst();
		void doAddBlits( VkImage srcTexture );
		void doAddStretches( VkPipeline pipeline
			, VkPipelineLayout pipelineLayout
			, uint32_t srcMinLevel
			, uint32_t srcMaxLevel
			, uint32_t dstMinLevel
			, uint32_t dstMaxLevel );
		void doCreateCommandBuffer( VkCommandPool pool
			, VkPipeline pipeline
			, VkPipelineLayout pipelineLayout
			, uint32_t srcMinLevel
			, uint32_t srcMaxLevel
			, uint32_t dstMinLevel
			, uint32_t dstMaxLevel );

	private:
		VkImage m_srcTexture{};
		VkImage m_dstTexture{};
		VkDeviceMemory m_srcMemory{};
		VkDeviceMemory m_dstMemory{};
		VkImage m_tmpSrcTexture{};
		VkImage m_tmpDstTexture{};
		VkSampler m_sampler{};
		VkDescriptorPool m_pool{};
		std::vector< std::shared_ptr< LayerCopy > > m_layerStretches;
		ashes::VkImageCopyArray m_layerBlitsToTmp;
		ashes::VkImageCopyArray m_layerBlitsFromTmp;
		ashes::VkImageCopyArray m_layerBlits;
		VkDeviceMemory m_uboMemory{};
		VkBuffer m_ubo{};
		VkCommandBuffer m_commandBuffer{};
		VkImageBlitArray m_blitRegions;
		VkImageBlitArray m_stretchRegions;
	};
}
