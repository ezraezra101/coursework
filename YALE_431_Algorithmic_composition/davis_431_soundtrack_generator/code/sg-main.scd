// Runs the soundtrack generator, populates it with voices, and launches the simple GUI.

(

var setup_voices = {

	var m_and_h = "sg-markovAndHarmonyVoices.scd".loadRelative[0];
	~addVoice.(\melody, m_and_h[0], quant: 4);
	~addVoice.(\harmony, m_and_h[1], quant: 4);
};

// Sets up voices for Pacman game
var setup_sound_effects = {

	var synth_sound_effect = SynthDef("sound-effect",{ |bus=0, pan=0, amp=1, bufnum|
		Out.ar( bus,amp * Pan2.ar(PlayBuf.ar(1, bufnum, BufRateScale.kr(bufnum), doneAction:2),pan))
	}).add;

	var pv1 = {
		arg getScale, getPan, getAmp, getBus, getGroup;
		Pbind(
			\instrument, \default,
			\midinote, 60+Pseq([0,1,-1], inf),
			\dur, Pseq([0.5,0.5,1],inf),
			\pan, getPan, \amp, getAmp, \bus, getBus,\scale, getScale, \group, getGroup,
		).play.pause;
	};
	var pv2 = {
		arg getScale, getPan, getAmp, getBus, getGroup;
		Pbind(
			\instrument, \default,
			\midinote, -6+60+Pseq([0,1,-1], inf),
			\dur, Pseq([0.5,0.5,0.5,0.5],inf),
			\pan, getPan, \amp, getAmp, \bus, getBus,\scale, getScale, \group, getGroup,
		).play.pause;
	};

	~addVoice.(\ghostdist1, pv1);
	~addVoice.(\ghostdist2, pv2);

	['beginning', 'death', 'extrapac', 'intermission', 'eatfruit', 'eatghost'].do { |name|
		var buf = Buffer.read(s, thisProcess.nowExecutingPath.dirname ++ "/sounds/pacman_"++name++".wav");
		var func = { |getScale, getPan, getAmp, getBus, getGroup|
			Pbind(\instrument, "sound-effect", \bufnum, Pseq([buf.bufnum]),
				\dur, Pseq([1]),
				\squirrel, p({~voices.reject({|v| v.player.isPlaying.not;}).postln; 1.yield}),
				\pan, getPan, \amp, getAmp, \bus, getBus, \group, getGroup,).asEventStreamPlayer.pause;
		};
		~addVoice.(name.asSymbol, func, true, 0).amp = 0.3;
	};
};


~run = {
	// Setup
	"sg-setup.scd".loadRelative;
	//~martin_harmonizer_routine

	"sg-gui.scd".loadRelative; // Shouldn't be run in same instance, but oh well...

	setup_voices.();
	setup_sound_effects.();

};
~run.();
// ~cleanup.();
)

