#include "NodesRenderer.hpp"

namespace vkapp
{
	NodesRenderer::NodesRenderer( renderer::Device const & device
		, std::string const & fragmentShaderFile
		, std::vector< renderer::Format > const & formats
		, bool clearViews
		, bool opaqueNodes
		, renderer::UniformBuffer< common::SceneData > const & sceneUbo
		, renderer::UniformBuffer< common::ObjectData > const & objectUbo )
		: common::NodesRenderer{ device
			, fragmentShaderFile
			, formats
			, clearViews
			, opaqueNodes }
		, m_sceneUbo{ sceneUbo }
		, m_objectUbo{ objectUbo }
	{
	}

	void NodesRenderer::doFillObjectDescriptorLayoutBindings( renderer::DescriptorSetLayoutBindingArray & bindings )
	{
		bindings.emplace_back( 1u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex );
		bindings.emplace_back( 2u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex );
	}

	void NodesRenderer::doFillObjectDescriptorSet( renderer::DescriptorSetLayout & descriptorLayout
		, renderer::DescriptorSet & descriptorSet )
	{
		descriptorSet.createBinding( descriptorLayout.getBinding( 1u )
			, m_sceneUbo
			, 0u
			, 1u );
		descriptorSet.createBinding( descriptorLayout.getBinding( 2u )
			, m_objectUbo
			, 0u
			, 1u );
	}
}
