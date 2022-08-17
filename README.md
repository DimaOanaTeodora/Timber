# Timber :deciduous_tree: :axe:
screen resolution is 1,920 x 1,080

:evergreen_tree: Demo [YouTube](https://youtu.be/gm4AyWtesWc)

- Download SFML [here](https://www.sfml-dev.org/download/sfml/2.5.1/)
- Microsfot Visual Studio 2022 

:pick: This project uses the simplest possible version of a game loop, every game will need these phases in the code:

1. Get the player’s input (if any).
2. Update the scene based on things such as artificial intelligence, physics, or the player’s input.
3. Draw the current scene.
4. Repeat these steps at a fast-enough rate to create a smooth, animated game world.

- **The player’s score**: Each time the player chops a log, they will get one point. They can chop a log with either the left or the right arrow (cursor) key.
- **Player character**: Each time the player chops, they will move to/stay on the same side of the tree relative to the cursor key they use. Therefore, the player must be careful which side they choose to chop on.
- When the player chops, a simple axe graphic will appear in the player character’s hands.
- **Shrinking time-bar**: Each time the player chops, a small amount of time will be added to the ever-shrinking time-bar.
- **The lethal branches**: The faster the player chops, the more time they will get, but also the faster the branches will move down the tree and therefore the more likely they are to get squished. The branches spawn randomly at the top of the tree and move down with each chop.
- When the player gets squished – and they will get squished quite regularly – a gravestone graphic will appear.
- **The chopped log**: When the player chops, a chopped log graphic will whiz off, away from the player.
- **Just for decoration**: There are three floating clouds that will drift at random heights and speeds, as well as a bee that does nothing but fly around.
- **The background**: All this takes place on a pretty background.

*So, in a nutshell, the player must frantically chop to gain points and avoid running out of time. As a slightly perverse, but fun consequence, the faster they chop, the more likely their squishy demise is.*