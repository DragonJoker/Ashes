/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace gl_renderer
{
	enum GlInternal
		: GLenum
	{
		/**
		*\~english
		*name
		*	Colour formats.
		*\~french
		*name
		*	Formats de couleur.
		*/
		/**@{*/
		GL_INTERNAL_R4G4B4A4_UNORM_PACK16 = 0x8056,
		GL_INTERNAL_R5G6B5_UNORM_PACK16 = 0x8D62,
		GL_INTERNAL_R5G5B5A1_UNORM_PACK16 = 0x8057,
		GL_INTERNAL_R8_UNORM = 0x8229,
		GL_INTERNAL_R8_SNORM = 0x8F94,
		GL_INTERNAL_R8_UINT = 0x8232,
		GL_INTERNAL_R8_SINT = 0x8231,
		GL_INTERNAL_R8_SRGB = 0x8FBD,
		GL_INTERNAL_R8G8_UNORM = 0x822B,
		GL_INTERNAL_R8G8_SNORM = 0x8F95,
		GL_INTERNAL_R8G8_UINT = 0x8238,
		GL_INTERNAL_R8G8_SINT = 0x8237,
		GL_INTERNAL_R8G8_SRGB = 0x8FBE,
		GL_INTERNAL_R8G8B8_UNORM = 0x8051,
		GL_INTERNAL_R8G8B8_SNORM = 0x8F96,
		GL_INTERNAL_R8G8B8_UINT = 0x8D7D,
		GL_INTERNAL_R8G8B8_SINT = 0x8D8F,
		GL_INTERNAL_R8G8B8_SRGB = 0x8C41,
		GL_INTERNAL_R8G8B8A8_UNORM = 0x8058,
		GL_INTERNAL_R8G8B8A8_SNORM = 0x8F97,
		GL_INTERNAL_R8G8B8A8_UINT = 0x8D7C,
		GL_INTERNAL_R8G8B8A8_SINT = 0x8D8E,
		GL_INTERNAL_R8G8B8A8_SRGB = 0x8C43,
		GL_INTERNAL_B8G8R8A8_UNORM = 0x93A1,
		GL_INTERNAL_A2R10G10B10_UNORM_PACK32 = 0x8059,
		GL_INTERNAL_A2R10G10B10_UINT_PACK32 = 0x906F,
		GL_INTERNAL_R16_UNORM = 0x822A,
		GL_INTERNAL_R16_SNORM = 0x8F98,
		GL_INTERNAL_R16_UINT = 0x8234,
		GL_INTERNAL_R16_SINT = 0x8233,
		GL_INTERNAL_R16_SFLOAT = 0x822D,
		GL_INTERNAL_R16G16_UNORM = 0x822C,
		GL_INTERNAL_R16G16_SNORM = 0x8F99,
		GL_INTERNAL_R16G16_UINT = 0x823A,
		GL_INTERNAL_R16G16_SINT = 0x8239,
		GL_INTERNAL_R16G16_SFLOAT = 0x822F,
		GL_INTERNAL_R16G16B16_UNORM = 0x8054,
		GL_INTERNAL_R16G16B16_SNORM = 0x8F9A,
		GL_INTERNAL_R16G16B16_UINT = 0x8D77,
		GL_INTERNAL_R16G16B16_SINT = 0x8D89,
		GL_INTERNAL_R16G16B16_SFLOAT = 0x881B,
		GL_INTERNAL_R16G16B16A16_UNORM = 0x805B,
		GL_INTERNAL_R16G16B16A16_SNORM = 0x8F9B,
		GL_INTERNAL_R16G16B16A16_UINT = 0x8D76,
		GL_INTERNAL_R16G16B16A16_SINT = 0x8D88,
		GL_INTERNAL_R16G16B16A16_SFLOAT = 0x881A,
		GL_INTERNAL_R32_UINT = 0x8236,
		GL_INTERNAL_R32_SINT = 0x8235,
		GL_INTERNAL_R32_SFLOAT = 0x822E,
		GL_INTERNAL_R32G32_UINT = 0x823C,
		GL_INTERNAL_R32G32_SINT = 0x823B,
		GL_INTERNAL_R32G32_SFLOAT = 0x8230,
		GL_INTERNAL_R32G32B32_UINT = 0x8D71,
		GL_INTERNAL_R32G32B32_SINT = 0x8D83,
		GL_INTERNAL_R32G32B32_SFLOAT = 0x8815,
		GL_INTERNAL_R32G32B32A32_UINT = 0x8D70,
		GL_INTERNAL_R32G32B32A32_SINT = 0x8D82,
		GL_INTERNAL_R32G32B32A32_SFLOAT = 0x8814,
		GL_INTERNAL_B10G11R11_UFLOAT_PACK32 = 0x8C3A,
		GL_INTERNAL_E5B9G9R9_UFLOAT_PACK32 = 0x8C3D,
		/**@}*/
		/**
		*\~english
		*name
		*	Depth / stencil formats.
		*\~french
		*name
		*	Formats de profondeur / stencil.
		*/
		/**@{*/
		GL_INTERNAL_D16_UNORM = 0x81A5,
		GL_INTERNAL_D24_UNORM = 0x81A6,
		GL_INTERNAL_D32_SFLOAT = 0x8CAC,
		GL_INTERNAL_S8_UINT = 0x8D48,
		GL_INTERNAL_D24_UNORM_S8_UINT = 0x88F0,
		GL_INTERNAL_D32_SFLOAT_S8_UINT = 0x8CAD,
		/**@}*/
		/**
		*\~english
		*name
		*	BC compressed formats.
		*\~french
		*name
		*	Formats de compression BC.
		*/
		/**@{*/
		GL_INTERNAL_BC1_RGB_UNORM_BLOCK = 0x83F0,
		GL_INTERNAL_BC1_RGB_SRGB_BLOCK = 0x8C4C,
		GL_INTERNAL_BC1_RGBA_UNORM_BLOCK = 0x83F1,
		GL_INTERNAL_BC1_RGBA_SRGB_BLOCK = 0x8C4D,
		GL_INTERNAL_BC2_UNORM_BLOCK = 0x83F2,
		GL_INTERNAL_BC2_SRGB_BLOCK = 0x8C4E,
		GL_INTERNAL_BC3_UNORM_BLOCK = 0x83F3,
		GL_INTERNAL_BC3_SRGB_BLOCK = 0x8C4F,
		GL_INTERNAL_BC4_UNORM_BLOCK = 0x8DBB,
		GL_INTERNAL_BC4_SNORM_BLOCK = 0x8DBC,
		GL_INTERNAL_BC5_UNORM_BLOCK = 0x8DBD,
		GL_INTERNAL_BC5_SNORM_BLOCK = 0x8DBE,
		GL_INTERNAL_BC6H_UFLOAT_BLOCK = 0x8E8F,
		GL_INTERNAL_BC6H_SFLOAT_BLOCK = 0x8E8E,
		GL_INTERNAL_BC7_UNORM_BLOCK = 0x8E8C,
		GL_INTERNAL_BC7_SRGB_BLOCK = 0x8E8D,
		/**@}*/
		/**
		*\~english
		*name
		*	ETC compressed formats.
		*\~french
		*name
		*	Formats de compression ETC.
		*/
		/**@{*/
		GL_INTERNAL_ETC2_R8G8B8_UNORM_BLOCK = 0x9274,
		GL_INTERNAL_ETC2_R8G8B8_SRGB_BLOCK = 0x9275,
		GL_INTERNAL_ETC2_R8G8B8A1_UNORM_BLOCK = 0x9276,
		GL_INTERNAL_ETC2_R8G8B8A1_SRGB_BLOCK = 0x9277,
		GL_INTERNAL_ETC2_R8G8B8A8_UNORM_BLOCK = 0x9278,
		GL_INTERNAL_ETC2_R8G8B8A8_SRGB_BLOCK = 0x9279,
		/**@}*/
		/**
		*\~english
		*name
		*	ETC compressed formats.
		*\~french
		*name
		*	Formats de compression ETC.
		*/
		/**@{*/
		GL_INTERNAL_EAC_R11_UNORM_BLOCK = 0x9270,
		GL_INTERNAL_EAC_R11_SNORM_BLOCK = 0x9271,
		GL_INTERNAL_EAC_R11G11_UNORM_BLOCK = 0x9272,
		GL_INTERNAL_EAC_R11G11_SNORM_BLOCK = 0x9273,
		/**@}*/
		/**
		*\~english
		*name
		*	ASTC compressed formats.
		*\~french
		*name
		*	Formats de compression ASTC.
		*/
		/**@{*/
		GL_INTERNAL_ASTC_4x4_UNORM_BLOCK = 0x93B0,
		GL_INTERNAL_ASTC_4x4_SRGB_BLOCK = 0x93D0,
		GL_INTERNAL_ASTC_5x4_UNORM_BLOCK = 0x93B1,
		GL_INTERNAL_ASTC_5x4_SRGB_BLOCK = 0x93D1,
		GL_INTERNAL_ASTC_5x5_UNORM_BLOCK = 0x93B2,
		GL_INTERNAL_ASTC_5x5_SRGB_BLOCK = 0x93D2,
		GL_INTERNAL_ASTC_6x5_UNORM_BLOCK = 0x93B3,
		GL_INTERNAL_ASTC_6x5_SRGB_BLOCK = 0x93D3,
		GL_INTERNAL_ASTC_6x6_UNORM_BLOCK = 0x93B4,
		GL_INTERNAL_ASTC_6x6_SRGB_BLOCK = 0x93D4,
		GL_INTERNAL_ASTC_8x5_UNORM_BLOCK = 0x93B5,
		GL_INTERNAL_ASTC_8x5_SRGB_BLOCK = 0x93D5,
		GL_INTERNAL_ASTC_8x6_UNORM_BLOCK = 0x93B6,
		GL_INTERNAL_ASTC_8x6_SRGB_BLOCK = 0x93D6,
		GL_INTERNAL_ASTC_8x8_UNORM_BLOCK = 0x93B7,
		GL_INTERNAL_ASTC_8x8_SRGB_BLOCK = 0x93D7,
		GL_INTERNAL_ASTC_10x5_UNORM_BLOCK = 0x93B8,
		GL_INTERNAL_ASTC_10x5_SRGB_BLOCK = 0x93D8,
		GL_INTERNAL_ASTC_10x6_UNORM_BLOCK = 0x93B9,
		GL_INTERNAL_ASTC_10x6_SRGB_BLOCK = 0x93D9,
		GL_INTERNAL_ASTC_10x8_UNORM_BLOCK = 0x93BA,
		GL_INTERNAL_ASTC_10x8_SRGB_BLOCK = 0x93DA,
		GL_INTERNAL_ASTC_10x10_UNORM_BLOCK = 0x93BB,
		GL_INTERNAL_ASTC_10x10_SRGB_BLOCK = 0x93DB,
		GL_INTERNAL_ASTC_12x10_UNORM_BLOCK = 0x93BC,
		GL_INTERNAL_ASTC_12x10_SRGB_BLOCK = 0x93DC,
		GL_INTERNAL_ASTC_12x12_UNORM_BLOCK = 0x93BD,
		GL_INTERNAL_ASTC_12x12_SRGB_BLOCK = 0x93DD,
		/**@{*/
	};
	enum GlFormat
		: GLenum
	{
		GL_FORMAT_S = 0x1802,
		GL_FORMAT_D = 0x1902,
		GL_FORMAT_R = 0x1903,
		GL_FORMAT_RGB = 0x1907,
		GL_FORMAT_RGBA = 0x1908,
		GL_FORMAT_ABGR = 0x8000,
		GL_FORMAT_BGR = 0x80E0,
		GL_FORMAT_BGRA = 0x80E1,
		GL_FORMAT_RG = 0x8227,
		GL_FORMAT_DS = 0x84F9,
	};
	enum GlType
		: GLenum
	{
		GL_TYPE_I8 = 0x1400,
		GL_TYPE_UI8 = 0x1401,
		GL_TYPE_I16 = 0x1402,
		GL_TYPE_UI16 = 0x1403,
		GL_TYPE_I32 = 0x1404,
		GL_TYPE_UI32 = 0x1405,
		GL_TYPE_F32 = 0x1406,
		GL_TYPE_F16 = 0x140B,
		GL_TYPE_US4444 = 0x8033,
		GL_TYPE_US5551 = 0x8034,
		GL_TYPE_UI8888 = 0x8035,
		GL_TYPE_UI_10_10_10_2 = 0x8036,
		GL_TYPE_UI_2_10_10_10 = 0x8368,
		GL_TYPE_UI565 = 0x8363,
		GL_TYPE_UI24_8 = 0x84FA,
		GL_TYPE_32F_UI24_8 = 0x8DAD,
		GL_UI_5_9_9_9 = 0x8C3E,
		GL_UI_10F_11F_11F = 0x8C3B,
	};
	std::string getName( GlInternal value );
	std::string getName( GlFormat value );
	std::string getName( GlType value );
	std::string getFormatName( VkFormat format );
	bool isSupportedInternal( VkFormat const & format )noexcept;
	/**
	*\brief
	*	Convertit un VkFormat en GlInternal.
	*\param[in] format
	*	Le VkFormat.
	*\return
	*	Le GlInternal.
	*/
	GlInternal getInternalFormat( VkFormat const & format )noexcept;
	/**
	*\brief
	*	Convertit un VkFormat en GlFormat.
	*\param[in] format
	*	Le VkFormat.
	*\return
	*	Le GlFormat.
	*/
	GlFormat getFormat( GlInternal format )noexcept;
	/**
	*\brief
	*	Convertit un VkFormat en GlType.
	*\param[in] format
	*	Le VkFormat.
	*\return
	*	Le GlType.
	*/
	GlType getType( GlInternal format )noexcept;
	/**
	*\brief
	*	Convertit un GlInternal en VkFormat.
	*\param[in] format
	*	Le GlInternal.
	*\return
	*	Le VkFormat.
	*/
	VkFormat convert( GlInternal format )noexcept;
	/**
	*\return
	*	le nombre de composantes du format donné.
	*\param[in] format
	*	Le VkFormat.
	*/
	uint32_t getCount( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Retrieves the byte size of given non compressed pixel format.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*\~french
	*\brief
	*	Donne le nombre d'octets du format de pixels non compressé donné.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	uint32_t getSize( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Retrieves the byte size of given BC compressed pixel format, for a 4x4 pixels block.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*\~french
	*\brief
	*	Donne le nombre d'octets du format de pixels compressé BC donné, pour un bloc de 4x4 pixels.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	uint32_t getBCCompressedSize( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Retrieves the byte size of given ETC2 compressed pixel format, for a 4x4 pixels block.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*\~french
	*\brief
	*	Donne le nombre d'octets du format de pixels compressé ETC2 donné, pour un bloc de 4x4 pixels.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	uint32_t getETC2CompressedSize( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Retrieves the byte size of given EAC compressed pixel format, for a 4x4 pixels block.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*\~french
	*\brief
	*	Donne le nombre d'octets du format de pixels compressé EAC donné, pour un bloc de 4x4 pixels.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	uint32_t getEACCompressedSize( VkFormat format )noexcept;
	/**
	*\~french
	*\brief
	*	Dit si le ashes::VkFormat donné est un format utilisable pour les tampons de profondeur et stencil.
	*\param[in] format
	*	Le ashes::VkFormat à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given ashes::VkFormat is usable in depth and stencil buffers.
	*\param[in] format
	*	The ashes::VkFormat.
	*\return
	*	\p true if it is usable in depth and stencil buffers.
	*/
	bool isDepthStencilFormat( VkFormat format )noexcept;
	/**
	*\~french
	*\brief
	*	Dit si le ashes::VkFormat donné est un format utilisable pour les tampons de stencil.
	*\param[in] format
	*	Le ashes::VkFormat à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given ashes::VkFormat is usable in stencil buffers.
	*\param[in] format
	*	The ashes::VkFormat.
	*\return
	*	\p true if it is usable in stencil buffers.
	*/
	bool isStencilFormat( VkFormat format )noexcept;
	/**
	*\~french
	*\brief
	*	Dit si le ashes::VkFormat donné est un format utilisable pour les tampons de profondeur.
	*\param[in] format
	*	Le ashes::VkFormat à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given ashes::VkFormat is usable in depth buffers.
	*\param[in] format
	*	The ashes::VkFormat.
	*\return
	*	\p true if it is usable in depth buffers.
	*/
	bool isDepthFormat( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Tells if given pixel format is a BC compressed pixel format.
	*\param[in] format
	*	The pixel format.
	*\~french
	*\brief
	*	Dit si le format de pixels donné est un format compressé BC.
	*\param[in] format
	*	Le format de pixel.
	*/
	bool isBCCompressedFormat( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Tells if given pixel format is a ETC2 compressed pixel format.
	*\param[in] format
	*	The pixel format.
	*\~french
	*\brief
	*	Dit si le format de pixels donné est un format compressé ETC2.
	*\param[in] format
	*	Le format de pixel.
	*/
	bool isETC2CompressedFormat( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Tells if given pixel format is a EAC compressed pixel format.
	*\param[in] format
	*	The pixel format.
	*\~french
	*\brief
	*	Dit si le format de pixels donné est un format compressé EAC.
	*\param[in] format
	*	Le format de pixel.
	*/
	bool isEACCompressedFormat( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Tells if given pixel format is a ASTC compressed pixel format.
	*\param[in] format
	*	The pixel format.
	*\~french
	*\brief
	*	Dit si le format de pixels donné est un format compressé ASTC.
	*\param[in] format
	*	Le format de pixel.
	*/
	bool isASTCCompressedFormat( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Tells if given pixel format is a compressed pixel format.
	*\param[in] format
	*	The pixel format.
	*\~french
	*\brief
	*	Dit si le format de pixels donné est un format compressé.
	*\param[in] format
	*	Le format de pixel.
	*/
	bool isCompressedFormat( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Tells if given pixel format is a YCBCR pixel format.
	*\param[in] format
	*	The pixel format.
	*\~french
	*\brief
	*	Dit si le format de pixels donné est un format YCBCR.
	*\param[in] format
	*	Le format de pixel.
	*/
	bool isYCBCRFormat( VkFormat format )noexcept;
	/**
	*\~english
	*\brief
	*	Tells if given pixel format is a PVRTC pixel format.
	*\param[in] format
	*	The pixel format.
	*\~french
	*\brief
	*	Dit si le format de pixels donné est un format PVRTC.
	*\param[in] format
	*	Le format de pixel.
	*/
	bool isPVRTCFormat( VkFormat format )noexcept;
	/**
	*\~french
	*\brief
	*	Dit si le ashes::VkFormat donné est un format utilisable pour les tampons de profondeur et/ou stencil.
	*\param[in] format
	*	Le ashes::VkFormat à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given ashes::VkFormat is usable in depth and/or stencil buffers.
	*\param[in] format
	*	The ashes::VkFormat.
	*\return
	*	\p true if it is usable in depth and/or stencil buffers.
	*/
	inline bool isDepthOrStencilFormat( VkFormat format )noexcept
	{
		return isDepthStencilFormat( format )
			|| isStencilFormat( format )
			|| isDepthFormat( format );
	}
	/**
	*\~english
	*\brief
	*	Gets the name of the given element.
	*\param[in] value
	*	The element.
	*\return
	*	The name.
	*\~french
	*\brief
	*	Récupère le nom de l'élément donné.
	*\param[in] value
	*	L'élément.
	*\return
	*	Le nom.
	*/
	std::string getName( VkFormat value );
	/**
	*\~english
	*\brief
	*	Retrieves the minimal VkExtent2D for given pixel format.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The VkExtent2D.
	*\~french
	*\brief
	*	Donne l'VkExtent2D minimales pour le format de pixels donné.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	L'VkExtent2D.
	*/
	inline VkExtent2D getMinimalExtent2D( VkFormat format )noexcept
	{
		VkExtent2D result{ 1, 1 };

		if ( isCompressedFormat( format ) )
		{
			if ( isBCCompressedFormat( format )
				|| isETC2CompressedFormat( format )
				|| isEACCompressedFormat( format ) )
			{
				result.width = 4;
				result.height = 4;
			}
			else if ( isASTCCompressedFormat( format ) )
			{
				switch ( format )
				{
				case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
				case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
					result.width = 4;
					result.height = 4;
					break;
				case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
				case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
					result.width = 5;
					result.height = 4;
					break;
				case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
				case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
					result.width = 5;
					result.height = 5;
					break;
				case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
				case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
					result.width = 6;
					result.height = 5;
					break;
				case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
				case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
					result.width = 6;
					result.height = 6;
					break;
				case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
				case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
					result.width = 8;
					result.height = 5;
					break;
				case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
				case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
					result.width = 8;
					result.height = 6;
					break;
				case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
				case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
					result.width = 8;
					result.height = 8;
					break;
				case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
				case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
					result.width = 10;
					result.height = 5;
					break;
				case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
				case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
					result.width = 10;
					result.height = 6;
					break;
				case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
				case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
					result.width = 10;
					result.height = 8;
					break;
				case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
				case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
					result.width = 10;
					result.height = 10;
					break;
				case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
				case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
					result.width = 12;
					result.height = 10;
					break;
				case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
				case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
					result.width = 12;
					result.height = 12;
					break;
				}
			}
			else
			{
				assert( false && "Unsupported compressed format." );
			}
		}

		return result;
	}
	/**
	*\~english
	*\brief
	*	Checks if the given extent fits the given format.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\return
	*	\p true if the extent is compatible with the format.
	*\~french
	*\brief
	*	Vérifie que les dimensiosn données sont compatibles avec le format donné.
	*\param[in] extent
	*	Les dimensions.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	\p true si les dimensiosn sont compatibles avec le format.
	*/
	inline bool checkExtent( VkFormat format, VkExtent2D const & extent )
	{
		auto minimal = getMinimalExtent2D( format );
		return extent.width >= minimal.width
			&& extent.height >= minimal.height;
	}
	/**
	*\~english
	*\brief
	*	Retrieves the byte size of given pixel format and dimensions.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*\~french
	*\brief
	*	Donne le nombre d'octets du format de pixels et les dimensions donnés.
	*\param[in] extent
	*	Les dimensions.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	inline uint32_t getSize( VkExtent2D const & extent
		, VkFormat format )noexcept
	{
		auto result = std::max( 1u, extent.width )
			* std::max( 1u, extent.height );

		if ( !isCompressedFormat( format ) )
		{
			result *= getSize( format );
		}
		else
		{
			auto minimal = getMinimalExtent2D( format );
			auto size = minimal.width * minimal.height;

			if ( ( result % size ) == 0 )
			{
				result /= size;

				if ( isBCCompressedFormat( format ) )
				{
					result *= getBCCompressedSize( format );
				}
				else if ( isETC2CompressedFormat( format ) )
				{
					result *= getETC2CompressedSize( format );
				}
				else if ( isEACCompressedFormat( format ) )
				{
					result *= getEACCompressedSize( format );
				}
			}
			else if ( result < size )
			{
				if ( isBCCompressedFormat( format ) )
				{
					result = getBCCompressedSize( format );
				}
				else if ( isETC2CompressedFormat( format ) )
				{
					result = getETC2CompressedSize( format );
				}
				else if ( isEACCompressedFormat( format ) )
				{
					result = getEACCompressedSize( format );
				}
			}
			else
			{
				assert( false );
			}
		}

		return result;
	}
	/**
	*\~english
	*\brief
	*	Retrieves the minimal Extent2D for given pixel format.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The Extent2D.
	*\~french
	*\brief
	*	Donne l'Extent2D minimales pour le format de pixels donné.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	L'Extent2D.
	*/
	inline VkExtent3D getMinimalExtent3D( VkFormat format )noexcept
	{
		auto const minimal = getMinimalExtent2D( format );
		return VkExtent3D{ minimal.width, minimal.height, 1 };
	}
	/**
	*\~english
	*\brief
	*	Checks if the given extent fits the given format.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\return
	*	\p true if the extent is compatible with the format.
	*\~french
	*\brief
	*	Vérifie que les dimensiosn données sont compatibles avec le format donné.
	*\param[in] extent
	*	Les dimensions.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	\p true si les dimensions sont compatibles avec le format.
	*/
	inline bool checkExtent( VkFormat format, VkExtent3D const & extent )
	{
		return checkExtent( format, VkExtent2D{ extent.width, extent.height } );
	}
	/**
	*\~english
	*\brief
	*	Retrieves the byte size of given pixel format and dimensions.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*\~french
	*\brief
	*	Donne le nombre d'octets du format de pixels et les dimensions donnés.
	*\param[in] extent
	*	Les dimensions.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	inline uint32_t getSize( VkExtent3D const & extent
		, VkFormat format )noexcept
	{
		return getSize( VkExtent2D{ extent.width, extent.height }, format )
			* std::max( 1u, extent.depth );
	}
}
