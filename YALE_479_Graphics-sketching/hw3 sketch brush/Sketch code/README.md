This is a fork of Mr. Doob's Harmony drawing tool, simplified
and modified to be a sandbox framework for students of digital
drawing.


====================================================================
How to create new brushes?
====================================================================
There are in general 3 steps to create a new brush:
1. create a javascript file defining the brush. The brush file is
   recommended to store inside the "js/brushes/" directory.
   - the brush name should be used consistently within the framework:
     the js file name, the main function name (line 1 in the brush js),
     the brush name in BRUSHES (defined in main.js)..etc
   - the brush prototype has to contain these three stroke functions:
        strokeStart(x, y)
        stroke(x, y)
        strokeEnd()
2. add the following script element inside <body> in index.html. Note
   that {BRUSH_NAME} should be the name of your new brush defined in
   step 1.
   <script type="text/javascript" src="js/brushes/{BRUSH_NAME}.js"></script>
3. add the name of your new brush into the variable BRUSHES in main.js


====================================================================
How to create new canvas papers?
====================================================================
There are in general 3 steps to create a new canvas paper:
1. create a javascript file defining the canvas paper. The paper file is
   recommended to store inside the "js/papers/" directory.
   - the paper name should be used consistently within the framework:
     the js file name, the main function name (line 1 in the paper js),
     the paper name in PAPERS (defined in main.js)..etc
   - the paper prototype has to contain this function:
        apply()
2. add the following script element inside <body> in index.html. Note
   that {PAPER_NAME} should be the name of your new paper defined in
   step 1.
   <script type="text/javascript" src="js/papers/{PAPER_NAME}.js"></script>
3. add the name of your new paper into the variable PAPERS in main.js

Two predefined papers were included in the framework: color and sandy.
Each paper demos a different type of paper. "color" is a simple background
paper of a selected solid color. "sandy" is a stroke-effect paper that (1)
creates a sandy texture and (2) applies the sandy effect on the strokes.



Now load index.html into a html5 ready browser (if you don't know which
one to use, Google Chrome is recommended as it has a nice set of development
features), you should be able to find the new paper in the drop-down list
of the paper selector.



====================================================================
License
====================================================================
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
