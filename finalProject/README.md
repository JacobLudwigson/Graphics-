### Jacob Ludwigson Final Project


### Build Instructions:
- In the main project directory, simply type make to generate an executable
- Type ./final.{executable type} to run the project

### Things to pay attention to:
- Per pixel lighting using shaders.
- Spotlights implemented using shaders
- Player collisions with walls
- Algorithm for "liquid" in tank in the back to bounce, combine and separate.

### Moving around the scene:
- WASD - move the player
- Up,Down,Right,Left arrow to look around (I removed the mouse capture for this review)
- If you want to leave the ship and look around, press m to switch to "fly" mode (just no collisions + y axis movement)
    - Note that as is, the ship isnt really intended to be viewed from the outside, so there are instances of Z-fighting on the outer parts of the ship that I dont really care about, since the ship is for now only meant to be viewed from inside. I intend to fix this, however, its not my highest priority.

### What Ill be working on from now to the deadline:
- Continue building out the ships interior, filling in the desks with objects, adding more lights to the scene, and filling in the ship more
- Possibly, depending on time - work on the black hole, however, after building the spaceship I have realized this is the more graphically interesting part.
- Possibly, depending on time - Refining the liquid algorithm (possibly using linear/quad interpolation of particle positions in between frames to smooth movements, separations and combinations and/or implementing a more "reactive" algorithm that will consider other particles movements and positions more).

### Notes for professor
- I am under the assumption that the algorithm I designed for the particles in the tank and the player collisions is enough for the graduate student "extra" something. 
    - If this is not the case, please let me know so I can work on something else
- If at all possible, in the comments if you could give me an estimation of what grade this project would get AS IS on the final submission, so I have a measure of how I need to manage time for my other projects & finals, that would be awesome. 
- Thanks for the help with getting my makefile up-to-date!

### Reused Code
- Most of the code snippets I have taken have been modified for my use case. However, anything that I have sourced from somewhere else is mentioned above the function
- In particular: The shader compilation functions are not my own and parts of the sphere & cylinder generation functions are not my own.
- I wrote some code following tutorials online, these are cited in the same way. 