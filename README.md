# Curves and Surfaces

Uses C++ code that is rendered using OpenGL and the GLFW framework.
Generates 5 different scenes with interactive (via keyboard and mouse controls) B-spline and Bezier curves and surfaces.
This program modifies the viewing pipeline for 3D projection and viewing.

## Usage and Keyboard Controls

### Change Scenes:
---------------------
1 Key - curve creation

2 Key - 3D curve projection

3 Key - surface of revolution

4 Key - Bezier tensor surface

5 Key - B-Spline tensor surface

### Control Camera (scenes 2,3,4,5):
--------------------------------
W Key - moves camera forward

S Key - moves camera backward

A Key - moves camera left

D Key - moves camera right

Cursor Position (top of window) - rotates scene upwards

Cursor Position (bottom of window) - rotates scene downwards

Cursor Position (left side of window) - rotates scene counterclockwise

Cursor Position (right side of window) - rotates scene clockwise

### Surface/Curve View:
---------------------
Left Arrow Key - bezier curve in scene 1; wireframe in scenes 3,4,5

Right Arrow Key - bspline curve in scene 1; wireframe in scenes 3,4,5

### Curve Creation/Editing (scene 1):
---------------------
Left Mouse Button - adds point at click

Left Mouse Button Click and Drag - selects a point to be moved (in blue) and places where button is released

Right Mouse Button - deletes point at click

F5 Key - resets curve points

## Running and Compiling
Running and Compliing of this program will require several third party GLFW libraries (argh- 1.3.1, fmt-7.0.3, glew-2.1.0, glfw-3.3.2, glm-0.9.9.7, imgui-1.78, stb-2.26, vivd-2.2.1)
