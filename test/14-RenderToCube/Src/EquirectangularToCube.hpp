#pragma once

#include "Prerequisites.hpp"

#include <Buffer/StagingBuffer.hpp>
#include <AshesPP/Core/Surface.hpp>
#include <AshesPP/Core/Device.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Image/Sampler.hpp>
#include <AshesPP/Core/SwapChain.hpp>

#include <AshesRenderer/Util/Signal.hpp>

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
			, ashes::Image & texture );

		void render();
		void render( ashes::CommandBuffer & commandBuffer );

	private:
		struct FacePipeline
		{
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
		ashes::ImageViewPtr m_view;
		ashes::SamplerPtr m_sampler;
		ashes::UniformBufferPtr< utils::Mat4 > m_matrixUbo;
		ashes::VertexBufferPtr< VertexData > m_vertexBuffer;
		ashes::VertexLayoutPtr m_vertexLayout;
		ashes::DescriptorSetLayoutPtr m_descriptorLayout;
		ashes::DescriptorSetPoolPtr m_descriptorPool;
		ashes::PipelineLayoutPtr m_pipelineLayout;
		ashes::RenderPassPtr m_renderPass;
		std::array< FacePipeline, 6u > m_faces;
	};
}
