#pragma once

#include "Prerequisites.hpp"

#include <ashespp/Buffer/Buffer.hpp>
#include <ashespp/Buffer/PushConstantsBuffer.hpp>
#include <ashespp/Buffer/StagingBuffer.hpp>
#include <ashespp/Buffer/UniformBuffer.hpp>
#include <ashespp/Buffer/VertexBuffer.hpp>
#include <ashespp/Command/CommandBuffer.hpp>
#include <ashespp/Command/CommandPool.hpp>
#include <ashespp/Core/Surface.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Core/PlatformWindowHandle.hpp>
#include <ashespp/Core/Instance.hpp>
#include <ashespp/Core/SwapChain.hpp>
#include <ashespp/Descriptor/DescriptorSet.hpp>
#include <ashespp/Descriptor/DescriptorSetLayout.hpp>
#include <ashespp/Descriptor/DescriptorSetPool.hpp>
#include <ashespp/Image/Sampler.hpp>
#include <ashespp/Image/Image.hpp>
#include <ashespp/Image/ImageView.hpp>
#include <ashespp/Miscellaneous/QueryPool.hpp>
#include <ashespp/Pipeline/GraphicsPipeline.hpp>
#include <ashespp/Pipeline/PipelineLayout.hpp>
#include <ashespp/RenderPass/FrameBuffer.hpp>
#include <ashespp/RenderPass/RenderPass.hpp>
#include <ashespp/Sync/Queue.hpp>

#include <util/StringUtils.hpp>
#include <util/Transform.hpp>
#include <util/GlslToSpv.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <numeric>
#include <sstream>

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/Importer.hpp>

#pragma warning( disable: 4996 )
#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/windowptr.h>
#include <wx/wx.h>

#if defined( __WXGTK__ )
#	include <gdk/gdkx.h>
#	include <gtk/gtk.h>
#elif defined( __WXMSW__ )
#	include <wx/msw/private.h>
#endif


#if defined( _WIN32 )

#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>

#include <direct.h>
#include <Shlobj.h>
#include <windows.h>

#elif defined( __linux__ )

#include <sys/stat.h>

#include <unistd.h>
#include <dirent.h>
#include <pwd.h>

#endif

#if defined( near )
#	undef near
#	undef far
#endif
