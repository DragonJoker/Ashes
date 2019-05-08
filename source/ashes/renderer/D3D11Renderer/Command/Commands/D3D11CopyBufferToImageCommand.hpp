/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	struct CopyToStagingProcess
	{
		VkBuffer stagingSrc{ VK_NULL_HANDLE };
		VkDeviceMemory stagingSrcMemory{ VK_NULL_HANDLE };
		VkBufferCopy copyToStaging;
	};

	struct CopyFromStagingProcess
	{
		VkImage stagingDst{ VK_NULL_HANDLE };
		VkDeviceMemory stagingDstMemory{ VK_NULL_HANDLE };
		UINT stagingDstSubresource;
		VkImageCopy copyFromStaging;
	};

	struct MapCopyImage
	{
		VkImage dst{ VK_NULL_HANDLE };
		VkDeviceMemory dstMemory{ VK_NULL_HANDLE };
		VkBufferImageCopy mapCopy;
		D3D11_BOX srcBox;
		UINT dstSubresource;
		VkSubresourceLayout dstLayout;
		Optional< CopyFromStagingProcess > copyFromStaging;
	};
	using MapCopyImageArray = std::vector< MapCopyImage >;

	struct MapCopyProcess
	{
		VkBuffer src{ VK_NULL_HANDLE };
		VkDeviceMemory srcMemory{ VK_NULL_HANDLE };
		MapCopyImageArray mapCopyImages;
	};

	struct BufferToImageCopyProcess
	{
		Optional< CopyToStagingProcess > copyToStaging;
		MapCopyProcess mapCopy;
	};
	/**
	*\brief
	*	Commande de copie du contenu d'un tampon dans une image.
	*/
	class CopyBufferToImageCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] copyInfo
		*	Les informations de copie.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	L'image destination.
		*/
		CopyBufferToImageCommand( VkDevice device
			, VkBufferImageCopyArray const & copyInfo
			, VkBuffer src
			, VkImage dst );
		~CopyBufferToImageCommand();

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		void apply( Context const & context
			, CopyToStagingProcess const & process )const;
		void apply( Context const & context
			, CopyFromStagingProcess const & process )const;
		void apply( Context const & context
			, MapCopyProcess const & process )const;
		void doMapCopy( MapCopyImage const & mapCopy
			, VkFormat format
			, VkDeviceMemory src
			, VkDeviceMemory dst )const;
		void doCopyToStaging( Context const & context
			, VkDeviceSize srcOffset
			, VkDeviceSize size
			, VkBuffer src
			, VkBuffer staging )const;
		void doCopyFromStaging( Context const & context
			, VkBufferImageCopy const & copyInfo
			, VkImage staging
			, VkImage dst )const;

	private:
		VkBuffer m_src;
		VkImage m_dst;
		DXGI_FORMAT m_format;
		bool m_srcMappable;
		bool m_dstMappable;
		BufferToImageCopyProcess m_process;
	};
}
