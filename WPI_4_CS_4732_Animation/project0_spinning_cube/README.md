## README

I this up with using WebGL. You shouldn't have any CORS issues.

Of note, I include the [Tiny WebGL Helper Library (TWGL)](https://twgljs.org/) which provides syntactic sugar and allows my code to be less verbose than plain webgl. It doesn't add any major features, so I felt safe adding it.

Because this project is so simple, I have all of my code in `project0.html`.
It contains a vertex shader, a fragment shader, and two javascript functions (`render` which renders a frame and triggers the next one, and `start`, which starts the animation).

You should just be able to open `project0.html` in a browser (I tested it in Chrome) to run the project. Or navigate to https://ezraezra101.github.io/animation-projects/project0_spinning_cube/project0.html

If the cube is somewhat off the screen, you will probably be able to fix it by resizing the page.

If you still can't get it to work, there's a screen capture in `project0.mov` and a YoutTube video at https://youtu.be/I_Ekwo01J5M
