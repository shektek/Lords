LORDS

Intended to be an homage to Lords of the Realm II. Uses irrlicht 1.8.3 and bullet 2.86.1. It expects these to be a level above the .git folder.

If you run it you'll be greeted with test stuff. Perhaps half of the structures needed for the overworld are in place. You can start it up, use the gui, change the camera using the c key, and drop cubes in the middle of the test map with left click. Code is there to handle peasant generation, territory generation/loading, terrain loading, level loading, etc.

SteelEngine is really the name of the level loading structure which is shared by this and INFOWAR. Generally bitmaps are used to do this sort of thing but I decided to use plain text. It probably wasn't the wisest decision in the long term. One day I'll split that particular code into it's own library.