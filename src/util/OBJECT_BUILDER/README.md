# Object Builder

Object builder builds the local objects that are a part of the world. These 
objects are centered so rotation and scaling work as they should by default 
(around the center of the object). Also the physics and collision can be set 
here, or the one from the world will be used.

* Game Object <-- World Object <-- Local Object

This shows the order of first processing the game xml file, based upon the 
worlds (levels) included, process those worlds, then process the objects in 
those worlds. 

## Future
* Correct vertex vs vector