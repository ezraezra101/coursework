# Real Time Soundtrack Generator
Ezra Davis (solo project)

##### Running
Open and run the contents of `code/sg-main.scd`. This will launch the soundtrack generator and my simple UI.

```bash
# Run pacman with python 2:
# Install OSC package:
pip install pyosc # (Must be pip for python 2)
# Run pacman:
cd pacman
python pacman.py -p KeyboardAgent # (Also must be Python 2)
```
##### Sources
- The Python pacman game (except for roughly the first 60 lines in `KeyboardAgents.py`) comes from [a Berkeley AI assignment](https://inst.eecs.berkeley.edu/~cs188/fa07/projects/multiagent/multiagentproject.html)
- The `.wav` files come from the original Pacman game via [www.classicgaming.cc](http://www.classicgaming.cc/classics/pac-man/sounds)
- The Context Free Grammar design for tonal harmonies comes from Martin Rohrmeier's paper [Towards a generative syntax of tonal harmony](http://musicweb.ucsd.edu/~sdubnov/Mu270d/Harmony/Rohrmeier2011.pdf)

##### My promises from the proposal
- [x] Write a SuperCollider program that creates different music when given OSC messages:
	- [x] reverb
	- [x] scale(s)
	- [x] tempo
	- [ ] *implied promise*: Complex section changes (e.g. play a bridge?)
	- [x] Voices
		- [x] On/off messages
		- [x] Panning, volume, etc...
		- [ ] Handle long-running ambiance synths
	- [x] Overall sound complexity (# of notes played at once)
	- [x] Trigger short sound effects
	- [x] Basic quantization/synchronization

- [x] Have interesting voices...
	- [x] Make a few `SynthDef`s
	- [x] generate melodies with Markov chains
	- [x] Use Automatata for section changes (and more)
	- [ ] Use envelopes for making gradual changes

- [x] Make a simple UI that controls the 1st program

- [x] Stretch goals
	1. [x] Voices interact
	2. [x] Generate soundtrack to real game (Python Packman)
	3. [ ] Output MIDI (probably to Garageband)
