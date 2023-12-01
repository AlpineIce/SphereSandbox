# Sphere Sandbox
My own personal environment for learning graphics programming; nothing unique to take away from, just my own documentaiton

To build and run, just use CMake and drop the contents of the "resources" folder into the same folder as your executable. Models and textures can be changed to your own likings, with any format that assimp supports.

Models and materials can be added by naming them similarly to how the already existing ones are. They will automatically be loaded into the engine, but must be coded into existence, just as the other objects are.

The current state of this project is loosely tied together, there arent exactly any shadows, but there are point lights and directional lights, as well as purely elastic sphere collisions. It will likely just end up being a physics based "game" to play around with gravity and collisions.
