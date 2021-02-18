/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlFenceWaitFlag
	{
		GL_WAIT_FLAG_SYNC_GPU_COMMANDS_COMPLETE = 0x9117,
		GL_WAIT_FLAG_SYNC_FLUSH_COMMANDS_BIT = 0x00000001,
	};
	std::string getName( GlFenceWaitFlag value );
	inline std::string toString( GlFenceWaitFlag value ) { return getName( value ); }
}
