// Let's set up a CFG for harmonizing:
// This is Martin Rohrmeier's CFG
// From: Martin Rohrmeier: Towards a generative syntax of tonal harmony, http://musicweb.ucsd.edu/~sdubnov/Mu270d/Harmony/Rohrmeier2011.pdf
// It's been slightly simplified - We're only using the major key version
(
var dictionary = (
	phrase: {[\tr, \phrase]}, // Keep going once we reach the end of phrase
	tr: { [[\tr,\tr], [\tr, \dr], [\dr, \t], [\t]].wchoose([1,1,1,1].normalizeSum) },
	dr: { [[\dr, \dr], [\sr, \d]].wchoose([0.33, 0.66]) },
	sr: { [[\sr, \sr], [\s]].wchoose([0.33,0.66]) }, // Want to discourage (boring) infinite loops
	// Chord level:
	t: { [[1, 3, 6].choose]},
	d: { [[5,7].choose]},
	s: { [[2, 4].choose] },
);
var martin_harmonizer_routine = { Routine({
	var cfgstring = [\phrase];
	{
	var key = cfgstring.removeAt(0);
	if(key.isInteger, {
			(key).yield;
	}, {
		cfgstring = dictionary[key].() ++ cfgstring;
	});
}.loop});
};

martin_harmonizer_routine
)

// Usage:
// Pbind(\degree, martin_harmonizer_routine.() * [0,1] /*+original degree generator*/, \dur, Pseq([0.25], inf)).play;
