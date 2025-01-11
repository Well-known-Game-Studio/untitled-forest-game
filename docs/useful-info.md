# Useful Info

## Blender

## Aseprite

## Unreal Engine

### Design

GameModes are per-level, while GameInstances are per-game. Therefore any
singleton-like objects or managers should be within the GameInstance (similarly
for the SaveGame info).

### Debugging

If you want to draw a lot of debug strings, you should may need to increase the
max number of debug strings that can be drawn per actor type:

```console
r.DebugSafeZone.MaxDebugTextStringsPerActor 1000
```


