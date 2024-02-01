workspace "Application"
    configurations { 
        "Debug", 
        "Release" 
    }

    startproject "Application"

    architecture "x64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Core"
include "Application"
include "external.lua"