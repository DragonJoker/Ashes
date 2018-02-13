#include "NodesRenderer.hpp"

namespace vkapp
{
	NodesRenderer::NodesRenderer( renderer::Device const & device
		, renderer::ShaderProgramPtr && program
		, std::vector< renderer::PixelFormat > const & formats
		, bool clearViews
		, bool opaqueNodes
		, renderer::UniformBuffer< renderer::Mat4 > const & matrixUbo
		, renderer::UniformBuffer< renderer::Mat4 > const & objectUbo )
		: common::NodesRenderer{ device
			, std::move( program )
			, formats
			, clearViews
			, opaqueNodes }
		, m_matrixUbo{ matrixUbo }
		, m_objectUbo{ objectUbo }
	{
	}

	void NodesRenderer::doFillDescriptorLayoutBindings( renderer::DescriptorSetLayoutBindingArray & bindings )
	{
		bindings.emplace_back( 1u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex );
		bindings.emplace_back( 2u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex );
	}

	void NodesRenderer::doFillDescriptorSet( renderer::DescriptorSetLayout & descriptorLayout
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
