/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Ashes/Descriptor/DescriptorSetLayout.hpp>

namespace test_renderer
{
	/**
	*\brief
	*	Classe wrappant un TestDescriptorSetLayout.
	*/
	class DescriptorSetLayout
		: public ashes::DescriptorSetLayout
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] bindings
		*	The bindings.
		*/
		DescriptorSetLayout( Device const & device
			, ashes::DescriptorSetLayoutBindingArray bindings );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~DescriptorSetLayout();
	};
}
