# Basic-engine-for-OpenGL
Basic engine to help me learn OpenGL. No pro but learning as I go. Will hopefully expand it's functionality in the future.

I used http://learnopengl.com/ to learn OpenGL and to help myself implement the basics of the engine such as shader loading, texture loading and model loading.

## Result so far
![image](http://i.imgur.com/pmFfnM3.png)

This scene uses a free model from http://graphics.cs.williams.edu/data/meshes.xml but in obj format and does not textures. Has point light with shadows.

## External libraries
* GLFW
* GLM
* Assimp
* STB Image

## Things to do
* Need to add multiple lights
* Add normal mapping and parallax mapping
* Want to add VSM. Looked into it but need to understand it better.
* Create a good looking scene!
* (Not important yet) Build basic GUI functionality, probably [NanoVG](https://github.com/memononen/nanovg) but could try making my own basic buttons and sliders.
* Basic forward rendering to begin with, can add deferred rendering functionality once I learn more about it.
