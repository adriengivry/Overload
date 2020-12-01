project "OvWindowing"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	files { "**.h", "**.inl", "**.cpp" }
	includedirs { "include", dependdir .. "glfw/include", dependdir .. "stb_image/include", "%{wks.location}/OvTools/include" }
	targetdir (outputdir .. "%{cfg.buildcfg}/%{prj.name}")
	objdir (objoutdir .. "%{cfg.buildcfg}/%{prj.name}")
	characterset ("MBCS")

	filter { "configurations:Debug" }
		defines { "DEBUG" }
		symbols "On"

	filter { "configurations:Release" }
		defines { "NDEBUG" }
		optimize "On"