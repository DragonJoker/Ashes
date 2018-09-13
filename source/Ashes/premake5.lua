include( "Ashes/premake5.lua" )
include( "VkRenderer/premake5.lua" )
include( "Gl4Renderer/premake5.lua" )
include( "Gl3Renderer/premake5.lua" )
include( "TestRenderer/premake5.lua" )

if ( os.istarget( "Windows" ) )
then
	include( "D3D11Renderer/premake5.lua" )
end
