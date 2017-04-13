# Basic-engine-for-OpenGL
Basic engine to help me learn OpenGL. No pro but learning as I go. Will hopefully expand it's functionality in the future.

I used http://learnopengl.com/ to learn OpenGL and to help myself implement the basics of the engine such as shader loading, texture loading and model loading.

## Result so far
![image](http://i.imgur.com/DQsmYAs.png)

Has model loading, blinn-phong lighting, shadows with a good bias.

## External libraries
* GLFW
* GLM
* Assimp
* STB Image

## Things to do
* Need to add multiple lights
* Need to add common a common material system so the shaders work with either textures or without textures.
* Create a good looking scene!
* (Not important yet) Build basic GUI functionality, probably [NanoVG](https://github.com/memononen/nanovg) but could try making my own basic buttons and sliders.
* Basic forward rendering to begin with, can add deferred rendering functionality once I learn more about it.
