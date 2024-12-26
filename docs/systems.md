# Systems

The systems in this game provide the core functionality of the game. They are
designed to be modular and can be easily added or removed from the game. The
systems are designed to be as independent as possible, but they can interact
with each other.

At a high level, the systems are:
- `Time Manager`: Manages the time in the game. This includes the day, month,
  year, and time of day.
- `Grid Manager`: Manages the grid of the game. This includes the tiles, the
  entities, and the interactions between them. The grid manager ensures the 3d
  world is accessed in a 2d manner, in the select areas that support grid-based
  tile management for foraging, farming, etc.
- `Growth Manager`: Manages the growth of the plants in the game. This includes
  the growth stages, the time it takes to grow, and the resources required to
  grow. It is also responsible for the decay of the plants.
- `Quest Manager`: Manages the quests in the game. This includes the quests
  themselves, the objectives, and the rewards.
- `Routine Manager`: Manages the routines of the NPCs in the game. This includes
  the routines themselves, the tasks, and the interactions with the player.
  
## Time Manager

The time manager is responsible for managing the time in the game. This includes
the day, month, year, and time of day. The time manager is responsible for
updating the time, advancing the time, and notifying the other systems of the
time changes.

## Grid Manager

The grid manager is responsible for managing the grid of the game. This includes
the tiles, the entities, and the interactions between them. The grid manager
ensures the 3d world is accessed in a 2d manner, in the select areas that support
grid-based tile management for foraging, farming, etc.

The grid manager is used by the player / NPCs to interact with the world, as
well as by some of the other systems to determine the state of the world (a key
example being the `Growth Manager`, which needs to know the state of the world
to determine how plants are growing).

## Growth Manager

The growth manager is responsible for managing the growth of the plants in the
game. This includes the growth stages, the time it takes to grow, and the
resources required to grow. It is also responsible for the decay of the plants.

The growth manager is notified by the `Time Manager` when the time changes, and
it uses this information to update the growth of the plants. The growth manager
uses the `Grid Manager` to determine the state of the world, and it uses the
`Quest Manager` to determine if any quests have been completed.

## Quest Manager

## Routine Manager
