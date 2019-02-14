project "Ashes"

currentSourceDir = path.join( sourceDir, "Ashes", "Ashes" )
currentBinaryDir = path.join( binaryDir, "Ashes", "Ashes" )

kind( "StaticLib" )
targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", staticLibDir ) )
location( currentBinaryDir )

includedirs{
	path.join( currentBinaryDir, "Src" ),
	path.join( currentSourceDir, "Src" )
}

local configFile = path.join( currentBinaryDir, "Src", "AshesConfig.hpp" )
local content = io.readfile( "./Src/AshesConfig.hpp.in" )
content = string.gsub( content, "${ASHES_WIN32}", os.istarget( "Windows" ) and 1 or 0 )
content = string.gsub( content, "${ASHES_XLIB}", os.istarget( "Linux" ) and 1 or 0 )
content = string.gsub( content, "${ASHES_XCB}", 0 )
content = string.gsub( content, "${ASHES_ANDROID}",0 )
content = string.gsub( content, "${ASHES_MIR}",0 )
content = string.gsub( content, "${ASHES_WAYLAND}",0 )
io.writefile( configFile, content )

forceincludes( "AshesPch.hpp" )

pchheader( "AshesPch.hpp" )
pchsource( "./Src/AshesPch.cpp" )

files{ "./Src/**.hpp",
	"./Src/**.inl",
	"./Src/**.cpp",
	configFile
}

vpaths{ ["Header Files"] = "**.hpp" }
vpaths{ ["Header Files"] = configFile }
vpaths{ ["Header Files"] = "**.inl" }
vpaths{ ["Source Files"] = "**.cpp" }
