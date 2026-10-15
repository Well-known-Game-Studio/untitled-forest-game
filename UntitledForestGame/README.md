# Untitled Forest Game (Unreal Engine)

This folder contains the project files for the Unreal Engine 5 game.

## Requirements

* Unreal Engine 5.8 (installed via the `Epic Games Launcher`)

### Plugins

* [Paper ZD Plugin](https://www.fab.com/listings/6664e3b5-e376-47aa-a0dd-f7bbbd5b93c0)
  (2.2.4 or newer), downloaded from Fab and installed into the UE 5.8 engine
  via the `Epic Games Launcher`.

## Building from the command line (macOS)

```sh
"/Users/Shared/Epic Games/UE_5.8/Engine/Build/BatchFiles/Mac/Build.sh" \
  UntitledForestGameEditor Mac Development \
  -Project="$PWD/UntitledForestGame.uproject" -WaitMutex
```

## Using Claude Code with the editor (MCP)

The project enables Epic's experimental **Unreal MCP** plugin (`ModelContextProtocol`)
plus a few editor toolsets (`EditorToolset`, `ConfigSettingsToolset`,
`AutomationTestToolset`, `UMGToolSet`). `Config/DefaultEditorPerProjectUserSettings.ini`
makes the editor start the MCP server automatically on
`http://127.0.0.1:8000/mcp`, and the repo-root `.mcp.json` points Claude Code at it.

1. Open the project in the UE 5.8 editor (the server starts with the editor).
2. Run `claude` from the repo root; the `unreal-mcp` server is picked up from
   `.mcp.json` and can be inspected with `/mcp`.
3. The server exposes `list_toolsets`, `describe_toolset`, and `call_tool`;
   Claude discovers the editor tools on demand through those.

Settings live under Edit > Project Settings > Plugins > Model Context Protocol.
Change the port there and in `.mcp.json` if 8000 is taken. The server can also
be started or stopped from the console with `ModelContextProtocol.StartServer`
and `ModelContextProtocol.StopServer`.

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

  ![CleanShot 2024-12-24 at 14 23 50@2x](https://github.com/user-attachments/assets/ea43cd6d-2698-4fb1-ad79-a6f5ee7861a3)

  When using the modeling mode, you should usually start with creating the basic
  type of shape you want to start with (e.g. box, sphere, cylinder), and after
  you've hit `Accept` on the creation, you can then use the deform, model, or
  mesh tools to adjust it as you see fit.

  ![CleanShot 2024-12-24 at 14 26 31@2x](https://github.com/user-attachments/assets/759fedf9-703a-4118-b5db-5cfaef611ea8)
