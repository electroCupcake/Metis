# Game Builder

Game builder parses a file to find out what world objects are contained in the 
game. These world objects contain local objects. The order of the worlds is 
set, and how to transistion from each world is set.

* Game Object <-- World Object <-- Local Object

This shows the order of first processing the game xml file, based upon the 
worlds (levels) included, process those worlds, then process the objects in 
those worlds. 

## Future
* Implement