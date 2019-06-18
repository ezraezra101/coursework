## README

I wrote this with WebGL. It shouldn't have any CORS issues.

Of note, I include the [Tiny WebGL Helper Library (TWGL)](https://twgljs.org/) which provides syntactic sugar and allows my code to be less verbose than plain webgl. It doesn't add any major features, so I felt safe adding it.

I also include the [glMatrix](http://glmatrix.net/) library, which is a library with simple matrix operations. It has quaternion support, but I didn't use it.

##### Missing features:
Bspline interpolation is not working.

##### Files:
- `interpolate.js` - contains all functions related to interpolation
- `project1.html` - The main html file. Contains shaders and rendering function.
- `Quaternion.js` - contains my quaternion class
- `readSpline.js` - contains functions to read in the spline text into a javascript object
- `style.css` - contains minimal styling for `project1.html`

You should just be able to open `project1.html` in a browser (I tested it in Chrome and Safari) to run the project.

If the cube is somewhat off the screen, you will probably be able to fix it by resizing the page.
