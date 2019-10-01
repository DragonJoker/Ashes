#include "NodesRenderer.hpp"

namespace vkapp
{
	NodesRenderer::NodesRenderer( utils::Device const & device
		, ashes::CommandPool const & commandPool
		, ashes::Queue const & transferQueue
		, std::string const & fragmentShaderFile
		, std::vector< VkFormat > const & formats
		, bool clearViews
		, bool opaqueNodes
		, ashes::UniformBuffer const & sceneUbo
		, ashes::UniformBuffer const & objectUbo
		, ashes::UniformBuffer const & lightsUbo )
		: common::NodesRenderer{ device
			, commandPool
			, transferQueue
			, fragmentShaderFile
			, formats
			, clearViews
			, opaqueNodes }
		, m_sceneUbo{ sceneUbo }
		, m_objectUbo{ objectUbo }
		, m_lightsUbo{ lightsUbo }
	{
	}

	void NodesRenderer::doFillObjectDescriptorLayoutBindings( ashes::VkDescriptorSetLayoutBindingArray & bindings )
	{
		bindings.push_back( { 1u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_VERTEX_BIT, nullptr } );
		bindings.push_back( { 2u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_VERTEX_BIT, nullptr } );
		bindings.push_back( { 3u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr } );
	}

	void NodesRenderer::doFillObjectDescriptorSet( ashes::DescriptorSetLayout & descriptorLayout
		, ashes::DescriptorSet & descriptorSet )
	{
		descriptorSet.createBinding( descriptorLayout.getBinding( 1u )
			, m_sceneUbo
			, 0u
			, 1u );
		descriptorSet.createBinding( descriptorLayout.getBinding( 2u )
			, m_objectUbo
			, 0u
			, 1u );
		descriptorSet.createBinding( descriptorLayout.getBinding( 3u )
			, m_lightsUbo
			, 0u
			, 1u );
	}
}
