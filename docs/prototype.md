# Prototype

This document describes the prototype for UFG.

It will consist of a tutorial level which contains some of the basic mechanics
of the game and interaction with some of the game systems.

It will take place on a large rock in a forest stream, you are a tiny bit of
moss and your goal is to rejoin the larger clump of moss that you were knocked
off from.

Actions:

* Lose stamina / energy as you move around
* Partially replenish energy at the end of each day if you're not on mossy ground
* Fully replenish energy at the end of each day if you're on mossy ground
* Gather some moss energy by moving over moss tiles
* Gather a little more moss energy per tick while stationary over a mossy tile
* Gather more moss energy by consuming moss tiles
* Grow more moss on a rock to cross a little stream
* Talk to beetle and give him some moss
* Make moss bed
* Get bug to work for you
* Get enough moss energy to take over rocks
* Gather mushrooms as rocky boi
* Grow mushrooms on log
* Climb back up and leave area

Note: we're gonna try to keep the UI light / simple so that we don't clutter the
screen more than we need to and so that we have less work to do for now.

Note: to let players know when they should start sleeping / when the day-night
cycle is about to restart (after it gets dark of course), we'll have some
fireflies spawn when there's an hour or so left. Similarly, we could have some
bio-luminescent vegetation like certain mushrooms and such start glowing as
well. We do have a full moon / star system as well which provides some light, so
we could adjust its lighting intensity as well. I guess what I'm saying is we
have a lot of knobs to turn.

Systems:
* NPC interaction
* NPC goals / routine
* NPC manager (for routines and such, even when they're not in the active level)
* Dialogue system
* UI (moss energy, time of day, pause menu, inventory, etc.)
* Player quest
* Item / Interactable interface
* Grid management
* Growth management (mossy tiles)

  The growth management system will be responsible for managing the growth of
  various plants and fungi in the game. It will be responsible for determining
  when a plant or fungus can grow, how much it grows, and what it grows into.
  This will require interaction with the grid system to determine where the
  plant or fungus can grow, and with the time management system to determine
  when it can grow.

* Time management

  Right now we'll use the SunMoonDaySequenceActor to actually get/set the time
  and render the time of day. We'll need to have code which allows the time to
  be changed, as well as how fast the time passes. Finally we'll need events
  which are broadcasted whenever the time changes, so that the growth manager
  can do any updates that are required.

* Player attributes / stamina
* Scale change + mount / takeover (moss to stone player change)
* Inventory (once stone)
* Save Game system for saving player state, NPC states, grid manager / item
  states, and time manager states.

## TODO List

- [ ] Finalize grid management so that items can be placed on the grid
- [ ] Implement growth management
- [ ] Implement time management - right now we have the SunMoonDaySequenceActor
      in the game and can set the time, but that time isn't able to be tracked
      by any systems.
- [ ] Implement player attributes / stamina
- [ ] Implement NPC goals / routine
- [ ] Implement NPC interaction (mostly done, just need to refine and ensure
      that the integration with npc management / routines is working)
- [ ] Implement NPC manager
- [ ] Implement the Player quest system
- [ ] Implement the dialogue system (mostly done, just need to ensure we can use
      the dialog importing and wire it up properly to various states of the
      NPCs)
- [ ] Implement the UI (moss energy, time of day, etc.)
- [ ] Implement the item / interactable interface (mostly done, just need to
      ensure that the interactibles and grid system work well together).
- [ ] Finalize the inventory system and wire up to UI
- [ ] Implement the save game system
- [ ] Implement scale change + mount / takeover
- [ ] Implement the into cutscene

## Stretch Goals

- [ ] Add in some effects that are triggered (such as fireflies)
- [ ] VFX for growth
- [ ] VFX for forest ambiance such as wind, leaves falling, etc.
- [ ] SFX for forest ambiance such as wind, leaves falling, etc.
- [ ] Editor plugin for creating, viewing, and modifying the grid(s) in the game
- [ ] Editor plugin for creating, viewing, and modifying the dialogues in the
      game
- [ ] Editor plugin for creating and modifying the quests
- [ ] NPC interaction cutscenes
- [ ] Fancy water
- [ ] Interactable water

## Open Questions

1. How will we define and maintain the quests in the game? 
2. How will we define and maintain the dialogues in the game?
3. How will we manage the transition between 3d and 2d (e.g. for when the player
   takes over the stones)?
4. How will we define and maintain the NPC routines?
5. At what point are the grids / NPCs registered with their various managers?
