Engine Development
==================

## Overview

This is the student implementation of the Full Sail game engine [Burnsidious](https://github.com/Burnsidious) 
An in house game engine developed for use in lab assignments and student projects at Full Sail University. The engine implementation is partly done in libraries that students have to replace as they go through various courses such as Engine Development, Networking and AI.

## Student Implementation

Marked as //TODO, the student needed to comment the specific library that implemented the functionality and came with an alternative implementation.

+ File Packer Lab 
	This lab required to write the functionality to load the directory of a PAK file, load a file’s content from the PAK file to a buffer, and supplementary functions.

+ Matrix Lab
	For this lab we implemented various algorithms related to entity behavior and camera movement using matrices.  The FSGD Engine is also a good opportunity to practice problem solving and debugging skills as well as trying to familiarize with a large pre-written code base.
	
+ Memory Manager Lab
	This lab was designed to show a simple implementation of a Memory Management system. It was good training  for testing familiarity with pointers, cyclic doubly linked lists, and problem solving skills.
	All functions were implemented inside the FS Student Game Engine, this mimics the real world scenario of working within an existing code base.  Code is written in MMHeap_TODO.cpp and consists of the following functions. Reading the class declaration in MMHeap.h is also advised. Note: If the flow of execution is updated be sure to lock/unlock the heap appropriately. To view memory usage metrics pull down the output console with “o” then use “u/i” to reach the memory page. To dump this information (and more), use the following command in the input console MetricDump.
	To execute a core dump, open up the input console (~) and use the command CoreDump.

+ Scripting Lab
	The objective of this lab was to familiarize with Python control statements and C++ interfaces.

+ Thread Pool Lab
	This lab was designed to show a simple implementation of a thread pool system. It tested the familiarity with pointers, multi threading, mutexes, working within a pre-existing code base, and problem solving skills.
	
+ Shadow Mapping Lab
	In this lab was used to implement shadow mapping.
	
+ Deferred Shading Lab
	This lab implements the basics of a Deferred Shading system.

+ Hierarchy and Sorting Lab
	Implements the Frame class that handles hierarchy flattening and the RenderSet classes which contains batches for rendering and sorting.

+ Renderer Lab
	Basics of a Rendering engine; the Renderer loops through batches of RenderNodes calling their RenderProcess method. The RenderNode’s RenderProcess method calls the RenderFunc function pointer. 
	RenderContext is a child of RenderNode and is used to set different rendering states such as the current shader, vertex stream and input layout.  The RenderShape is also a child of RenderNode; its RenderFuncs makes draw calls using data obtained from references to the RenderMesh and RenderContext classes. The RenderMesh class stores vertex and index data needed for rendering.

+ Level of Detail Lab
	Implements the selection part of a discrete level of detail system. The switching portion of the system was actually already written in the previous Renderer lab. By using the “accessor” methods for getting access to the Mesh and Context to be used in our render functions we in turn use the current LOD.
	
+ Post-Process Effect Lab
	Implements a system to handle Post-Process Effects. It also implements solutions for gaussian blur, depth of field and edge drawing effects. use the console to turn them on.
	+ List of commands:
		+ AddBlurH – Adds a horizontal Gaussian blur pass.
		+ AddBlurV – Adds a vertical Gaussian blur pass.
		+ AddDoF – Adds a down sample, a series of blurs, an up sample and finally a depth of field combine process.
		+ AddEdges – Adds an edge drawing effect pass.
		+ ClearPost – clears all post-process effects

+ Advanced Lighting Lab
	This lab implements shader code to support lighting in the engine's deferred renderer, as well as solutions for normal mapping and ambient occlusion mapping.
	+ GBuffers_PS
	+ GBuffersBump_VS.hlsl
	+ GBuffersBump_PS.hlsl
	+ DeferredDirLight_PS.hlsl
	+ DeferredPointLight_PS.hlsl
	
+ Terrain Tessellation Lab
	In this lab you will be implementing the Tessellation pipeline stages used by the terrain system. The Tessellator pipeline stage itself is not programmable, instead a Hull shader is used to influence what the tessellator does, and a Domain shader is used to handle the results of tessellation.
To have the engine use the tessellated terrain system you will need to change the level that is run at startup. Open the file settings.xml ( ctrl + alt + ‘o’, then type “open settings.xml”). Near the top of this XML file you will find a “Level” tag, set its data to “Levels/ed2_tessLevel.xml”. When running the engine now, you should see the new terrain. The terrain does not have collision detection enabled at this time.

+ Debug Buffers Lab
	The goal of this assignment was to provide support for viewing additional rendering buffers in real-time for debugging purposes. Renders the depth buffer, diffuse color buffer, normal buffer, or specular buffer based on user input.  The 2, 3, and 4 keys will toggle viewing the additional buffers.
		
## Controls

+ WASD 		– move the camera
+ Buggy:  I – drive forward, K drive backwards, J turn left, L turn right
+ Mouse to pick up and drag entities
+ PGUP/PGDN – Change the Camera Mode
+ Keys 2, 3, and 4 will toggle viewing the additional debug buffers (Note: normal scene should be visible any time)
+ "~" opens the console
	
## Built With

* [Visual Studio 12](https://visualstudio.microsoft.com/)	- C++ development

## Contributing

Please read [CONTRIBUTING.md](https://github.com/Cabrra/Contributing-template/blob/master/Contributing-template.md) for details on the code of conduct, and the process for submitting pull requests to me.

## Authors

* [credits](https://github.com/Burnsidious/FSGDEngine/blob/master/credits.txt)
* **Jonathan Burnside** 	- Course Directors 			- [Burnsidious](https://github.com/Burnsidious/FSGDEngine/blob/master/credits.txt)
* **Brandon Patterson** 	- Course Director
* **Jagoba "Jake" Marcos** 	- Student implementation -[Cabrra](https://github.com/Cabrra)

## License

This project is licensed under the MIT license - see the [LICENSE](LICENSE) file for details

## Acknowledgments

* Full Sail University - Game Development Department
