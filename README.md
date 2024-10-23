# Coin Collection Game

## Overview
This is a console-based coin collection game where players navigate a grid, collect coins, avoid bombs, and ultimately find a key to escape through a door. The game adjusts difficulty levels, which influence the grid size, number of bombs, and moves allowed.

## Features
- Random placement of coins and bombs.
- Different difficulty levels (Easy, Medium, Hard).
- Ability to undo moves a limited number of times.
- Hint system for guiding the player towards the key and door.
- Grid-based movement using W/A/S/D keys.

## Requirements
- C++ compiler (e.g., g++, clang++)
- C++11 standard or higher

Compile the Code:
g++ -o AI_B_i222578.cpp

Gameplay Instructions
Choose a difficulty level:

Easy: 10x10 grid, 24 moves, 6 undos
Medium: 15x15 grid, 20 moves, 4 undos
Hard: 20x20 grid, 18 moves, 1 undo
Use the following keys to navigate:

W: Move Up
A: Move Left
S: Move Down
D: Move Right
U: Undo the last move
Collect coins, avoid bombs, and find the key to escape through the door.

Game Logic
The player starts at the top-left corner of the grid.
Coins are visible on the grid, while bombs are hidden until stepped on.
The player receives hints about their proximity to the key and door.
The game ends when the player collects the key and reaches the door or steps on a bomb.


License:
This project is licensed under FAST NUCES. Feel free to use.
