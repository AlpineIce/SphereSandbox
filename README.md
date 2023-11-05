# NothingGL
My own personal environment for learning graphics programming; nothing unique to take away from, just my own documentaiton

Not listed in the code here are the library dependencies which are: assimp, GLFW, GLEW, glm, and stbimage.

Currently, objects are rendered by placing them in the appropriate folders within the build directory with models going into "Models/", materials going into "Materials/", and shaders (glsl files) going into Shaders/. Textures must be named accordingly to be loaded with materials, with names being Albedo, Roughness, Metallic, and Normal. Currently, PBR is the only shader type, so naming a folder within shaders PBR with the vertex and fragment shader in that folder is required.

The current state of this project is loosely tied together, though I'm currently working on implementing lights (point and directional), as well as writing the logic for the PBR shader to work with different texture types. Plans for the future include actors, such as what would be found in a game, as well as a physics engine eventually.
