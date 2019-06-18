Blades of the High Seas
Brian Rubenstein & Ezra Davis

--------------------------------------------------------------------

To build:

0) Setup development environment for MacOS.
See http://dragonfly.wpi.edu/ for details.
It's probably possible to use a different engine, but the currently included one is built for MacOS.

1) Run `make`

2) Run `./game` to start the game

--------------------------------------------------------------------

How to play:

The game is designed for two players, each controlling a different pirate.

A, or J: Moves your pirate to the left.
D, or L: Moves your pirate to the right.
W, or I: Makes your pirate jump. Pirates need a bit of time to make their jump, so make sure you have enough time!
E, or O: Your standard slash attack. Be careful not to get your hands slashed off!
R, or P: Fire your pistol! Skilled pirates can shoot other bullets out of the air, but be careful, you only have three!
ESC: Quit the game.

--------------------------------------------------------------------

Directories:

include/ - contains all game headers
obj/ - contains the object files created during building
sounds/ - contains the sound files
sprites/ - contains the sprite files
src/ - contains all C++ files used except for game.cpp, which contains the main function.

--------------------------------------------------------------------
