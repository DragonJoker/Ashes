workspace( "RendererLib" )

sourceDir = os.getcwd()
rootDir = path.getdirectory( sourceDir )
binaryDir = path.join( rootDir, "build_premake" )
modulesDir = path.join( sourceDir, "Premake" )

configurations{ "Debug", "Release" }
platforms{ "x32", "x64" }
location( binaryDir )
language( "C++" )
cppdialect( "C++14" )

filter( "platforms:x86" ) architecture( "x86" )
filter( "platforms:x64" ) architecture( "x64" )

filter( "configurations:Debug" )
	defines( "DEBUG" )
	symbols( "On" )

filter( "configurations:Release" )
	defines( "NDEBUG" )
	optimize( "On" )

outputDir = path.join( rootDir, "pre_binaries" )
binaryLibDir = "bin"
staticLibDir = "lib"

if ( os.istarget( "Windows" ) )
then
	sharedLibDir = "bin"
else
	sharedLibDir = "lib"
end

if ( os.istarget( "Linux" ) )
then
	renderLibXLib = 1
end

include( "libs_config.lua" )
include( "Core/premake5.lua" )
include( "Renderer/premake5.lua" )