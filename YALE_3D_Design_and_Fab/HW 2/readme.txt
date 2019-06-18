
## Overview

My homework is divided into several files:
- `3D printing/` where my scanning and printing files are.
	- `beethoven...` - Files related to the 3D scan of Beethoven's head
	- `squirrels.stl` and `squirrels.blend` - I smoothed my squirrel, and just for the fun of it, I made a new squirrel in blender in about 3 minutes.
	- `prints.jpg` - a photo of the final prints for both squirrels and Beethoven.
	- `squirrels_and_beethoven.gcode` - gcode file for the printer
- `sum_triangle/` - files associated with part 1A of the homework.
- `polyhedron_prog_subdiv_off/` - part 1B of the homework
- `Voronoi_diagram_2/` - part 1C


## 1A Convex hull and minkowski sum operations

##### Build instructions:
```bash
cd sum_triangle
cmake .
make
```
###### Run instructions
```bash
# To generate a set of triangles:
ruby generate_triangles.rb

./sum_triangle_triangle triangles_100.txt # creates ms_triangles_100.txt
./sum_triangle_triangle_ch triangles_100.txt # creates ch_triangles_100.txt
```
###### Description and results:

Unsurprisingly, the convex hull operation produces fewer ouput points than just the minkowski sum, and not always the same number for the number of input points given. (Also, the minkowski sum prints out the number of points returned, whereas the convex hull version part doesn't)

Creating the convex hull is a neglidgable part of the process - taking only roughly 1/20th of a second longer than just performing the minkowski sum.
```bash
time  ./sum_triangle_triangle ../triangles_10000.txt;  time  ./sum_triangle_triangle_ch ../triangles_10000.txt
	real	0m3.943s
	user	0m3.876s
	sys	0m0.061s

	real	0m3.981s
	user	0m3.921s
	sys	0m0.058s
```

## 1B Subdivision of OFF file

##### Build instructions:
```bash
cd polyhedron_prog_subdiv_off
cmake .
make
```
###### Run instructions:
```bash
./polyhedron_prog_subdiv_off ./corner.off
# produces corner_sd.off

```
###### Description:
This program subdivides each face into smaller, smoother faces, causing the number of faces to increase by a factor of roughly four, depending on the shape of the faces in the model. Incidentally, the program triangulates the mesh.
The number of vertices in the new mesh is equal to the sum of the vertices and faces in the old mesh.


## 1C
##### Build instructions:
```bash
cd Voronoi_diagram_2
cmake .
make
```
###### Run instructions:
```bash
./vd_my_program.cpp
```

###### Description:
`vd_2_point_location`, the original example for `Voronoi_diagram_2` creates a voronoi diagram based on the contents of `data/data1.dt.cin`, and then finds out where the points in `data/queries1.dt.cin` are and prints out some information about the points' location in the voronoi diagram.

I modified the program to print out the boundaries of all the faces adjacent to the original point.

## 2
All files relevant to this section are in the `3D printing/` directory.

I used the NextEngine scanner with Professor Brown's software to scan a bust of Beethoven, which I then printed along with a smoothed version of my squirrel and a new squirrel I threw together for the fun of it.


## 3

### Scanning principles
#### Sturctured Light Camera
	In the structured light approach is an active stereo- vision technique. A sequence of known patterns is sequentially projected onto an object, which gets deformed by geometric shape of the object. The object is then observed from a camera from a different direction. By analyzing the distortion of the observed pattern, i.e. the disparity from the original projected pattern, depth information can be extracted... - *page 4*
The structured light version of the Kinect uses a dot pattern projected by a near infrared laser which is viewed from a camera 7.5 cm away (there is also a RGB camera between the two, but this isn't used for 3D scanning). The Kinect uses a single dot pattern to calculate depths, so each the correspondence between pixels and locations in the pattern is detected using a window that is usually 9x9 pixels.

#### Time of Flight
The newer version of the Kinect uses "Continuous Wave Intensity Modulation", which modulates a light source according to some pattern - typically a sine wave. An IR camera picks up peaks (or troughs) in the returning light, and uses that to determine the phase offset in the signal for each pixel.
The Kinect uses 10 images, 3 photos at different phases at 3 different frequencies to calculate the distance. The 10th image is used for mysterious purposes.

This runs counter to my previous (incorrect) belief that the camera was somehow determining the phase of the emitted light (the phase of the 850nm signal), instead of a slower signal with the infrared frequencies acting as a carrier wave.


### Quality of results

Comparing the two scanners, the structured light scanner is more accurate when exposed to ambient light or translucent liquids, but cannot read anything when past a certain threshold. It is also slightly more inaccurate for farther away objects. The time of flight scanner handles reflective objects pointing towards the camera worse, but is better at marking invalid pixels on moving objects.

If we have multiple scanners in the same room, the time of flight scanner will have spikes of error periodically, whereas the structured light scanner is more consistent, except when the second scanner is placed at very specific positions.


---------

## Outside sources

- I had a brief conversation with Nathan and Sherry comparing the output of our sum_triangle_triangle program.
- I quickly googled how to use the `time` terminal command.
- I also used cplusplus.com to learn a bit about C++
- I received Professor Brown's help with scanning Beethoven's head
- I used the CGAL documentation to puzzle out what Voronoi's examples actually does (incidently, Voronoi's circulators don't match the general circulator documentation for `operator==`)