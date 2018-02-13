#pragma once

#include "Prerequisites.hpp"

#include <OpaqueRendering.hpp>

namespace vkapp
{
	class OpaqueRendering
		: public common::OpaqueRendering
	{
	public:
		OpaqueRendering( renderer::Device const & device
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureView const & colourView
			, renderer::TextureView const & depthView
			, common::Object const & submeshes
			, common::TextureNodePtrArray const & textureNodes
			, renderer::UniformBuffer< renderer::Mat4 > const & matrixUbo
			, renderer::UniformBuffer< renderer::Mat4 > const & objectUbo );

	private:
		void doFillDescriptorLayoutBindings( renderer::DescriptorSetLayoutBindingArray & bindings )override;
		void doFillDescriptorSet( renderer::DescriptorSetLayout & descriptorLayout
			, renderer::DescriptorSet & descriptorSet )override;

	private:
		renderer::UniformBuffer< renderer::Mat4 > const & m_matrixUbo;
		renderer::UniformBuffer< renderer::Mat4 > const & m_objectUbo;
	};
}
