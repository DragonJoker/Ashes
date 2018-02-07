newoption  {
  trigger     = "build-tests",
  description = "Enables tests build"
}

newoption {
  trigger     = "build-samples",
  description = "Enables tests build"
}

workspace( "RendererLib" )

sourceDir = os.getcwd()
rootDir = path.getdirectory( sourceDir )
binaryDir = path.join( rootDir, "build", "premake" )
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

outputDir = path.join( rootDir, "binaries" )
executableDir = "bin"
staticLibDir = "lib"
assetsDir = "share"

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
group( "Core" )
include( "Core/premake5.lua" )
group( "Renderer" )
include( "Renderer/premake5.lua" )

if _OPTIONS["build-tests"] then
	group( "Test" )
	include( "Test/premake5.lua" )
end

if _OPTIONS["build-samples"] then
	group( "Samples" )
	include( "Samples/premake5.lua" )
end