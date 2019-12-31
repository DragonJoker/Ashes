project "GlRenderer"

currentSourceDir = path.join( sourceDir, "Ashes", "GlRenderer" )
currentBinaryDir = path.join( binaryDir, "Ashes", "GlRenderer" )

kind( "SharedLib" )
targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", sharedLibDir ) )
implibdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", staticLibDir ) )
location( currentBinaryDir )
defines( "GlRenderer_EXPORTS" )

includedirs{
	path.join( sourceDir, "Ashes", "Ashes", "Src" ),
	path.join( binaryDir, "Ashes", "Ashes", "Src" ),
	path.join( currentBinaryDir, "Src" ),
	path.join( currentSourceDir, "Src" )
}

links{
	"Ashes"
}

if ( os.istarget( "Windows" ) )
then
	links{
		"opengl32",
		binaryLinks
	}
else
	links{
		"GL",
		binaryLinks
	}
end

forceincludes( "GlRendererPch.hpp" )

pchheader( "GlRendererPch.hpp" )
pchsource( "GlRendererPch.cpp" )

files{ "./**.hpp",
	"./**.inl",
	"./**.cpp"
}

vpaths{ ["Header Files"] = "**.hpp" }
vpaths{ ["Header Files"] = "**.inl" }
vpaths{ ["Source Files"] = "**.cpp" }
