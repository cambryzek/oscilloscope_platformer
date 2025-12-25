# oscilloscope_platformer
2D platformer built using Arduino with output display on oscilloscope.

This game is run by an Arduino Due, music is output by an Arduino Uno, and an oscilloscope outputs the game display with the help of a function generator. This repository stores the code run by the Arduino's, `due_game.ino` handling the majority of the game logic for this project, and `uno_music.ino` playing the custom written music.

The game terrain was built in a spreadsheet: `terrain_builder.csv`

Terrain is easily converted into use in `due_game.ino` using the python notebook: `terrain_compiler.ipynb`

Demonstration video at https://www.youtube.com/watch?v=0Jd6WMV6zR4
