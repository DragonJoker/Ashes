/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Core/PhysicalDevice.hpp>

namespace d3d11_renderer
{
	/**
	*\~french
	*\brief
	*	Classe contenant les informations liées au GPU physique.
	*\~english
	*\brief
	*	Wraps informations about physical GPU.
	*/
	class PhysicalDevice
		: public ashes::PhysicalDevice
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] vulkan
		*	L'instance Vulkan.
		*\param[in] gpu
		*	Le GPU physique.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] vulkan
		*	The Vulkan instance.
		*\param[in] gpu
		*	The Vulkan physical GPU.
		*/
		PhysicalDevice( Renderer & renderer
			, IDXGIAdapter * adapter
			, IDXGIAdapter1 * adapter1
			, IDXGIAdapter2 * adapter2 );
		~PhysicalDevice();
		/**
		*\copydoc	ashes::PhysicalDevice::initialise
		*/
		void initialise()override;

		inline IDXGIAdapter * getAdapter()const
		{
			return m_adapter;
		}

		inline IDXGIAdapter1 * getAdapter1()const
		{
			return m_adapter1;
		}

		inline IDXGIAdapter2 * getAdapter2()const
		{
			return m_adapter2;
		}

		inline IDXGIOutput * getOutput()const
		{
			return m_output;
		}

		inline D3D_FEATURE_LEVEL getFeatureLevel()const
		{
			return m_featureLevel;
		}

	private:
		Renderer & m_renderer;
		IDXGIAdapter * m_adapter;
		IDXGIAdapter1 * m_adapter1;
		IDXGIAdapter2 * m_adapter2;
		IDXGIOutput * m_output;
		D3D_FEATURE_LEVEL m_featureLevel;
	};
}
