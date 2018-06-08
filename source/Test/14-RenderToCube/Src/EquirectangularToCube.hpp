#pragma once

#include "Prerequisites.hpp"

#include <Buffer/StagingBuffer.hpp>
#include <Core/Connection.hpp>
#include <Core/Device.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Image/Sampler.hpp>
#include <Core/SwapChain.hpp>

#include <Utils/Signal.hpp>

#include <ObjLoader.hpp>

#include <wx/panel.h>

#include <array>

namespace vkapp
{
	class EquirectangularToCube
	{
	public:
		EquirectangularToCube( std::string const & filePath
			, renderer::Device & device
			, renderer::Texture & texture );

		void render();
		void render( renderer::CommandBuffer & commandBuffer );

	private:
		struct FacePipeline
		{
			renderer::TextureViewPtr view;
			renderer::FrameBufferPtr frameBuffer;
			renderer::PipelinePtr pipeline;
			renderer::DescriptorSetPtr descriptorSet;
		};

		renderer::Device & m_device;
		renderer::CommandBufferPtr m_commandBuffer;
		common::ImageData m_image;
		renderer::StagingBuffer m_stagingBuffer;
		renderer::TexturePtr m_texture;
		renderer::TextureViewPtr m_view;
		renderer::SamplerPtr m_sampler;
		renderer::UniformBufferPtr< utils::Mat4 > m_matrixUbo;
		renderer::VertexBufferPtr< VertexData > m_vertexBuffer;
		renderer::VertexLayoutPtr m_vertexLayout;
		renderer::DescriptorSetLayoutPtr m_descriptorLayout;
		renderer::DescriptorSetPoolPtr m_descriptorPool;
		renderer::PipelineLayoutPtr m_pipelineLayout;
		renderer::RenderPassPtr m_renderPass;
		std::array< FacePipeline, 6u > m_faces;
	};
}
