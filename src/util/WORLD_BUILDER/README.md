# World Builder

World builder will build each world aka level of the game. These worlds will be 
included in the game builder, which will give the layout of the worlds in 
relation to the game. World builder will create the world layout with the 
objects it is suppose to contain. This contains the world coordinates of the 
local objects, and the collision/physics used for the world. Though each object 
can have its own collision/physics attached.

* Game Object <-- World Object <-- Local Object

This shows the order of first processing the game xml file, based upon the 
worlds (levels) included, process those worlds, then process the objects in 
those worlds.

## Future
* Implement