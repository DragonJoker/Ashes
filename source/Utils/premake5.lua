project( "Utils" )

currentSourceDir = path.join( sourceDir, "Core", "Utils" )
currentBinaryDir = path.join( binaryDir, "Core", "Utils" )

kind( "StaticLib" )
targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", staticLibDir ) )
location( currentBinaryDir )

files{
	"**.hpp",
	"**.inl",
	"**.cpp"
}

vpaths{ ["Header Files"] = "**.hpp" }
vpaths{ ["Header Files"] = "**.inl" }
vpaths{ ["Source Files"] = "**.cpp" }