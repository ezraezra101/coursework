# Real Time Soundtrack Generator Check in
Ezra Davis (solo project)

### How my project is going:
I've got the basic bones of my project layed out, and most of what I need to do now is to fill in my voices (which will use techniques I've used for previous assignments, so I won't be clueless). I also have relatively concrete plans for the other remaining tasks (controlling sound 'complexity', handling transitions, and the stretch goals).

My current 'voices' are *really* sad.

##### My promises
- [x] Write a SuperCollider program that creates different music when given OSC messages:
	- [x] reverb
	- [ ] scale(s)
	- [ ] tempo
	- [ ] Other section changes (e.g. play a bridge?)
	- [x] Voices
		- [x] on/off messages
		- [ ] panning, volume, etc...
		- [ ] handle both note events and long-running ambiance
	- [ ] Overall sound complexity (# of notes played at once)
	- [ ] Trigger short sound effects
	- [x] Basic quantization/synchronization

- [ ] Have interesting voices...
	- [ ] Hunt down a few `SynthDef`s
	- [ ] generate melodies with Markov chains
	- [ ] Use Automatata for section changes (and more)
	- [ ] Use envelopes for making gradual changes

- [x] Make a simple UI that controls the 1st program

- [ ] Stretch goals
	1. [ ] Voices interact
	2. [ ] Generate soundtrack to real game (Python Packman)
	3. [ ] Output MIDI (probably to Garageband)


### Changes
The project is still very similar to what I laid out originally.
Outputting MIDI seems more difficult than I'd originally thought (at least when keeping the global reverb), and I've thought that it would be a good idea to add more detail when performing section changes (e.g. playing a bridge), but there are no significant changes.

### Current (relatively minor) issues
I'm still not 100% certain what data I will need for each voice and how to get them to talk to each other.
My current thought is that each voice is simply an `EventStreamPlayer` which outputs to a buffer that is controlled globally. This would allow me to, for instance, only play the 3 most 'important' (recently triggered) voices.

The upside to this is it allows my voices to be reasonably flexible - All I need is a `.play`, `.pause` and `.reset` method from something that makes sound and it can become a voice.

The downside is that there isn't an easy way to force graceful section changes & synchronize with other voices.


### To run:
1. Run both `soundtrack_generator.scd` and `simple_gui.scd`
2. On the dropdown menus in the gui, select `v1` (or `v2`) and `play` and press `send`.
3. You should hear an incredibly boring pattern...

#### `simple_gui.scd`
```supercollider

(
if(w.notNil, {w.close});
w = Window.new.front;
w.name = "Real Time Soundtrack Generator GUI";
~rows = List[];


// Networking setup:
~ip = TextField(w);
~port = TextField(w);
~netlabel = StaticText.new(w).string = "IP and port: ";
~updateNetwork = { ~sgNetAddr = NetAddr.new(~ip.string, ~port.string.asInteger).postln; };
~ip.string = NetAddr.localAddr.ip;
~ip.action = ~updateNetwork;
~ip.align = \center;
~port.string = NetAddr.localAddr.port;
~port.action = ~updateNetwork;
~port.align = \center;
~rows.add(HLayout(~netlabel,~ip,~port));

// Limited message sending:
~voiceOpts = ( v1: ["play", "stop", "reset"], v2: ["play", "stop", "reset"], reverb: ["amp", "room"]);

~voice = PopUpMenu(w).items = ~voiceOpts.keys.collect({|i| i.asString}).as(Array).sort;
~voiceActions = PopUpMenu(w);
~voice.action = {|v| ~voiceActions.items = ~voiceOpts[v.item.asSymbol]};
~voiceActionOpt = TextField.new(w).string = 0;
~voiceSend = Button(w).states_([["send"]]).action_({~sgNetAddr.sendMsg("/sg", ~voice.item, ~voiceActions.item, ~voiceActionOpt.string)});
~rows.add(HLayout(~voice, ~voiceActions,~voiceActionOpt, ~voiceSend));
~voice.doAction;

// Arbitrary message sending!
~msgLabel = StaticText.new(w).string = "Send message to /sg:";
~msgType = TextField.new(w).string = "reverb";
~msgKey = TextField.new(w).string = "amp";
~msg = TextField.new(w).string = "1";
~sendMsg = {|field| ~sgNetAddr.sendMsg("/sg", ~msgType.string,~msgKey.string, ~msg.string)};
~msgType.action = ~sendMsg; ~msg.action = ~sendMsg; ~msgKey.action = ~sendMsg;
~rows.add(HLayout(~msgLabel, ~msgType,~msgKey, ~msg));


w.layout = VLayout(*~rows);

~updateNetwork.();
)


/*
b = Button.new(w,Rect(10,0,80,30)).states_([["Hide"],["Show"]]);
s = Slider.new(w,Rect(95,0,150,30));
c = CompositeView.new(w,Rect(20,35,100,60));
StaticText.new(c,Rect(0,0,80,30)).string_("Hello");
StaticText.new(c,Rect(20,30,80,30)).string_("World!");
b.action = { c.visible = b.value.asBoolean.not };
s.action = { c.bounds = Rect( s.value * 150 + 20, 35, 100, 100 ) };
w.front;

thisProcess.openUDPPort(1121)

Speech.channels[0].speak("Force the voice to speaking something different.");
Speech.channels[0].speak("Force the voice to speaking something different.".reverse, true);
Speech.setSpeechPitch(0, 40); //pitch in MIDI Num
*/
```

#### `simple_gui.scd`
```supercollider
// To run:
// Run the 'Setup' section
// Open and run `simple_gui.scd`


(
///////// Setup //////////////

~cleanup = {
	var free = {|a| if(a.notNil, {a.free})};
	free.(~sources);
	free.(~effects);
	free.(~bus);
	free.(~oscFunc);
};

~cleanup.(); // Remove pesky duplicates if this isn't the first time going through here.

s.boot;

~sources = Group.new;
~effects = Group.after(~sources);     // make sure it's after
~bus = Bus.audio(s, 2);         // this will be our stereo effects bus


SynthDef('reverb', {|amp = 1, mix = 0.25, room = 0.15, damp = 0.5, in = 0, out=0|
	Out.ar(out, amp * FreeVerb.ar(In.ar(in, 2),mix,room,damp));
}).add;
~reverb = Synth(\reverb, [\in, ~bus], ~effects);


// Each voice implements oscFunc.
~voices = Dictionary.new();
~voices.put(\reverb, (
	oscFunc: {|msg| ~reverb.set(msg[0], msg[1].asFloat); }, // Just control reverb's attributes directly.
));

~addVoice = { |id, player|
	var self = (player: player);
	~voices.put(id,  self);
	self.lastPlayed = 0;
	self.oscFunc  = { | msg |
		switch(msg[0],
			\reset, {self.player.reset},
			\play, {self.player.play(quant:1)},
			\stop, {self.player.pause},
			\pan, {},
			\volume, {},
		);
	};
};

~oscFunc = OSCFunc({ |msg| var v=~voices[msg.removeAt(1)];msg.removeAt(0);if(v.notNil, { v[\oscFunc].(msg) })}, "/sg");


// Let's create an example voice:
SynthDef(\example_synth, { |freq=440, bus=0, dur=2|
	Out.ar(bus, (EnvGen.ar(Env.perc(releaseTime: dur), doneAction: 2) * SinOsc.ar(freq) ) ! 2);
}).add;
(instrument: \example_synth, bus: ~bus, group: ~sources).play;

x = Pbind(\instrument, \example_synth, \degree, Pseq([1,2,3,4], inf), \bus, ~bus, \group, ~sources).play.reset.stop;
~addVoice.(\v1, x);

x = Pbind(\instrument, \example_synth, \degree, Pseq([1,2,3,4], inf), \bus, ~bus, \group, ~sources).play.reset.stop;
~addVoice.(\v2, x);
)

// Cleanup:
~cleanup.();


//////// Example noise: //////////
SynthDef(\example_synth, { |bus=0, dur=2|
	Out.ar(bus, (EnvGen.ar(Env.perc(releaseTime: dur), doneAction: 2) * SinOsc.ar(440) ) ! 2);
}).add;
Synth(\example_synth, [\bus, ~bus], ~sources)
(instrument: \example_synth, bus: ~bus, group: ~sources).play

```

