# IMAGE TO TEXTURE LIBARY

Image to texture takes an image file and converts it to raw image data in the 
original color format. It can also swap the colors based on the method called 
on the data. At this point this library handles TIM images and bitmap images of 
the 16 bit ARGB or ABGR type. The playstation uses ABGR for the textures.

## Future
* 24 bit bitmap support (maybe others).
* 4,8,24 bit TIM support.
* Other Image formats... png, jpeg, gif, etc
* Routines to convert to 16 bit, 8 bit or 4 bit (all can be used direclt in the 
* PSX, of course, 4 and 8 bit with a CLUT)/
