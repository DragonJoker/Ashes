#include "NodesRenderer.hpp"

namespace vkapp
{
	NodesRenderer::NodesRenderer( renderer::Device const & device
		, std::string const & fragmentShaderFile
		, std::vector< renderer::PixelFormat > const & formats
		, bool clearViews
		, bool opaqueNodes
		, renderer::UniformBuffer< common::SceneData > const & sceneUbo )
		: common::NodesRenderer{ device
			, fragmentShaderFile
			, formats
			, clearViews
			, opaqueNodes }
		, m_sceneUbo{ sceneUbo }
	{
	}

	void NodesRenderer::doFillBillboardDescriptorLayoutBindings( renderer::DescriptorSetLayoutBindingArray & bindings )
	{
		bindings.emplace_back( 1u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex );
	}

	void NodesRenderer::doFillBillboardDescriptorSet( renderer::DescriptorSetLayout & descriptorLayout
		, renderer::DescriptorSet & descriptorSet )
	{
		descriptorSet.createBinding( descriptorLayout.getBinding( 1u )
			, m_sceneUbo
			, 0u
			, 1u );
	}
}
