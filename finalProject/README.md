### Jacob Ludwigson Final Project


### Build Instructions:
- In the main project directory, simply type make to generate an executable
- Type ./final.{executable type} to run the project

### Things to pay attention to:
- Per pixel lighting using shaders.
- Spotlights implemented using shaders
- Player collisions with walls
- Algorithm for "liquid" in tank in the back to bounce, combine and separate.
    - If this is giving you framerate problems, press 2 to stop the movement algorithm from running.
- 3D convex hull algorithm for the point set of the liquid in the tank (Look out the window at the red thing in the glass sphere)
    - This will give your computer a run for its money, convex hull is an expensive computation, especially when degenerate point sets are involved. 
    - If this is giving you framerate problems simply press 4 to stop drawing it OR press 2 to stop calculating new movements and new hulls (this is the better option).
- Flashlight! (Press f to equip, g to unequip)

### Moving around the scene and controls:
- WASD - move the player
- Up,Down,Right,Left arrow to look around (I removed the mouse capture for this review)
- God mode: M - on, Space - off (default off) (This will allow you to fly outside of the ship with no collisions)
- Flashlight: F - on, Space - off (default on)
- Particle Movement: 1 - on, 2 - off (default on) (you should probably just press 2 if you are not looking at the particles or the convex hull bc this is expensive and slow.)
- Convex Hull Drawing: 3 - on, 4 -off (default on) (This is if you want to completely remove the drawing of the hull, simply pressing 2 will stop it from recalculating new ones.)
- Ship lights: 5 - on, 6 - off (default off)

### NOTES ON TRIANGULARIZED HULL ALGORITHM (Funky moving shape in the middle) - read if interested not required for project grading
- This algorithm is based on the algorithm described in this paper.
    - https://web.archive.org/web/20180106161310/http://thomasdiewald.com/blog/?p=1888

- This algorithm attempts to find a 3d encapsulating polygon with triangles placed on the vertices of an arbitrary point cloud.

- The intention behind this was to develop something unique with graphics applications (I explicitly avoid marching cubes because I didnt want to copy the example project's idea).

- A convex hull can only be defined on a non degenerate point set, the point cloud I run this on has no guarantees of that. I.E. a coplanar or cospherical set of points will do weird stuff!

- Since my algorithm has to consider the degenerate point set, I was not able to achieve the O(nlogn) outlined in the paper, since utilizing a stack of faces being considered minimizes time complexity from O(n faces squared) but will disincentivize the algorithm evaluating the 'more important' faces that were pushed earlier (i.e. the initial tetrahedron faces) (and if the degenerate set is found, it will NEVER evaluate these faces leading to a considerably more noticeable failure to construct a proper hull - Trust me, I tried.).

- Hopefully you think this is as cool as I do, it was really hard.

### Reused Code
- Most of the code snippets I have taken have been modified for my use case. However, anything that I have sourced from somewhere else is mentioned above the function
- In particular: The shader compilation functions are not my own and parts of the sphere & cylinder generation functions are not my own.
- I wrote some code following tutorials online, these are cited in the same way. 