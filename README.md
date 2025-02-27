# Multi Spheres openGL POC


(Slightly hurried readme)
I tried this mini project with the following goals in mind.
1. Render 1M spheres
2. Show dynamic lighting
3. Unique materials per sphere.
4. Highlighting for each sphere.
5. Maintaining 60 FPS.
6. Learning openGL (because why not :D)

Actual specs - 
1. Rendered 10k spheres with instances method.
2. Single light source to emulate phong lighting with proper attenuation techniques.
3. 4 unique texture support (more can be added tbh).


## Demo
[demo](https://youtube.com/shorts/R5YZGdQ08io?si=w2w5Tvhtb_oF_9hc)

Things to improve?
- Get a GPU
- Clean up utility methods into a class.
- Make a separate lighting class.



## How to compile?

```bash
$ g++ -g -std=c++11 main.cpp src/* -o multi_cube \
-Iinclude -I/opt/homebrew/include -L/opt/homebrew/lib \
-lglfw -lglew \
-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo \
-DGL_SILENCE_DEPRECATION

$ ./multi_cube
```

### 🎮 Camera Controls

| Action                | Key                          |
|-----------------------|-----------------------------|
| **Move Forward**      | `W`                         |
| **Move Backward**     | `S`                         |
| **Move Left**         | `A`                         |
| **Move Right**        | `D`                         |
| **Move Up**           | `Space`                     |
| **Move Down**         | `Z`                         |
| **Sprint (Faster Movement)** | `Left Shift (Hold)`  |
| **Stop Sprinting**    | `Left Shift (Release)`      |
| **Look Around**       | `Mouse Left Click + Move Mouse` |
| **Unlock Cursor**     | `Mouse Left Click (Release)` |
| **Exit Application**  | `Escape`                    |

## Future plans?
Try porting this to wasm!