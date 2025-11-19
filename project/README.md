# Beauty and flipyflop
#### Video Demo: https://youtu.be/rT3JLzNZwxk
#### Description:

**Author: Nhat Quynh**
* **GitHub:** nhatquynh1107
* **edX:** nhatquynh1107
* **Location:** Ho Chi Minh City, Vietnam
* **Date:** November 14, 2025

##### Game plot

**Beauty and Flipyflop** is a puzzle-strategy game inspired by the classic title **Mummy Maze**, and by one of my dear friends, **flipyflop**.

Your objective is simple: **outsmart the monsters and reach the exit.**

You play as a *lost princess* who has wandered into a mysterious dungeon filled with dangerous creatures. Each level is a pre-designed maze, and your mission is to find a safe path to the exit without being caught.

Fortunately, you possess a map for every maze, but that doesn’t mean escape will be easy. You must plan your movement, predict how monsters respond, and make your way to safety.

---

###### Key features

* **Turn-based game**: Every valid move you make (W/A/S/D/Space) that are still in the maze allows the monsters to make the move.
* **Intelligent Enemy Move:** Monsters don't move randomly! They use a greedy algorithm to find the shortest path to you.
* **Different monster type**: 
	* **Goblin**: Moves **2 tiles** per turn, the method it uses to move is smarter when the level is higher.
	* **Scorpion**: Moves **1 tile** per turn, the method it uses to move is similar to the goblin.
* **Level progression**: Features multiple level with increasing difficulty.
* **Progress Save**: The level is automatically saved to “progress.txt”.
* **In-game control**: Apart from basic move (W/A/S/D and space), we can redo (‘R’) or reset (‘M’) our moves.

---

##### How to Play

The game is controlled entirely by the keyboard.

| Key | Action |
| :--- | :--- |
| **W / A / S / D** | Move Up / Left / Down / Right |
| **Space** | **Wait a turn** (stay still while enemies move) |
| **R** | **Redo** (undo your last move) |
| **M** | **Reset** the current level to its starting position |
| **Q** | **Quit** the game at any time |
| **Enter** | Make selections in the menu screens |

—

##### Game visualisation
The game visualisation is very simple. The blue circle represents our player, the lost princess. The red circle is the Goblin, the yellow one is the Scorpion and lastly the small pink circle is the trap.

—

##### Maze Rendering
The maze walls are not stored as characters but as a 2D vector of integers. Each integer is a bitmask that defines which o the four walls for that tile should be drawn.
* **mask & 1**: Left wall.
* **mask & 2**: Right wall.
* **mask & 4**: Up wall.
* **mask & 8**: Down wall.

##### Monsters’ moving
On level 1, the Goblin is set to move 2 tiles randomly. So there’s nothing to explain.
From level 2 onwards, the Goblin and Scorpion move based on the Manhattan distance heuristic: **abs(x2 - x1) + abs(y2 - y1).

In each monsters’ turn, it checks all valid non-wall adjacent tiles and moves to the one that results in the smallest Manhattan distance to the player. This greedy solution makes them a constant threat.

— 

##### Dependencies

1.  **C++ Compiler:** A modern compiler like **g++** (MinGW for Windows).
2.  **SFML Library:** The Simple and Fast Multimedia Library. This project is set up to find SFML's `include` and `lib` folders inside the `src/SFML/` directory.
3.  **Font File:** `arial.ttf` must be present in the same directory as the final executable for any text (menus, HUD) to render.

—

##### Compilation Command (g++)

Navigate to the `src` directory (where `main.cpp` is located) and run the following command to compile the game:

```bash
g++ main.cpp frontend.cpp -I SFML/include -L SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -o main.exe

—

##### Running the game

After compiling, you will have main.exe. Before running it, you must copy the SFML DLLs (e.g., sfml-graphics-2.dll, sfml-window-2.dll, etc.) from your SFML download's /bin folder into the same src directory as main.exe.
Finally, ensure arial.ttf is also in the src directory, 
— 

##### How to win the game

A unique twist defines the gameplay:

If 2 monsters collide on the same tile, the scorpion will be eaten by the mummy.

There's also a special pattern of the goblin so that we win the game. Those monsters prioritize moving horizontally to moving vertically. For example, when there are 2 options for a monster to get closer to the princess, they will always choose the horizontal option. This pattern requires the players to figure out themselves so that they can trap those monsters into the wall of the maze in order to escape easier.

Will you escape the maze and return to the royal palace with your beloved family?
Or will the monsters catch you first?
