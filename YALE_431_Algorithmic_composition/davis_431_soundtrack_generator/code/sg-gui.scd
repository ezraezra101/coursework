// A simple way of using the soundtrack generator.
// Can operate from a different address to the soundtrack generator.
(

// These are the basic messages we can send:
~vopts = ["play", "stop", "reset", "pan", "volume"];
~voiceOpts = ( ghostdist1: ~vopts, ghostdist2: ~vopts, beginning: ~vopts, melody: ~vopts, harmony: ~vopts, global: ["scale", "reverb", "max_voices", "tempo", "volume"]);


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
~voiceOpts.keys.asArray.sort.do { |key|
	var voice = StaticText(w).string = key.asString;
	var voiceActions = PopUpMenu(w).items = ~voiceOpts[key];
	var voiceActionOpt = TextField.new(w).string = 0;
	var voiceSend = Button(w).states_([["send"]]).action_({~sgNetAddr.sendMsg("/sg", key, voiceActions.item, voiceActionOpt.string)});
	//~voice = PopUpMenu(w).items = ~voiceOpts.keys.collect({|i| i.asString}).as(Array).sort;
	~rows.add(HLayout([voice, s:3], voiceActions,voiceActionOpt, voiceSend));
};
// Arbitrary message sending!
~msgLabel = StaticText.new(w).string = "Send message to /sg:";
~msgType = TextField.new(w).string = "global";
~msgKey = TextField.new(w).string = "reverb";
~msg = TextField.new(w).string = "1";
~sendMsg = {|field| ~sgNetAddr.sendMsg("/sg", ~msgType.string,~msgKey.string, ~msg.string)};
~msgType.action = ~sendMsg; ~msg.action = ~sendMsg; ~msgKey.action = ~sendMsg;
~rows.add(HLayout(~msgLabel, ~msgType,~msgKey, ~msg));


w.layout = VLayout(*~rows);

~updateNetwork.();
)
