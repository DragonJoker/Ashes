/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_DescriptorSetLayout_HPP___
#define ___D3D11Renderer_DescriptorSetLayout_HPP___
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Descriptor/DescriptorSetLayout.hpp>

namespace d3d11_renderer
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

#endif
