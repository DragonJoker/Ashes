#include "TransparentRendering.hpp"

namespace vkapp
{
	TransparentRendering::TransparentRendering( renderer::Device const & device
		, renderer::StagingBuffer & stagingBuffer
		, renderer::TextureView const & colourView
		, renderer::TextureView const & depthView
		, common::Object const & submeshes
		, common::TextureNodePtrArray const & textureNodes
		, renderer::UniformBuffer< renderer::Mat4 > const & matrixUbo
		, renderer::UniformBuffer< renderer::Mat4 > const & objectUbo )
		: common::TransparentRendering{ device
			, AppName.ToStdString()
			, "offscreen"
			, colourView.getFormat()
			, depthView.getFormat() }
		, m_matrixUbo{ matrixUbo }
		, m_objectUbo{ objectUbo }
	{
		doInitialise( submeshes
			, stagingBuffer
			, colourView
			, depthView
			, textureNodes );
	}

	void TransparentRendering::doFillDescriptorLayoutBindings( renderer::DescriptorSetLayoutBindingArray & bindings )
	{
		bindings.emplace_back( 1u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex );
		bindings.emplace_back( 2u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex );
	}

	void TransparentRendering::doFillDescriptorSet( renderer::DescriptorSetLayout & descriptorLayout
		, renderer::DescriptorSet & descriptorSet )
	{
		descriptorSet.createBinding( descriptorLayout.getBinding( 1u )
			, m_matrixUbo
			, 0u
			, 1u );
		descriptorSet.createBinding( descriptorLayout.getBinding( 2u )
			, m_objectUbo
			, 0u
			, 1u );
	}
}
