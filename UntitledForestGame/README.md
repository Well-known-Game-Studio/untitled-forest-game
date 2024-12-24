# Untitled Forest Game (Unreal Engine)

This folder contains the project files for the Unreal Engine 5 game.

## Requirements

This project requires that you have downloaded and installed these plugins into
your Unreal Engine 5 Engine installation. The plugins can be downloaded via [the
Fab marketplace](https://fab.com), and then they can be installed into your
engine via the `Epic Games Launcher`.

* Unreal Engine 5 (>= 5.5)

### Plugins

* [Paper ZD Plugin](https://www.fab.com/listings/6664e3b5-e376-47aa-a0dd-f7bbbd5b93c0)

## Blocking out levels

As we are early in development, we are still mostly just blocking out levels.

Some resources / information that can help with level blockout are provided
below

* We have some material instances in the `Materials` folder, such as
  `MI_VoxelGrass`, `MI_VoxelDirt`, etc. which can be applied to basic shapes
  such as cubes and such that are in the level. These material instances can be
  applied to the static meshes by simply dragging the material instance from the
  content drawer onto the static mesh in the scene you want to apply it to.
* We have enabled the editor modeling tools within unreal engine, which enables
  a `Modeling Mode` (which can be accessed from the top left in the map viewer,
  which defaults to `Selection Mode`). This mode allows you to create custom
  meshes or modify meshes that exist in the level. This can be useful for moving
  the pivot of a static mesh or for making a slightly more complex static mesh
  like a mushroom or a ramp.
