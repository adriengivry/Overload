![Overload Logo](https://user-images.githubusercontent.com/33324216/94352497-cc8c2200-0033-11eb-93e1-1a30386133b9.png)

<p align="center">
  <a href="https://github.com/adriengivry/Overload/releases">Releases</a> |
  <a href="#screenshots">Screenshots</a> |
  <a href="#features">Features</a> |
  <a href="https://github.com/adriengivry/Overload/wiki">Wiki</a> |
  <a href="https://github.com/adriengivry/Overload/blob/develop/CONTRIBUTING.md">Contributing</a>
<br/>
<br/>
<br/>
<a href="https://github.com/adriengivry/Overload/releases"><img alt="platforms" src="https://img.shields.io/badge/platforms-Windows-blue?style=flat-square"/></a>
<a href="https://github.com/adriengivry/Overload/releases"><img alt="release" src="https://img.shields.io/github/v/release/adriengivry/overload?style=flat-square"/></a>
<a href="https://github.com/adriengivry/Overload/tree/develop/Sources/Overload"><img alt="size" src="https://img.shields.io/github/repo-size/adriengivry/overload?style=flat-square"/></a>
<br/>
<a href="https://github.com/adriengivry/overload/issues"><img alt="issues" src="https://img.shields.io/github/issues-raw/adriengivry/overload.svg?color=yellow&style=flat-square"/></a>
<a href="https://github.com/adriengivry/overload/pulls"><img alt="pulls" src="https://img.shields.io/github/issues-pr-raw/adriengivry/overload?color=yellow&style=flat-square"/></a>
<br/>
<a href="https://github.com/adriengivry/Overload/blob/develop/LICENSE"><img alt="license" src="https://img.shields.io/github/license/adriengivry/overload?color=green&style=flat-square"/></a>
<a href="https://github.com/adriengivry/Overload/releases"><img alt="downloads" src="https://img.shields.io/github/downloads/adriengivry/overload/total?color=green&style=flat-square"></a>
<br/>
<br/>
<br/>
<a href="https://discord.gg/wqe775s"><img src="https://img.shields.io/discord/622075717659656195.svg?label=&logo=discord&logoColor=ffffff&color=7389D8&labelColor=6A7EC2" height=30></img></a>
</p>

# 1. What's Overload?
## 1.1. Description
Overload is a 3D game engine inspired by industry standards, initiated by [Benjamin VIRANIN](https://github.com/BenjaminViranin), [Max BRUN](https://github.com/maxbrundev) and [Adrien GIVRY](https://github.com/adriengivry) as a graduation project. It is now a community project open to external contribution.

Overload pillars are:
- Simplicity
- Documentation
- High-quality modern C++ code

## 1.2. Modules
Overload is divided into 12 modules (10 DLLs and 2 executables):

### 1.2.1. Overload SDK
The Overload SDK is the core of the engine. It is a set of libraries used by our applications: `OvGame` and `OvEditor`.
We designed theses libraries with reusability in mind. They are highly modular and easy to extract from a game engine context.
- `OvAnalytics`: Code and hardware profiling
- `OvDebug`: Logging and assertions
- `OvTools`: Serialization, INI files, events, clock
- `OvMaths`: Vectors, matrices, quaternions, transforms
- `OvAudio`: Wraps irrKlang
- `OvPhysics`: Wraps Bullet3
- `OvRendering`: Rendering engine using OpenGL
- `OvWindowing`: GLFW and Windows API wrapper
- `OvUI`: Dear ImGui wrapped into an event-based and component-based UI system
- `OvCore`: Component-based scene system, scripting, and resource management

### 1.2.2. Overload Applications
- `OvGame`: Uses OvCore, asset-dependent, a generic executable for any game built with Overload
- `OvEditor`: Uses OvCore, exposes game development to the end-user (from creation to building)

![Editor](https://user-images.githubusercontent.com/33324216/94352908-fd228a80-0038-11eb-849a-c076bde4c7c6.PNG)

# 2. History of Overload
## 2.1. Context
Overload started as a graduation project. The three os us, ([Benjamin VIRANIN](https://github.com/BenjaminViranin), [Max BRUN](https://github.com/maxbrundev) and [Adrien GIVRY](https://github.com/adriengivry), worked on it for five months. Our task was to create a game engine from scratch, using as few external libraries as possible.

## 2.2. Goals
Our goals for this project were to:
- Understand how to architect a game engine
- Design an application with end-users in mind
- Make the game engine as simple as possible
- Produce high-quality modern code
- Create a game using our engine
- Structure and manage a long-term project
- Develop documentation for both developers and end-users

## 2.3. Pre-production
We spent about a month designing our initial architecture.  We provided technical solutions using UML diagrams, flowcharts, package diagrams, and use-case diagrams. This helped us to start the production with a clear vision of the engine.

We also defined coding conventions to ensure our work remained consistent throughout the production process. Defining these kinds of rules is crucial to maintaining clean, manageable code over the course of the project.

## 2.4. Production
During production, we realized that our initial understanding of what a game engine entails was naive. As we progressed, our architecture evolved. We discovered that our original design was sometimes overly complex or suboptimal. As a result, we revisited and refined the architecture multiple times throughout the project.

<span name="features"></span>
# 3. Features
## 3.1. Implemented
Here is a non-exhaustive list of Overload's main features:
- Game Editor
- Lua scripting
- Physically-Based Rendering (PBR)
- Custom shaders support
- Windows game building
- Profiling tools (Editor and build)
- Material editor
- 3D sound system
- Rigidbody physics

## 3.2. To Be Implemented
A non-exhaustive list of upcoming features for Overload:
- Expanded input device support (Gamepad)
- Prefab system
- Skeletal animation
- User script profiling

# 4. Details
## 4.1. Software
- Visual Studio 2022 (previous versions should also be supported)

## 4.2. Dependencies
- OpenGL with GLEW (Graphics API)
- GLFW (Windowing and inputs)
- Assimp (3D model loader)
- Bullet3 (Physics)
- irrKlang (Audio)
- Tinyxml2 (XML Serializer)
- SOL2 (Lua binder)
- ImGui (GUI)

## 4.3. Requirements
Overload requires:
- RAM: 100MB
- OS: Windows 7
- GPU: Graphics card supporting OpenGL 4.3
- CPU: x64

## 4.4. Compiling sources
Overload uses Premake5 to generate project files. To generate these files, execute `GenerateProjects.bat` located at the root of the repository. By default, `GenerateProjects.bat` will generate project files for Visual Studio 2022. If you want to use another version of Visual Studio (e.g., VS2019, VS2017) or different IDE (see https://github.com/premake/premake-core/), you can execute `GenerateProjects.bat` from the command line with an argument. (e.g., `.\GeneratedProjects.bat vs2019`)<br><br>

*Overload only targets Windows 64 bits.*

## 4.5. Licence
Overload is licenced under an MIT licence.

## 4.6. More information
If you're interested in Overload, you can download our engine and the demo game we created with it on our website:<br>
https://overloadengine.org

Learn about Overload (tutorials and scripting API) by visiting our Wiki:<br>
https://github.com/adriengivry/Overload/wiki

You can also watch our features reel (Overload v1.0.0) on YouTube:<br>
https://www.youtube.com/watch?v=ARXSJh-ZMHM

Join our Discord Server:<br>
https://discord.gg/wqe775s<br>

# 5. Getting started
Learn how to create your own games using Overload by visiting our [wiki](https://github.com/adriengivry/Overload/wiki).

# 6. Contributing
Overload is open to contributions of all kinds. Feel free to open issues (feature requests or bug reports) or submit pull requests.

If you'd like to contribute, please refer to our [CONTRIBUTING.md](https://github.com/adriengivry/Overload/blob/develop/CONTRIBUTING.md) guidelines.

<span name="screenshots"></span>
# 7. Screenshots
![PBR Shading](https://user-images.githubusercontent.com/33324216/94352806-96e93800-0037-11eb-8d7f-9c9a318ca2c7.PNG)
![Material Editor](https://user-images.githubusercontent.com/33324216/94352805-96e93800-0037-11eb-883b-fdd8818b93a6.PNG)
![Standard Shader Library](https://user-images.githubusercontent.com/33324216/94352810-9781ce80-0037-11eb-8788-095794711b2c.PNG)
![Custom Shaders](https://user-images.githubusercontent.com/33324216/94352802-9650a180-0037-11eb-8931-c6b2163c0ef1.PNG)
![Realtime Lighting](https://user-images.githubusercontent.com/33324216/94352808-9781ce80-0037-11eb-8b91-3ec0ab06db45.PNG)
![Scene Edition](https://user-images.githubusercontent.com/33324216/94352809-9781ce80-0037-11eb-9adf-d216eb4d963e.PNG)
![Build System](https://user-images.githubusercontent.com/33324216/94352926-5db1c780-0039-11eb-88ef-7ca14a8bc821.PNG)
![Project Hub](https://user-images.githubusercontent.com/33324216/94352807-9781ce80-0037-11eb-911b-7e3d0d00ce41.png)
