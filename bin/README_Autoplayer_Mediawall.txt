
AUTOPLAYER - MEDIAWALL README
 
Link Mediawall aspect ratio currently is 3.555:1 (or two HD screens 1.777:1, side by side), 6 screens horizontal x 3 screens vertical, overall resolution 3840 x 1080, to demo on your own machine, use a smaller window size, maintaining the same aspect ratio as the mediawall  (example 1920 pixels wide x 540 pixels tall create as many media folders as you'd like, inside the data folder, each with one type of media ie videos, images or sounds
 
 
 key shortcuts:
 press <s> key - heads up info for application
 press <a> key - toggle on/off audio
 press <f> key - toggle between windowed and fullscreen view
 
 
 you can demo/test interactivity by creating triggered events, using the mouse input to simulate the camera tracking which occurs at the Mediawall, obviously multiple viewers can stand in front of the wall, each with their own position, thus triggering multiple simulaneous events, at different locations in front of the wall.

—————————————————————————————————————————————————————————————————— 
—————————————————————————————————————————————————————————————————— 

TimedEvent(s)
<TIMED_EVENTS>
	<EVENT> </EVENT>
		...multiple events...
	<EVENT> </EVENT>
</TIMED_EVENTS>
within <EVENT> tags
 
—————————————————————————————————————————————————————————————————— 
——————————————————————————————————————————————————————————————————
start/end, tags: <START></START>,<END></END>

in seconds, when event starts and stops (along time line, between start(0 sec) and end (specified seconds)), example: <START>10.0</START> <END>20.0</END>, or <START>start</START><END>end</END> event runs as long as application is running
 

——————————————————————————————————————————————————————————————————
location, tag: <LOC></LOC>

where visual media is played on screen contents: FULL(fullscreen), LHALF(left half), RHALF(right half), TILE(specific tile), RANDOMTILE(random tile in grid),
RANDOM(anything goes), example: <LOC>FULL</LOC> if <LOC>TILE</LOC> is specified, select exact tile with horizontal/vertical tile position, top left is position 1,1 
example: <LOC>TILE</LOC><TILE_H>1</TILE_H><TILE_V>1</TILE_V>


——————————————————————————————————————————————————————————————————
type, tag: <TYPE></TYPE>

IMAGE, VIDEO, SOUND, RANDOMIMAGE (picks random image from specified directory), RANDOMVIDEO, RANDOMSOUND, IMAGESEQUENCE (picks image in sequential order (alpha/numeric) from specified directory), VIDEOSEQUENCE, SOUNDSEQUENCE)


—————————————————————————————————————————————————————————————————— 
path, tag: <PATH></PATH>

relative to data folder, either to a particular file, or a directory -- for random/sequence media type, set path to folder with desired files


——————————————————————————————————————————————————————————————————  
alpha, tag: <A></A>

relavent to images and videos - level of opacity, from 0 (transparent) to 1 (opaque) or RANDOM (picks random alpha value from range specified in global variables, unless A_RND_MIN or A_RND_MAX are set in event), example: <A>0.5</A>


——————————————————————————————————————————————————————————————————  
alpha random range, tags: <A_RND_MIN></A_RND_MIN>,<A_RND_MAX></A_RND_MAX>

sets random range if <A>RANDOM</A> tag is specified, example: <A_RND_MIN>0.5</A_RND_MIN><A_RND_MAX>1.0</A_RND_MAX>, picks random value between 0.5 and 1
 

—————————————————————————————————————————————————————————————————— 
alpha_transition, tag: <A_TRANS></A_TRANS>

does image fade in/ out (TRUE or FALSE)


——————————————————————————————————————————————————————————————————  
alpha_transition_time, tag: <A_TIME></A_TIME>

seconds to fade in/out, example: <A_TIME>3.0</A_TIME>, fully fade in to specified alpha value in 3 seconds, begin fading out 3 seconds before end. do not specify larger value than 1/2 duration of event


—————————————————————————————————————————————————————————————————— 
volume, tag: <V></V>

relavent to audio from sounds and videos -  from 0 (silent) to 1 (full volume) or RANDOM (picks random volume from range specified in global variables, unless V_RND_MIN or V_RND_MAX are set in event), example: <V>0.5</V>


—————————————————————————————————————————————————————————————————— 
volume random range, tags: <V_RND_MIN></V_RND_MIN>,<V_RND_MAX></V_RND_MAX>

sets random range if <V>RANDOM</V> tag is specified, example: <V_RND_MIN>0.5</V_RND_MIN><V_RND_MAX>1.0</V_RND_MAX>, picks random value between 0.5 and 1


—————————————————————————————————————————————————————————————————— 
volume_transition, tag: <V_TRANS></V_TRANS>

does audio fade in/ out (TRUE or FALSE), example: <V_TRANS>TRUE</V_TRANS>


——————————————————————————————————————————————————————————————————  
volume_transition_time, tag: <V_TIME></V_TIME>

seconds for audio to fade in/out, example: <V_TIME>3.0</V_TIME>, fully fade in to specified volume in 3 seconds, begin fading out 3 seconds before end. do not specify larger value than 1/2 duration of event


—————————————————————————————————————————————————————————————————— 
loop, tag:  <LOOP></LOOP>

play video or sound once, or loop continuously, TRUE or FALSE

——————————————————————————————————————————————————————————————————  
speed, tag: <SPEED></SPEED> 

play speed of sound or video, 1.0 = normal play speed, 2.0 = 2x speed, 0.5 = 1/2 speed, or RANDOM (picks random speed from range specified in global variables, unless S_RND_MIN or S_RND_MAX are set in event)


—————————————————————————————————————————————————————————————————— 
speed random range, tags: <S_RND_MIN></S_RND_MIN>,<S_RND_MAX></S_RND_MAX>

sets random range if <SPEED>RANDOM</SPEED> tag is specified, example: <S_RND_MIN>0.5</S_RND_MIN><S_RND_MAX>1.0</S_RND_MAX>, picks random value between 0.5 and 1


——————————————————————————————————————————————————————————————————  
layer, tag: <LAYER></LAYER>

visual layering of images and video from 1 to 9, (1 front, 9 back)(1 is on top of 2, etc.)
 

—————————————————————————————————————————————————————————————————— 
—————————————————————————————————————————————————————————————————— 

Random Event(s)
	<RANDOM_EVENTS>
    		<EVENT> </EVENT>
        	...multiple events...
    		<EVENT> </EVENT>
 	</RANDOM_EVENTS>
within <EVENT> tags, see timed events for more complete listing of tags


—————————————————————————————————————————————————————————————————— 
—————————————————————————————————————————————————————————————————— 
chance, tag: <CHANCE></CHANCE>

chance that an event occurs, within a specified time period, evaluated every second, example: <EVENT><START>5</START><END>10</END><CHANCE>0.05</CHANCE></EVENT>, from 5 to 10 seconds, each second there is a 5% chance that a random event is triggered, and then plays for a specified duration.


—————————————————————————————————————————————————————————————————— 
duration, tag: <DURATION></DURATION>

once an event occurs, it plays for a specified duration in seconds, or RANDOM duration (picks random duration from range specified in global variables, unless D_RND_MIN or D_RND_MAX are set in event)


—————————————————————————————————————————————————————————————————— 
repeatable, tag: <REPEATABLE></REPEATABLE>

this event can be triggered more than once, TRUE or FALSE
 

—————————————————————————————————————————————————————————————————— 
—————————————————————————————————————————————————————————————————— 
Triggered Event(s) - either via mouse or camera
	<RANDOM_EVENTS>
    		<EVENT> </EVENT>
        	...multiple events...
    		<EVENT> </EVENT>
 	</RANDOM_EVENTS>
within <EVENT> tags, see timed or random events for more complete listing of tags

—————————————————————————————————————————————————————————————————— 
—————————————————————————————————————————————————————————————————— 
 
trigger, tag <TRIGGER></TRIGGER>

region which triggers the event, mouse position is specified as 0,0 (upper left corner of window) to 1,1 (lower right corner of window), as an x,y coordinate (horizontal,vertical), example 0.5,0.5, is the mouse in exactly the middle of the window. if the mouse falls within that region, it can either always trigger an event (<CHANCE>1</CHANCE>), or occasionally based upon a value specified. when camera is specified, the camera may track more than one person, so multiple events locations can be triggered at the same time.

the region is specified as follows, for example:
    
    <TRIGGER>
        <X>
            <MIN>0.5</MIN>
            <MAX>1</MAX>
        </X>
        <Y>
            <MIN>0</MIN>
            <MAX>1</MAX>
        </Y>
    <TRIGGER>

in this case we are specifying the right half of the window as a trigger

