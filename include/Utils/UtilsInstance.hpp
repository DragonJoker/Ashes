/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "UtilsPlugin.hpp"

#include <AshesPP/Core/Instance.hpp>

namespace utils
{
	class Instance
	{
	public:
		Instance( ashes::RendererList const & rendererList
			, std::string const & name
			, ashes::ApplicationInfo applicationInfo );
		/**
		*\~french
		*\brief
		*	Ajoute les couches de l'instance aux noms d�j� pr�sents dans la liste donn�e.
		*\param[in,out] names
		*	La liste � compl�ter.
		*\~english
		*\brief
		*	Adds the instance layers names to the given names.
		*\param[in,out] names
		*	The liste to fill.
		*/
		void completeLayerNames( ashes::StringArray & names )const;

		inline ashes::PhysicalDevice & getPhysicalDevice( uint32_t gpuIndex )const
		{
			assert( gpuIndex < m_gpus.size()
				&& "Invalid Physical Device index." );
			return *m_gpus[gpuIndex];
		}

		inline ashes::Instance const & getInstance()const
		{
			return *m_instance;
		}

		inline ashes::StringArray const & getLayerNames()const
		{
			return m_layerNames;
		}

		inline ashes::StringArray const & getExtensionNames()const
		{
			return m_extensionNames;
		}

	private:
		ashes::InstancePtr m_instance;
		VkDebugReportCallbackEXT m_debugCallback;
		ashes::PhysicalDevicePtrArray m_gpus;
		VkLayerProperties m_globalLayer;
		ashes::VkLayerPropertiesArray m_layers;
		ashes::StringArray m_layerNames;
		ashes::StringArray m_extensionNames;
		ashes::VkExtensionPropertiesArray m_globalLayerExtensions;
		std::map< std::string, ashes::VkExtensionPropertiesArray > m_layersExtensions;
	};
	using InstancePtr = std::unique_ptr< Instance >;
}

