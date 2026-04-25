# Setup
Basic setup, to quickly start playing with the integration.

## The Quick Way
There's a command to do all the below. **Maybe** it'll work! (No harm if it fails, just pick up where it left off.)
```bat
xmake first-time-setup
```

## The Steady Way
Work your way through these tasks in-order, one at a time.

### Set SKYRIM_PATH
```bat
xmake setup-skyrim-path
```

### Build
```bat
xmake build
```
> ***Note:*** *This will generate a `build/windows/` directory in the **project's root directory** with the build output.*

### SKSE Installation
The mod requires the runtime component of [SKSE](https://skse.silverlock.org/) to run.
```bat
xmake setup-skse
```

### Run
Run Skyrim via SKSE
```bat
xmake launch
```