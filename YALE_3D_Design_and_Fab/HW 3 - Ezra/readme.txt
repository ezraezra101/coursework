# HW 3/4 Fabrication class - October 27th
Ezra Davis

### Corner detector

There's still a few issues in not getting exactly the same result as the examples...

Compile with:
`g++ corner_detector.cpp;`

Run with:
`./a.out mandrill.ppm >mandrill_my_out.pgm`


##### Resources
I had a long conversation with Sherry about debugging (based on our image results) and realized that I was running into issues with ignoring negative numbers (and integer wraparound)... which was entirely unrelated to my other integer overflow issues.

I also looked at a series of slides about Harris image matching here: http://www.cse.psu.edu/~rtc12/CSE486/lecture06.pdf


### 3D print and scan

Because `vsfm` didn't agree with my pictures of a sea shell I found on the beach (it didn't find any feature matches)... possibly because I didn't use it, I instead used (COLMAP)[https://colmap.github.io], which is a similar application that is functionally equivalent (that was much easier to install).

I then imported to meshlab, reconstructed a ply file with faces (Poisson reconstruction), cut off the bottom (in Blender), and then made it into gcode in Cura.



### Summary of paper

This paper explores a new method for combining photos into a scene using a new (at the time) technique for determining camera locations using SIFT features, and morphing between photographs based on their relative location.

They find camera locations by adding one image at a time, re-optimizing the non-linear least squares problem that gives camera locations, and repeating this process until all photos have been integrated. This allows good initialization variables for the last re-optimization and helps avoid local minima and throw out outliers early.
The paper also can incorporate elevation maps and other geodata into these estimations.

There are two rendering modes: a non-photorealistic mode where the images are drawn as translucent planes in 3-space, and a more realistic one where you largely look at one camera at a time.
When transitioning between views, the camera interpolates its position in a straightforward manner, but there are two options view interpolation: planar morphs (projecting the images onto the same plane and crosfading) and triangular morphs (figuring 3D points they have in common and stretching the image between them, fillin in the edges with a grid...)

It also features several ways of annotating and navigating between pictures, or moving in the full 3D scene, but these tools are a little hard to judge without having the tool (or the video) on hand.

You can also add new photos at runtime (which took only ~10 seconds in testing), which doesn't perform the global optimization or move the other cameras.