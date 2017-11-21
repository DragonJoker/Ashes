#include "Mat4.hpp"

namespace renderer
{
	namespace details
	{
		template< typename T >
		struct FormatGetter;

		template<>
		struct FormatGetter< float >
		{
			static VkFormat constexpr value = VK_FORMAT_R32_SFLOAT;
		};

		template<>
		struct FormatGetter< Vec2 >
		{
			static VkFormat constexpr value = VK_FORMAT_R32G32_SFLOAT;
		};

		template<>
		struct FormatGetter< Vec3 >
		{
			static VkFormat constexpr value = VK_FORMAT_R32G32B32_SFLOAT;
		};

		template<>
		struct FormatGetter< Vec4 >
		{
			static VkFormat constexpr value = VK_FORMAT_R32G32B32A32_SFLOAT;
		};

		template<>
		struct FormatGetter< int >
		{
			static VkFormat constexpr value = VK_FORMAT_R32_SINT;
		};

		template<>
		struct FormatGetter< IVec2 >
		{
			static VkFormat constexpr value = VK_FORMAT_R32G32_SINT;
		};

		template<>
		struct FormatGetter< IVec3 >
		{
			static VkFormat constexpr value = VK_FORMAT_R32G32B32_SINT;
		};

		template<>
		struct FormatGetter< IVec4 >
		{
			static VkFormat constexpr value = VK_FORMAT_R32G32B32A32_SINT;
		};
	}
}
