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
* Player quest
* Grid management
* Growth management (mossy tiles)
* Time management
* Player attributes / stamina
* Scale change + mount / takeover (moss to stone player change)
* Inventory (once stone)

