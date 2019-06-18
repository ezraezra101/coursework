// File that creates and sets up the soundtrack generator / audio engine. Just add voices!


~cleanup = {
	var free = {|a| if(a.notNil, {a.free})};
	free.(~sources);
	free.(~effects);
	free.(~bus);
	free.(~oscFunc);
	~voices.do({|v| v.player.stop;});
	"End cleanup".postln;
};

~cleanup.(); // Remove pesky duplicates if this isn't the first time going through here.

s.boot;

~sources = Group.new;
~effects = Group.after(~sources);     // make sure it's after
~bus = Bus.audio(s, 2);         // this will be our stereo effects bus


SynthDef('reverb', {|amp = 1, mix = 0.25, room = 0.15, damp = 0.5, in = 0, out=0|
	Out.ar(out, amp * FreeVerb.ar(In.ar(in, 2),mix,room,damp));
}).add;
~reverb = Synth('reverb', [\in, ~bus], ~effects);


~max_voices = inf;
~tempo = TempoClock(1,4);
~voices = Dictionary();
~scale = Scale.at('major').degrees;


~oscFunc = OSCFunc({ |msg|
	var target = msg[1];
	var type = msg[2];
	var argument = msg[3];
	msg.postln;
	if(target == \global, { // Sending message to control reverb synth or 'global' variables
		switch(type,
			\tempo, { ~tempo.tempo = argument.asFloat},
			\scale, {if(Scale.at(argument).isNil.not, { "Scale changed".postln; ~scale = Scale.at(argument).degrees;});},
			\section_change, {},
			\max_voices, {~max_voices = argument.asInt;~enforceMaxVoices.();}, // Set 'sound complexity'
			\reverb, {~reverb.set(\room, argument.asFloat);},
			\volume, { ~reverb.set(\amp, argument.asFloat);},
		);
	}, { // Else it's a voice:
		var voice =  ~voices[target];
		if(voice.isNil.not, {
			switch(type,
				\reset, {voice.player.reset},
				\play,  {if(voice.do_not_wake, {voice.player.reset}); voice.player.play(~tempo, quant: voice.quant); voice.lastPlayed = ~tempo.beats; ~enforceMaxVoices.()},
				\stop, {voice.player.stop; voice.lastPlayed = -1;                    ~enforceMaxVoices.()},
				\pan, { voice.pan = argument.asFloat;},
				\volume, { voice.amp = argument.asFloat;},
			);
		}, {"Voice not found".postln;});
	});

}, "/sg");

// Play only the ~max_voices most recently played voices
~enforceMaxVoices = {
	// Find all the playing voices and sort them by how recently they've been played (recent to oldest)
	var playing = ~voices.values.reject({ |v| (v.player.isPlaying).not }).sort({ |a,b| a.lastPlayed > b.lastPlayed});
	var not_playing = ~voices.values.reject({ |v| v.player.isPlaying }).sort({ |a,b| a.lastPlayed > b.lastPlayed});
	while({ (playing.size > ~max_voices) && (playing.size > 0)}, {
		playing.pop.player.pause;
	});
	while({(playing.size < ~max_voices) && if(not_playing.size == 0, false, {not_playing[0].lastPlayed > 0 }) }, {
		var p0 = not_playing[0];
		not_playing.removeAt(0);
		if(p0.do_not_wake.not, {
			playing = playing ++ [p0];
			p0.player.play(~tempo, quant: p0.quant);
		});
	});
};

// Every half second, run enforceMaxVoices... a little kludgy, but it catches when voices end on their own.
Routine({ {0.5.yield; ~enforceMaxVoices.()}.loop}).play(~tempo, quant: 1);

(
// Takes in a unique \id (for the voice to be identified by), a function that takes in patterns that get information about the voice,
//    whether to wake the voice from pausing it, and a quant.
~addVoice = { |id, func, do_not_wake=false, quant=1|
	var self = ();
	var p2 = {|func| p({{func.().yield}.loop})}; // We want the following functions as routines... sigh...
	var getScale = { self.scale ? topEnvironment[\scale] ? Scale.at('major').degrees; };
	var getPan = {self.pan ? topEnvironment[\pan] ? 0 };
	var getAmp = {self.amp ? 1; };
	var getBus = {self.bus ? topEnvironment[\bus] ? 0};
	var getGroup = {(self.group ? topEnvironment[\sources]) ? topEnvironment[\group]};
	~voices.put(id,  self);
	self.lastPlayed = 0;
	self.amp = 1;
	self.do_not_wake = do_not_wake;
	self.quant = quant;
	self.player = func.(getScale: p2.(getScale), getPan: p2.(getPan), getAmp: p2.(getAmp), getBus: p2.(getBus), getGroup: p2.(getGroup));
};

)