/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Command/Commands/XBoxCommandBase.hpp"

namespace ashes::xbox
{
	struct CopyToStagingProcess
	{
		VkBuffer stagingSrc{};
		VkDeviceMemory stagingSrcMemory{};
		VkBufferCopy copyToStaging{};
	};

	struct CopyFromStagingProcess
	{
		VkImage stagingDst{};
		VkDeviceMemory stagingDstMemory{};
		UINT stagingDstSubresource{};
		VkImageCopy copyFromStaging{};
	};

	struct MapCopyImage
	{
		VkImage dst{};
		VkDeviceMemory dstMemory{};
		VkBufferImageCopy mapCopy;
		D3D11_BOX srcBox{};
		UINT dstSubresource{};
		VkSubresourceLayout dstLayout{};
		Optional< CopyFromStagingProcess > copyFromStaging{};
	};
	using MapCopyImageArray = std::vector< MapCopyImage >;

	struct MapCopyProcess
	{
		VkBuffer src{};
		VkDeviceMemory srcMemory{};
		MapCopyImageArray mapCopyImages{};
	};

	struct BufferToImageCopyProcess
	{
		Optional< CopyToStagingProcess > copyToStaging{};
		MapCopyProcess mapCopy{};
	};
	/**
	*\brief
	*	Commande de copie du contenu d'un tampon dans une image.
	*/
	class CopyBufferToImageCommand
		: public CommandBase
		, public NonCopyable
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
			, ArrayView< VkBufferImageCopy const > const & copyInfo
			, VkBuffer src
			, VkImage dst );
		~CopyBufferToImageCommand()noexcept override;

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		void apply( Context const & context
			, CopyToStagingProcess const & process )const;
		void apply( Context const & context
			, CopyFromStagingProcess const & process )const;
		void apply( Context const & context
			, MapCopyProcess const & process )const;
		void doMapCopy( Context const & context
			, MapCopyImage const & mapCopy
			, VkFormat format
			, D3D11_MAPPED_SUBRESOURCE srcMapped
			, ObjectMemory const & dst )const;
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
		std::vector< VkBufferImageCopy > m_copyInfos;
		DXGI_FORMAT m_format;
		bool m_srcMappable;
		bool m_dstMappable;
		BufferToImageCopyProcess m_process;
	};
}
