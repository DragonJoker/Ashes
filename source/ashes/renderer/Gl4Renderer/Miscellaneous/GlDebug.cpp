#include "Miscellaneous/GlDebug.hpp"

#include <cstdint>
#include <iostream>
#include <sstream>

#include <GL/gl.h>

namespace ashes::gl4
{
	std::string getErrorName( uint32_t code, uint32_t category )
	{
		static uint32_t constexpr InvalidEnum = 0x0500;
		static uint32_t constexpr InvalidValue = 0x0501;
		static uint32_t constexpr InvalidOperation = 0x0502;
		static uint32_t constexpr StackOverflow = 0x0503;
		static uint32_t constexpr StackUnderflow = 0x0504;
		static uint32_t constexpr OutOfMemory = 0x0505;
		static uint32_t constexpr InvalidFramebufferOperation = 0x0506;

		static std::map< uint32_t, std::string > const errors
		{
			{ InvalidEnum, "Invalid Enum" },
			{ InvalidValue, "Invalid Value" },
			{ InvalidOperation, "Invalid Operation" },
			{ StackOverflow, "Stack Overflow" },
			{ StackUnderflow, "Stack Underflow" },
			{ OutOfMemory, "Out of memory" },
			{ InvalidFramebufferOperation, "Invalid frame buffer operation" },
		};

		if ( category == GL_DEBUG_CATEGORY_API_ERROR_AMD
			|| category == GL_DEBUG_TYPE_ERROR )
		{
			auto it = errors.find( code );

			if ( it == errors.end() )
			{
				return "UNKNOWN_ERROR";
			}

			return it->second;
		}
		else
		{
			return std::string{};
		}
	}

	bool glCheckError( std::string const & text )
	{
		bool result = true;
		uint32_t errorCode = glGetError();

		if ( errorCode )
		{
			std::cerr << "OpenGL Error, on function: " << text << std::endl;
			std::cerr << "  ID: 0x" << std::hex << errorCode << " (" << getErrorName( errorCode, GL_DEBUG_TYPE_ERROR ) << ")" << std::endl;
			errorCode = glGetError();
			result = false;
		}

		return result;
	}

	//*************************************************************************************************
}
