#pragma once

#include "Prerequisites.hpp"

#include <ashespp/Buffer/StagingBuffer.hpp>
#include <ashespp/Core/Surface.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Pipeline/GraphicsPipeline.hpp>
#include <ashespp/Pipeline/PipelineLayout.hpp>
#include <ashespp/Image/ImageView.hpp>
#include <ashespp/Image/Sampler.hpp>
#include <ashespp/Core/SwapChain.hpp>

#include <ashes/common/Signal.hpp>

#include <ObjLoader.hpp>

#include <wx/panel.h>

#include <array>

namespace vkapp
{
	class EquirectangularToCube
	{
	public:
		EquirectangularToCube( std::string const & filePath
			, utils::Device const & device
			, ashes::Queue const & queue
			, ashes::CommandPool const & commandPool
			, ashes::Image const & texture );

		void render();
		void render( ashes::CommandBuffer const & commandBuffer );

	private:
		struct FacePipeline
		{
			ashes::ImageView view;
			ashes::FrameBufferPtr frameBuffer;
			ashes::GraphicsPipelinePtr pipeline;
			ashes::DescriptorSetPtr descriptorSet;
		};

		utils::Device const & m_device;
		ashes::Queue const & m_queue;
		ashes::CommandBufferPtr m_commandBuffer;
		common::ImageData m_image;
		ashes::StagingBuffer m_stagingBuffer;
		ashes::ImagePtr m_texture;
		ashes::ImageView m_view;
		ashes::SamplerPtr m_sampler;
		std::array< utils::Mat4, 6u > m_matrixData;
		ashes::UniformBufferPtr m_matrixUbo;
		ashes::VertexBufferPtr< VertexData > m_vertexBuffer;
		ashes::PipelineVertexInputStateCreateInfo m_vertexLayout;
		ashes::DescriptorSetLayoutPtr m_descriptorLayout;
		ashes::DescriptorSetPoolPtr m_descriptorPool;
		ashes::PipelineLayoutPtr m_pipelineLayout;
		ashes::RenderPassPtr m_renderPass;
		std::array< FacePipeline, 6u > m_faces;
	};
}
