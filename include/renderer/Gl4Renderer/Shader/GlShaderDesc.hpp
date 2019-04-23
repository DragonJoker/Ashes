/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	struct ConstantDesc
	{
		std::string name;
		uint32_t location{ 0u };
		ConstantFormat format{};
		uint32_t size{ 0u };
		uint32_t arraySize{ 1u };
		uint32_t offset{ 0u };
	};

	inline bool operator==( ConstantDesc const & lhs, ConstantDesc const & rhs )
	{
		return lhs.name == rhs.name
			&& lhs.location == rhs.location
			&& lhs.format == rhs.format
			&& lhs.size == rhs.size
			&& lhs.arraySize == rhs.arraySize
			&& lhs.offset == rhs.offset;
	}

	inline bool operator!=( ConstantDesc const & lhs, ConstantDesc const & rhs )
	{
		return !operator==( lhs, rhs );
	}

	using ConstantsLayout = std::vector< ConstantDesc >;

	inline bool operator==( ConstantsLayout const & lhs, ConstantsLayout const & rhs )
	{
		auto result = lhs.size() == rhs.size();

		if ( result )
		{
			auto lhsIt = lhs.begin();
			auto rhsIt = rhs.begin();
			auto lhsEnd = lhs.end();

			while ( result && lhsIt != lhsEnd )
			{
				result = ( *lhsIt == *rhsIt );
				++lhsIt;
				++rhsIt;
			}
		}

		return result;
	}

	inline bool operator!=( ConstantsLayout const & lhs, ConstantsLayout const & rhs )
	{
		return !operator==( lhs, rhs );
	}

	struct ConstantBufferDesc
	{
		std::string name;
		uint32_t binding{ 0u };
		uint32_t size{ 0u };
		ConstantsLayout constants;
	};

	inline bool operator==( ConstantBufferDesc const & lhs, ConstantBufferDesc const & rhs )
	{
		return lhs.name == rhs.name
			&& lhs.binding == rhs.binding
			&& lhs.size == rhs.size
			&& lhs.constants == rhs.constants;
	}

	inline bool operator!=( ConstantBufferDesc const & lhs, ConstantBufferDesc const & rhs )
	{
		return !operator==( lhs, rhs );
	}

	using InterfaceBlockLayout = std::vector< ConstantBufferDesc >;

	inline bool operator==( InterfaceBlockLayout const & lhs, InterfaceBlockLayout const & rhs )
	{
		auto result = lhs.size() == rhs.size();

		if ( result )
		{
			auto lhsIt = lhs.begin();
			auto rhsIt = rhs.begin();
			auto lhsEnd = lhs.end();

			while ( result && lhsIt != lhsEnd )
			{
				result = ( *lhsIt == *rhsIt );
				++lhsIt;
				++rhsIt;
			}
		}

		return result;
	}

	inline bool operator!=( InterfaceBlockLayout const & lhs, InterfaceBlockLayout const & rhs )
	{
		return !operator==( lhs, rhs );
	}

	struct ShaderDesc
	{
		VkShaderStageFlags stageFlags;
		ConstantsLayout constantsLayout;
		InterfaceBlockLayout interfaceBlockLayout;
	};

	inline bool operator==( ShaderDesc const & lhs, ShaderDesc const & rhs )
	{
		return lhs.stageFlags == rhs.stageFlags
			&& lhs.constantsLayout == rhs.constantsLayout
			&& lhs.interfaceBlockLayout == rhs.interfaceBlockLayout;
	}

	inline bool operator!=( ShaderDesc const & lhs, ShaderDesc const & rhs )
	{
		return !operator==( lhs, rhs );
	}

	using ProgramLayout = std::vector< ShaderDesc >;

	inline bool operator==( ProgramLayout const & lhs, ProgramLayout const & rhs )
	{
		auto result = lhs.size() == rhs.size();

		if ( result )
		{
			auto lhsIt = lhs.begin();
			auto rhsIt = rhs.begin();
			auto lhsEnd = lhs.end();

			while ( result && lhsIt != lhsEnd )
			{
				result = ( *lhsIt == *rhsIt );
				++lhsIt;
				++rhsIt;
			}
		}

		return result;
	}

	inline bool operator!=( ProgramLayout const & lhs, ProgramLayout const & rhs )
	{
		return !operator==( lhs, rhs );
	}

	struct PushConstantDesc
	{
		ConstantFormat format{};
		uint32_t location{ 0u };
		uint32_t offset{ 0u };
		uint32_t size{ 0u };
		uint32_t arraySize{ 1u };
	};

	inline bool operator==( PushConstantDesc const & lhs, PushConstantDesc const & rhs )
	{
		return lhs.format == rhs.format
			&& lhs.location == rhs.location
			&& lhs.offset == rhs.offset
			&& lhs.size == rhs.size
			&& lhs.arraySize == rhs.arraySize;
	}

	inline bool operator!=( PushConstantDesc const & lhs, PushConstantDesc const & rhs )
	{
		return !operator==( lhs, rhs );
	}

	struct PushConstantsDesc
	{
		VkShaderStageFlags stageFlags{};
		uint32_t offset{ 0u };
		uint32_t size{ 0u };
		std::vector< PushConstantDesc > constants;
		std::vector< uint8_t > data;
	};

	inline bool operator==( PushConstantsDesc const & lhs, PushConstantsDesc const & rhs )
	{
		return lhs.stageFlags == rhs.stageFlags
			&& lhs.offset == rhs.offset
			&& lhs.size == rhs.size
			&& lhs.constants == rhs.constants
			&& lhs.data == rhs.data;
	}

	inline bool operator!=( PushConstantsDesc const & lhs, PushConstantsDesc const & rhs )
	{
		return !operator==( lhs, rhs );
	}
}
