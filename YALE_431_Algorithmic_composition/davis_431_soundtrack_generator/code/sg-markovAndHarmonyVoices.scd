// Creates two voices that talk to each other for use by sg-main.
// Harmony teaches melody's markov chain the degrees returned by `martin_harmonizer` as it is played.
// Duration is a 'random' 4 beat measure (or 2 beat, for harmony)
(
var martin_harmonizer_routine = "martin_harmonizer.scd".loadRelative[0];

var markov = MarkovSet([[0,[0]]]);
var harmony_stream = martin_harmonizer_routine.();
var markov_last_note = 0;
//~chords = [[0,2,5, \rest]];//[[0,2,4], [0,2,5], [0,2,6], [0,2,3], [0,1,4], [-5,2,5], [1,5,7]];
var dur_patterns = [1!4, 1!3 ++ (0.5!2), [1,1] ++ (0.5!4), 2!2, [2,1,1,0.5,0.5], 0.25!2 ++ (0.5!3) ++ (1!2), 0.25!14 ++ (0.5!2)];


var melody_r = Routine({
	var mstream = markov.makeSeeds.asStream;
	{   markov_last_note = mstream.next(markov_last_note);  markov_last_note.yield;
}.loop});
var harmony_r = Routine({ {
	var h_next = harmony_stream.next;
	markov.read(markov_last_note, h_next);
	(h_next+markov_last_note).yield;
}.loop});

var dur_r = p({{ dur_patterns.choose.scramble.do { |dur| dur.yield} }.loop});

var synthdef = SynthDef(\sin_strong, { |bus=0, pan=0, dur=2, freq=440, amp=1|
	Out.ar(bus,
		Pan2.ar(
			amp * 0.15
			* EnvGen.ar(Env.new([0,1,1,0], [0.05*dur,0.7*dur, 0.05*dur,0], \sqr), doneAction: 2)
			*SinOsc.ar(freq)
		, pan));
}).add;

////// Return value:
// Oddly, I made the harmony go twice as fast as the 'martin_harmonizer' voice.
var exports = [ // Returns [melody, harmony] functions
	{
	arg getScale, getPan, getAmp, getBus, getGroup;
		Pbind(
			\instrument, \sin_strong,
			\degree, melody_r,
			\dur, dur_r,
			\pan, getPan, \amp, getAmp, \bus, getBus,\scale, getScale, \group, getGroup,
		).play.pause;},
	{
	arg getScale, getPan, getAmp, getBus, getGroup;
		Pbind(
			\instrument, \sin_strong,
			\degree, harmony_r,
			\dur, (0.5 * dur_r),
			\scale, getScale, \pan, getPan, \amp, getAmp, \bus, getBus, \group, getGroup,
		).play.pause;},
];

/* // Uncomment this to play standalone:

//var getPan = 0; //var getAmp = 1; var getBus = 0; var scale = scale.at('major').degrees;
Pbind(
\instrument, \sin_strong,
\degree, melody_r,
\dur, dur_r,
	\pan, 0, \scale, Scale.at('major').degrees, \amp, 1,// \bus, getBus, \group, {getGroup.()}, \scale, getScale,
).play;
Pbind(
\instrument, \sin_strong,
\degree, harmony_r,
\dur, (0.5 * dur_r),
	\scale, Scale.at('major').degrees, \amp, 1,  \pan,1,
//\scale, getScale, \pan, getPan, \amp, getAmp,// \bus, getBus, \group, {getGroup.().postln},
).play;
*/
exports
)