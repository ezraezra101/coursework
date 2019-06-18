# You can place the script of your game in this file.
init:
# Declare images below this line, using the image statement.
# eg. image eileen happy = "eileen_happy.png"
    $ renpy.music.register_channel("record", "sfx", True)
    image gray = "#888888"
    image white = "#FFFFFF"
    #image bg ballroom_past = "assets/ballroom_past.png"
    #image bg ballroom_present = "assets/ballroom_present.png"

# Snippet borrowed for fast importing of pictures:
init -3 python:
  import os
  sounds = {} # Setting up the sounds dictionary
  for fname in os.listdir(config.gamedir + '/assets'):
    if fname.endswith(('.jpg', '.png')):
      tag = fname[:-4]
      fname =  'assets/' + fname
      renpy.image(tag, fname)
    elif fname.endswith(('.mp3','.wav','.ogg')):
        tag = fname[:-4]
        fname = 'assets/' + fname
        sounds[tag] = fname

    # I don't know why this was breaking
    if '02_Ghostly_Music_Box' in sounds:
        sounds['menu_music'] = sounds['02_Ghostly_Music_Box']
    

    hallway_present_imagemap_image = "assets/hallway_present.png"
    hallway_present_imagemap_highlighted_image = "assets/hallway_present_highlight.png"

# Declare characters used by this game.
define mc = Character("Main Character", color=main_char_name_color)

define t = Character('Taylor', color="#FFFFFF")

define o = Character("Olivia", color="#FF0000")

define h = Character("Henry", color="#00FF00")

define f = Character("Faith", color="#0000FF")
define hope = Character("Hope", color="#0000FF")
define m = Character("Milo O'Brien", color= "#FF8800")


# The game starts here.
label start:
    stop music

    call initialize

    call taylor_intro
    return
    
label initialize:
    # Initializing variables:
    $ intro_count = 0
    $ olivia_intro_val = False
    $ milo_intro_val = False
    $ twins_intro_val = False
    $ taylor_intro_val = False
    
    $ death_count = 0
    $ death_kitchen = False
    $ death_hallway = False
    $ death_ballroom = False
    $ death_upstairs = False
    $ death_newspaper = False
    $ death_outside = False

    $ menu_box = "assets/au_box.png"

    $ dev = True # Developer options
    
    scene black with dissolve

    $ name = renpy.input("What is your name?", "Your name", length=12)
    python:
        if name== "Your name" or name=="":
            name = "John"
    
    $ mc = Character(name, color=main_char_name_color)
    
    return


label taylor_intro:
    
    
    scene ballroom_past
    show taylor
    with dissolve
    
    t "%(name)s, this party is awesome."
    mc "Thanks."
    t "How come you've never invited me to your mansion before?"
    mc "I guess it just never came up, we always hang out at your house."
    t "You mean apartment, right?"
    mc "Of course."
    t "Just you wait, after my family pays off our debt, we'll get a real house."
    mc "I bet you will."
    t "Fine, fine. Let's go talk to some other people."
    hide taylor with easeoutleft
    call intro_choices
    return
    
label intro_choices:
    # I'm not sure this is the best way to lay this out
    menu:
        "Who do you want to talk to?"
        "Olivia & Henry" if olivia_intro_val==False:
            call olivia_intro
        "Milo" if milo_intro_val==False:
            call milo_intro
        "Hope & Faith" if twins_intro_val==False:
            call twins_intro
        "Talk to Taylor more" if taylor_intro_val==False:
            call taylor_intro_2
        #"SKIP" if dev==True:
        #    $ intro_count = 4

    python:
        if intro_count < 4:
            renpy.jump("intro_choices")
        else:
            renpy.call("spill_drink")

    return

label olivia_intro:
    show olivia_henry with easeinright
    
    $ olivia_intro_val = True
    $ intro_count += 1
    o "Hey %(name)s, how is the middle sibling doing?"
    mc "I'm fine, how about you?"
    o "I'm fantastic.\nHey, I even brought our nephew Henry."
    mc "Hey Henry!"
    h "Play with me sometime kay?"
    mc "I will."
    o "Henry, do you want some punch?"
    h "Yes!"
    hide olivia_henry with easeoutleft
    return

label milo_intro:
    show milo with easeinright
    $ milo_intro_val = True
    $ intro_count += 1
    mc "Professor Milo O'Brien, how are you doing these days?"
    m "I'm doing well, and yourself?"
    mc "I'm doing just fine."
    m "Splendid. Let me get you a drink and we can chat for a bit..."
    hide milo with easeoutright
    return

label twins_intro:
    show faith_hope with easeinleft
    $ twins_intro_val = True
    $ intro_count += 1
    mc "Hey Hope, Faith. How did you like the hamster I gave you for your birthday?"
    hope "He was really cute. Too bad he died."
    f "It turns out hamsters can't breath underwater."
    hope "Oops."
    f "hehehehehehehe"
    mc "Maybe I should stop giving you both pets as presents."
    hide faith_hope with easeoutleft
    return

label taylor_intro_2:
    show taylor with easeinleft
    $ taylor_intro_val = True
    $ intro_count += 1
    
    t "I can't believe that you already wrote your will!"
    mc "They said it was a good idea... with my parents gone and all."
    t "But seriously, a will?\nIt’s not like you’re gonna die soon or whatever."
    t "Or is there something your not telling us?"
    mc "Haha, very funny."
    
    hide taylor with easeoutleft
    return

label spill_drink:
    play sound sounds['spill']
    mc "Whoops, darn it. Now my best shirt is ruined."
    mc "I'd better go and change. I'll be right back."
    scene hallway_past with fade
    mc "There, now that's better."
    play sound sounds['footsteps']
    mc "..."
    stop sound
    mc "Who's there?"
    mc "Oh, it's just you. Jeez, you scared me for a bit."
    mc "Wait, what are you doing?"
    mc "Stop! No stop!"
    mc "NO!"
    
    $ renpy.play(sounds['punch'])
    scene black with Dissolve(0.5)
    with Shake((0, 0, 0, 0), 0.25, dist=20)
    #Fades to black 

    call death_monologue
    return

label death_monologue:
    with Pause(0.5)
    scene ballroom_present with Dissolve(1.0)
    mc "Wha...\nWhat happened?"
    mc "Where am I?"
    mc "What's going on... my head..."
    show ghost_hands with easeinbottom
    mc "What the hell?\nWhat happened to my hands!?"
    mc "What happened to me? Why am I see-through?" with Pause(0.1)
    mc "What's this? A newspaper?"
    with None
    show newspaper
    show text ("{color=#000} {size=50}" + name + "{/color}{/size}") at Position(xpos=553,ypos= 193, xanchor=0.5, yanchor = 0.5)
    with Dissolve(0.3)
    hide ghost_hands
    mc "This newspaper... says that I was killed?"
    mc "How is that even possible, I'm right here... only transparent..."
    mc "But how did I die? Who killed me?"
    mc "..."
    mc "I can't remember."
    mc "Maybe looking around will jog my memory."
    call death_choices
    return

label death_choices:
    $ death_count += 1
    hide newspaper
    hide text
    with dissolve
    menu:
        mc "Hmm, where should I go?"    
        "Visit the kitchen" if death_kitchen == False:
            call kitchen
        "Look around the hallway" if death_hallway == False:
            call hallway
        "Stay in the ballroom" if death_ballroom == False:
            call ballroom
        "Read the newspaper more" if death_count <= 1 and death_newspaper == False:
            call newspaper
        "Go outside" if death_outside == False:
            call outside
    
    python:
        if death_count < 2:
            renpy.jump("death_choices")
        else:
            renpy.call("footsteps")
        
    return

label kitchen:
    $ death_kitchen = True
    scene kitchen_present with dissolve
    "Why is the kitchen a mess?"
    mc "Silverware everywhere...\n{w=0.2}Forks... {w=0.4} Spoons... {w=0.4} Knives?"
    scene kitchen_past
    show faith_hope
    call show_flashback
    hope "Shiny objects... hehehehehe"
    f "Spoons... not fun."
    hope "Forks... a bit more fun."
    f "Knives... perfect.\nhehehehehe"
    scene kitchen_present
    call hide_flashback
    mc "Even looking back into the past, that still creeps me out..."
    mc "I'd better keep moving."
    return

label hallway:
    $ death_hallway = True
    #scene hallway_present with dissolve
    scene hallway_past
    show olivia_taylor
    call show_flashback
    o "Our father hadn't written his will yet, so everything went to my oldest sibling."
    t "So?"
    o "He didn't want it! So it all went to %(name)s!"
    t "Yeah... your point?"
    o "I wanted something too."
    t "You do know that %(name)s said that you could live here too, right?"
    hide olivia
    hide taylor
    scene hallway_present
    call hide_flashback
    "..."
    mc "Yes. Yes I did."
    mc "I'd better move on..."
    scene ballroom_present with dissolve
    return

label ballroom:
    $ death_ballroom = True
    scene ballroom_present with dissolve
    mc "There's Police tape everywhere..."
    mc "They didn't even clean up the party food"
    mc "Or the drinks..."
    scene ballroom_past
    show milo
    call show_flashback
    
    m "I assume you are still keeping up your studies..."
    "..."
    m "Excellent."
    "..."
    m "How about another toast to celebrate your bright future?"
    scene ballroom_present
    call hide_flashback
    mc "I forgot. I lied to Milo..." #change?
    mc "I'd better move on."
    return

label newspaper:
    $ death_newspaper = True
    show newspaper
    show text ("{color=#000} {size=50}" + name + "{/color}{/size}") at Position(xpos=553,ypos= 193, xanchor=0.5, yanchor = 0.5)
    with dissolve
    mc "Since I'm here, I might as well read the whole article."
    mc "Let's see... %(name)s... killed Halloween Night, no culprit arrested."
    mc "...will to be executed in 2 weeks time ... will?"
    scene ballroom_past
    call show_flashback
    mc "Wha? My head... hurts..."
    show taylor with dissolve
    t "Hey %(name)s, I finally got that job I've been applying for..."
    "..."
    t "Now maybe I can pay off some of my parents' debt..."
    "..."
    t "No thanks, I don't need your help.\nI should do this myself, though it would be nice..."
    scene ballroom_present
    show newspaper
    show text ("{color=#000} {size=50}" + name + "{/color}{/size}") at Position(xpos=553,ypos= 193, xanchor=0.5, yanchor = 0.5)
    
    call hide_flashback
    mc "What was that?\nOne of my memories?"
    mc "Must have been."
    mc "Poor Taylor, I wish I could go and see..."
    mc "I should probably move on..."
    return
    
label outside:
    $ death_outside = True
    mc "I don't even know how much time has passed."
    mc "Maybe if I go outside I will figure it out."
    "..."
    mc "I can't grab the door handle?!?"
    mc "Oh, right. I'm a ghost... I think."
    mc "Maybe I can just walk through the do...\nOuch!"
    mc "So that's not going to work..."
    scene ballroom_past
    show olivia_henry
    call show_flashback
    o "Henry, don't run outside yet."
    h "Why?"
    o "Because we should play inside.\nListen to Aunt Olivia."
    h "Okay! What should we play?"
    o "You know what's fun?"
    h "What?"
    o "Football. So go tackle Taylor to the ground, like a football player!"
    h "Okay!"
    scene ballroom_present
    call hide_flashback
    "..."
    mc "Henry couldn't leave, and neither can I."
    mc "I'd better keep moving, I guess."
    return




label footsteps:
    #scene ballroom_present # change later
    play sound sounds["footsteps"]
    mc "Wait… what’s that sound?"
    mc "Could it be . . . \n{w=0.4}Footsteps?"
    mc "Who's there?"
    mc "Should I go see?"
    mc "Wait...\nWhat if they see me?"
    mc "Can they see me?"
    mc "I'd better not chance it..."
    mc "I'd better hide."
    "..."

    stop sound
    call death_choices_2
    return

label death_choices_2:
    scene hallway_present with fade
    mc "Phew."
    mc "I didn't run into them"
    "..."
    mc "What is all of this stuff here?"
    mc "I know it wasn't this messy during the party..."
    mc "I don't know what to look at first..."
    #pause 0.5
    #"What is all this stuff doing here!?!"
    #"I know it wasn’t this messy when I wasn’t … well… \ndead."
    #"Where did it all come from?"
    #"Where do I start?"
    
    $ hotspots = [(225, 480, 560, 600, "broom"), (632, 411, 757, 500, "stain"), (472, 343, 600, 423, "bunny")]
    $ result = renpy.imagemap(hallway_present_imagemap_image, hallway_present_imagemap_highlighted_image, hotspots)

    if result == "stain":
        call stain_investigate
    elif result == "broom":
        call broom_investigate
    elif result == "bunny":
        call bunny_investigate
    
    call second_footsteps
    
    return
    
label bunny_investigate:
    $ last_investigate = "bunny"
    mc "This must be Henry's"
    mc "Definitely..."
    #call show_flashback
    scene hallway_past
    show olivia_henry
    
    call show_flashback
    h "Auntie Olivia, why are we waiting upstairs?"
    o "Don't you want to give %(name)s a big surprise?"
    h "Yeah!"
    #call hide_flashback
    scene hallway_present
    call hide_flashback
    "..."
    mc "I don't even remember what the surprise was..."
    mc "How sad."
    return

label stain_investigate:
    $ last_investigate = "stain"
    "Is that... blood?"
    scene hallway_past
    show faith_hope
    call show_flashback
    hope "Oh no, it died."
    f "Poor gerbil..."
    hope "I guess gerbils don't like to play..."
    f "Especially not with knives..."
    hope "hehehehehe"
    #"A twin cut themself while playing with knives in the kitchen"
    scene hallway_present
    call hide_flashback
    return
    
label broom_investigate:
    $ last_investigate = "broom"
    mc "Why is this broken in half?"
    scene hallway_past
    show olivia
    call show_flashback
    o "grumble... grumble"
    o "Didn't inherit anything..."
    o "Why am I cleaning like a maid, huh?!?"
    o "I should be telling a maid to clean this, not doing it myself!"
    #"Olivia cleaning up broken glass with the broom"
    scene hallway_present
    call hide_flashback
    mc "Olivia always had a big ego."
    return

label second_footsteps:
    play sound sounds['footsteps']
    pause 0.3
    mc "Footsteps...\nAgain?"
    mc "Who's there?"
    mc "Why are they here?"
    mc "What are they looking for?"
    mc "Who is even here?"
    menu:
        mc "Should I try to find out, or try to hide?"
        "Try to find out: Run upstairs into the bedroom":
            call endings
        "Try to hide: Run into the library":
            call library

    return

label library:
    scene library_present with dissolve
    pause 0.5
    mc "Running was a good idea."
    mc "Besides, I haven't checked the library yet."
    mc "Maybe something here will trigger my memory."
    "..."
    mc "What's this?"
    "..."
    mc "A really old newspaper; why did I keep this?"
    show newspaper_splash with fade
    mc "...wait."
    
    mc "Milo Green disappeared... convicted of murder..."
    mc "Wait..."
    
    call milo_ending
    return
    

label endings:
    python:
        if twins_intro_val == True and death_kitchen == True and last_investigate == "stain":
            renpy.call("twins_ending")
        elif olivia_intro_val == True and death_outside == True and last_investigate == "bunny":
            renpy.call("henry_ending")
        elif olivia_intro_val == True and death_hallway == True and last_investigate == "broom":
            renpy.call("olivia_ending")
        elif True:
            renpy.call("taylor_ending")
            
    return

label henry_ending:
    play sound sounds['door_creak']
    scene ending_3 with dissolve
    h "sniff... Sniff"
    o "You were a very good boy."
    h "But %(name)s stopped playing! They hate me now... sniff..."
    o "I'm sure %(name)s was very happy when you wanted to play with him."
    h "You sure? You really mean it?"
    o "I'm sure. I mean, who doesn't love being pushed down the stairs?"
    #scene ending_3
    call the_end
    return

label olivia_ending:
    scene bedroom_present
    show olivia
    with dissolve
    o "And this is where the dresser will go."
    o "And this is where my mirror will go."
    o "And let's not forget my bed!"
    scene ending_1 with dissolve
    o "I'll prbably need a new broom though; this one has seen enough action for a lifetime."
    
    scene black with fade
    call the_end
    return

label milo_ending:

    mc "Now I remember... Mr. O'Brien wasn't really Mr. O'Brien."

    scene ballroom_past
    show milo
    call show_flashback
    m "You look awfully thirsty, let me get you something to drink..."
    m "Here, have another."
    #show milo with vial
    scene bedroom_past
    show milo
    show vignette
    with fade
    
    m "Oh you don't look so good. Here, let me help you."
    m "..."
    scene ending_5
    show vignette
    with dissolve
    m "You knew about my secret. Goodbye."
    
    call the_end

    
    
    pause
    return

label twins_ending:
    play sound sounds['door_creak']
    scene bedroom_present
    show faith_ending
    with dissolve
    f "hehehehehe"
    f "Don't worry sister; I'll take care of you."
    f "Everything will be okay."
    f "It's too bad %(name)s already left, but I'm still here.\nI guess %(name)s didn't like these kind of games either."
    scene ending_4 with dissolve
    f "Let's have some fun."

    call the_end
    

    #pause 0.5
    #scene white with fade
    #pause
    return

label taylor_ending:
    scene ending_2 with dissolve
    play sound sounds['door_creak']
    t "Can't miss any glass, this window won't clean itself."
    t "..."
    t "Sorry %(name)s, but you shouldn't have put me in your will."
    t "You knew about my parents being up to their heads in debt."
    t "This was the only way, you knew that."
    call the_end
    return

label the_end:
    scene black
    call hide_flashback
    mc "Now... now I remember..."
    pause 0.5
    scene white with fade
    pause 1.0
    scene intro with fade
    pause 0.1
    return


label show_flashback:
    play record sounds['record_hiss'] fadein 1.0
    show vignette
    with fade
    return

label hide_flashback:
    stop record fadeout 1.0
    hide vignette
    with fade
    return
    
label splashscreen:
    scene short_circuit_games with fade
    pause 0.7
    show short_circuit_stripe
    with Shake((0, 0, 0, 0), 0.25, dist=5)
    pause 0.2
    scene short_circuit_off
    show short_circuit_stripe
    with dissolve
    pause 0.6
    scene Murdered with fade
    pause 2.0
    scene intro with fade
    return
    