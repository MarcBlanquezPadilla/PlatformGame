Read Me Template (To Remove):
# PlatformGame

Group Members: 

Halloween Night (temp title) is a brief Halloween-themed platformer where you're a little ghost that goes trick or treating, while avoiding the dangers of the world around them. Transform back and forth between a ghost to move around, and a pumpkin to attack.

In the 1st release, the following aspects have been implemented:
- Player Controls: move, jump, climb stairs
- Player Animations: idle, walk, jump, fall, hurt and death
- Level 1 with Parallax Background
- Colliders (ground, spikes and bushes) and Sensors (stairs)
- Small pushback every time player hits an obstacle (spikes, bushes)
- Respawn to starting point if player gets hit 3 times
- God Mode (fly around, can't take damage)

In the 2nd release, the following aspects have been implemented:
- Player Transformation Mechanic: player can transform into Pumpkin Head when pressing Z while walking over a pumpkin.
- Player can perform 2 attacks when transformed, which one-shot enemies
- 2 types of enemies which patrol side to side, and chase the player when close enough
   - A bat: flies around and will deal damage to the player upon touching them.
   - A skeleton: walks on land, jumps, and deals damage with a sword or upon contact too.
- Save & Load:
   - Pressing F5 will save the game state (player position, transformation, items and enemies) 
   - Pressing F6 will load the game state at the last point saved, even if the game restarted.
- CheckPoint: The game saves automatically when reaching a big tree surrounded by pumpkins, positioned a little beyond halfway through the level.



HOW TO PLAY / CONTROLS (Keyboard):
- A: Move Left
- D: Move Right
- W: Climb Up Stairs / Move Up (only in God Mode)
- S: Climb Down Stairs / Move Down (only in God Mode)
- Space Bar: Jump
- Z: when close to a pumpkin, transform into Pumpkin Head / transform back into Ghost 
As a Ghost, you cannot attack, but you jump much further
As Pumpkin Head, you jump less, but you can attack: 
- X: Attack 1 (close range magic swing)
- C: Attack 2 (long range fireball shot, with cooldown)
- H: Show / Hide Player Controls (Help Menu)
- F5: Save State
- F6: Load State

Debug Controls:
- F9: Show Logic (colliders, enemy pathfinding)
- F10: God Mode: 
   - Float around with WASD (no need to jump as gravity is deactivated) 
   - Transform at any moment with Z (without any pumpkin nearby)
- F11: Cap game to 30 FPS
- Esc: Quit Game


- Space Bar: Jump

- Esc: Quit Game

- H: Show / Hide Player Controls (Help Menu)

