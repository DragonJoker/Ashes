/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once
#include <Utils/Mat4.hpp>

namespace renderer
{
	namespace details
	{
		template< typename T >
		struct FormatGetter;

		template<>
		struct FormatGetter< float >
		{
			static AttributeFormat constexpr value = AttributeFormat::eFloat;
		};

		template<>
		struct FormatGetter< utils::Vec2 >
		{
			static AttributeFormat constexpr value = AttributeFormat::eVec2f;
		};

		template<>
		struct FormatGetter< utils::Vec3 >
		{
			static AttributeFormat constexpr value = AttributeFormat::eVec3f;
		};

		template<>
		struct FormatGetter< utils::Vec4 >
		{
			static AttributeFormat constexpr value = AttributeFormat::eVec4f;
		};

		template<>
		struct FormatGetter< int >
		{
			static AttributeFormat constexpr value = AttributeFormat::eInt;
		};

		template<>
		struct FormatGetter< utils::IVec2 >
		{
			static AttributeFormat constexpr value = AttributeFormat::eVec2i;
		};

		template<>
		struct FormatGetter< utils::IVec3 >
		{
			static AttributeFormat constexpr value = AttributeFormat::eVec3i;
		};

		template<>
		struct FormatGetter< utils::IVec4 >
		{
			static AttributeFormat constexpr value = AttributeFormat::eVec4i;
		};
	}
}

