/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlFenceWaitResult
	{
		GL_WAIT_RESULT_SYNC_STATUS = 0x9114,
		GL_WAIT_RESULT_UNSIGNALED = 0x9118,
		GL_WAIT_RESULT_SIGNALED = 0x9119,
		GL_WAIT_RESULT_ALREADY_SIGNALED = 0x911A,
		GL_WAIT_RESULT_CONDITION_SATISFIED = 0x911C,
		GL_WAIT_RESULT_TIMEOUT_EXPIRED = 0x911B,
	};

	std::string getName( GlFenceWaitResult value );
	inline std::string toString( GlFenceWaitResult value ) { return getName( value ); }
}
