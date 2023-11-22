# NothingGL
My own personal environment for learning graphics programming; nothing unique to take away from, just my own documentaiton

Currently, objects are rendered by placing them in the appropriate folders within the build directory with models going into "Models/", materials going into "Materials/", and shaders (glsl files) going into Shaders/<NAME>/. Textures must be named accordingly to be loaded with materials, with names being Albedo, Roughness, Metallic, and Normal. Currently, PBR is the only shader type.

The current state of this project is loosely tied together, though I'm currently working on implementing lights (point and directional), as well as writing the logic for the PBR shader to work with different texture types. Plans for the future include actors, such as what would be found in a game, as well as a physics engine eventually.
