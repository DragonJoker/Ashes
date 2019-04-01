/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ColorSpace_HPP___
#define ___Ashes_ColorSpace_HPP___
#pragma once

namespace ashes
{
	/**
	*\french
	*\brief
	*	Enumération des espaces de couleurs.
	*\english
	*\brief
	*	colour spaces enumeration.
	*/
	enum class ColorSpace
		: int32_t
	{
		eSRGBNonLinear = 0,
		eDisplayP3NonLinear = 1000104001,
		eExtendedSRGBLinear = 1000104002,
		eDCIP3Linear = 1000104003,
		eDCIP3NonLinear = 1000104004,
		eBT709Linear = 1000104005,
		eBT709NonLinear = 1000104006,
		eBT2020Linear = 1000104007,
		eHDR10ST2084 = 1000104008,
		eDolbyVision = 1000104009,
		eHDR10HLG = 1000104010,
		eAdobeRGBLinear = 1000104011,
		eAdobeRGBNonLinear = 1000104012,
		ePassThrough = 1000104013,
		eExtendedSRGBNonLinear = 1000104014,
		Ashes_EnumBounds( eSRGBNonLinear )
	};
	/**
	*\~english
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*\~french
	*\brief
	*	Récupère le nom du type d'élément donné.
	*\param[in] value
	*	Le type d'élément.
	*\return
	*	Le nom.
	*/
	inline std::string getName( ColorSpace value )
	{
		switch ( value )
		{
		case ColorSpace::eSRGBNonLinear:
			return "srgb_nonlinear";

		case ColorSpace::eDisplayP3NonLinear:
			return "display_p3_nonlinear";

		case ColorSpace::eExtendedSRGBLinear:
			return "extended_srgb_linear";

		case ColorSpace::eDCIP3Linear:
			return "dci_p3_linear";

		case ColorSpace::eDCIP3NonLinear:
			return "dci_p3_nonlinear";

		case ColorSpace::eBT709Linear:
			return "bt709_linear";

		case ColorSpace::eBT709NonLinear:
			return "bt709_nonlinear";

		case ColorSpace::eBT2020Linear:
			return "bt2020_linear";

		case ColorSpace::eHDR10ST2084:
			return "hrd10_st2084";

		case ColorSpace::eDolbyVision:
			return "dolby_vision";

		case ColorSpace::eHDR10HLG:
			return "hdr10_hlg";

		case ColorSpace::eAdobeRGBLinear:
			return "adobe_rgb_linear";

		case ColorSpace::eAdobeRGBNonLinear:
			return "adobe_rgb_nonlinear";

		case ColorSpace::ePassThrough:
			return "pass_through";

		case ColorSpace::eExtendedSRGBNonLinear:
			return "extended_srgb_nonlinear";

		default:
			assert( false && "Unsupported ColorSpace." );
			throw std::runtime_error{ "Unsupported ColorSpace" };
		}

		return 0;
	}
}

#endif
