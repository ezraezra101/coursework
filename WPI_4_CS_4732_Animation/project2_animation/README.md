## README

I wrote this with WebGL. It shouldn't have any CORS issues.

Of note, I include the [Tiny WebGL Helper Library (TWGL)](https://twgljs.org/) which provides syntactic sugar and allows my code to be less verbose than plain webgl. It doesn't add any major features, so I felt safe adding it.

I also include the [glMatrix](http://glmatrix.net/) library, which is a library with simple matrix operations. It has quaternion support, and I use two functions from it to fix bugs in my `Quaternion.js` file.

##### Files:
- `interpolate.js` - contains all functions related to interpolation
- `project2.html` - The main html file. Contains shaders and rendering functions.
  - `project2.html` also contains the splines and an OBJ file in textareas. (I would have put these in other files, but AJAX doesn't work on local files, and I wanted this to be able to run without a server)
- `Quaternion.js` - contains my quaternion class
- `readSpline.js` - contains functions to read in the spline text into a javascript object
- `readOBJ.js` - A stripped down OBJ file reader. Will fail on *anything* complicated (e.g. non-triangle shapes)
- `style.css` - contains minimal styling for `project2.html`

You should just be able to open `project2.html` in a browser (I tested it in Chrome and Safari) to run the project. Or navigate to https://ezraezra101.github.io/animation-projects/project2_animation/project2.html

You should just be able to open project0.html in a browser (I tested it in Chrome) to run the project. 

If the cube is somewhat off the screen, you will probably be able to fix it by resizing the page.

If you still can't get it to work, there's a screen capture in project2.mov and a YoutTube video at https://youtu.be/mydpeGZ7Frg
