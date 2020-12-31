# AlsaTonic

AlsaTonic is a sine wave sound generator from Alsa Audio Api.
Inspired by AlsaGenTone project.
[alsagentone](https://github.com/sourcegod/alsagentone.git "AlsaGenTone Project")

### How to compile it ?

Just write in a console :  
`sudo apt-get install libasound2-dev`  
`git clone https://github.com/sourcegod/alsatonic.git`  
`make`

### How to run it ?

Examples

`./alsatonic [frequency, default 440Hz] [duration, default 1 seconds]`  
Play default frequency with default duration.


`./alsatonic freq dur start [stop, default 1Hz] [step, default 1Hz]` 
Play frequency freq, with duration dur, starting frequency at start, with default
stop frequency, with default step frequency.


`./alsatonic -h`
display the help.


`./alsatonic -F 440 -d 0.2 -s 0 -S 400 -t 20`
Play sequence frequencies from 440 HZ, with duration 0.2 seconds, starting at
440+0 HZ, stopping at 440+400 HZ,  by step 20 HZ.


`./alsatonic -n 48 -d 3`
Play note number 48 (A4), with duration 3 seconds.


`./alsatonic -N 51 -d 0.2 -s 0 -S 13 -t 1`
Play sequence notes from 51 note number, with duration 0.5 seconds, starting at
51 note number (C5), stopping at 63 note number (C6),  by step 1 note.


### See also:
[alsabeep](https://github.com/sourcegod/alsabeep.git "AlsaBeep Project")
