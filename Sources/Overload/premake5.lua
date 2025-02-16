workspace "Overload"
	configurations { "Debug", "Release" }
	platforms { "x64" }
	startproject "OvEditor"
	defines { "LUA_SCRIPTING" }

outputdir = "%{wks.location}/../../Bin/"
objoutdir = "%{wks.location}/../../Bin-Int/"
dependdir = "%{wks.location}/../../Dependencies/"
builddir = "%{wks.location}/../../Build/"
resdir = "%{wks.location}/../../Resources/"

include "OvAnalytics"
include "OvAudio"
include "OvCore"
include "OvDebug"
include "OvMaths"
include "OvPhysics"
include "OvRendering"
include "OvTools"
include "OvUI"
include "OvWindowing"

include "OvEditor"
include "OvGame"

include "../../Resources"
