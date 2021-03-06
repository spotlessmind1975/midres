# PROGRAMS

This is the list of programs (games and utilities) that can be compiled directly with the MIDRES library. 

To compile a `program` for a specific `target` just run the command:

    make program=[name] target=[target] all

To compile all programs for all targets, run the command:

    ./build_programs.sh

The list of targets is available [on this page](targets.md).

## LIST OF PROGRAMS

- `airattack` - an arcade game clone of "Blitz" ([italian page](https://retroprogramming.iwashere.eu/it:airattack) - [english page](https://retroprogramming.iwashere.eu/airattack))
- `alienstorm` - “remake” of the Space Invaders (スペースインベーダー) game developed by Tomohiro Nishikado and released in 1978 ([italian page](https://retroprogramming.iwashere.eu/it:alienstorm) - [english page](https://retroprogramming.iwashere.eu/alienstorm))
- `benchmarks` - execution benchmarks ([italian page](https://retroprogramming.iwashere.eu/it:benchmarks) - [english page](https://retroprogramming.iwashere.eu/benchmarks))
- `elevator` - a simulation / tactical / cooperative game based onan  elevator simulation ([italian page](https://retroprogramming.iwashere.eu/it:elevator) - [english page](https://retroprogramming.iwashere.eu/elevator))
- `joycheck` - the program has the purpose of verifying if the digital joystick and/or the port to which it is connected works correctly ([italian page](https://retroprogramming.iwashere.eu/it:joycheck) - [english page](https://retroprogramming.iwashere.eu/joycheck))
- `totto` - “remake” of the Flappy Bird game ([italian page](https://retroprogramming.iwashere.eu/it:totto) - [english page](https://retroprogramming.iwashere.eu/totto))

## SOUNDTRACK PLAYERS

In order to compile a collection of soundtracks, it is possible to produce executables with music alone. For this purpose the "IMF" file is incorporated with a generic player. This is the list of executables that can be generated:

- `airattack_intro` - Intro tune for "Air Attack!"
- `airattack_ending` - Ending tune for "Air Attack!"
- `totto_intro` - Intro tune for "Totto!"
- `totto_ending` - Ending tune for "Totto!"
- `elevator_ending` - Ending tune for "The Elevator"
