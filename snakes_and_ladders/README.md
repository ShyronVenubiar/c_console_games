# Snakes and Ladders (C Console Game)

This project is a complete console implementation of the classic board game Snakes and Ladders, written in C.
It was one of my first major programming projects and was created during my early learning phase, before I knew about modern project structures or module design.
Nevertheless, it already contains many exciting elements that I developed myself step by step at the time.

---
## Features
- Dynamically generated playing field (7x7, 8x8, or 9x9)
- Different difficulty levels:
  - Normal - classic gameplay
  - Hard - fewer ladders,more snakes
  - Extreme - D4 instead of D6, no ladders, guaranteed endgame snake
- Random placement of snakes and ladders
- Player movement includes:
  - Rolling the dice
  - Snake bite (falling back)
  - Climbing ladders
- Colored console output
- Some Easter Eggs

---

## What I learned from this project
This project was an important milestone for me.
Among other things, I learned:
- How to use dynamic memory (calloc, free)
- How to work with multidimensional arrays
- How to structure a larger program into functions
- How to render consoles with colors (SetConsoleTextAttribute)
- Debugging more complex processes

Even though many things wourld be structured differently (and more modularly) today, this project shows well how I approached larger tasks.

---

## Project Structure
The entire game is contained in a single main.c file, as it was one of my early projects.
This was perfectly fine for the learning phase at the time - and I am deliberately leaving it that way to show the development status authentically.
This project has deliberately not been refactored or modernized.
