# UnityMain Patch

Executable to patch some Windows targeted Unity games that fail to call the UnityMain function when ran on Linux using Wine / Proton.

Please note that this patch is not guaranteed to fix any Unity game on Proton / Wine. Other issues may require different fixes.

The original concept for this patch can be found [here](https://notabug.org/Krock/dawn/issues/454).

## Games

This patch is currently known to fix the following games on Linux when ran with Proton / Wine.

* Punishing: Gray Raven
* Reverse: 1999

## Installation

1. Download the `patch.exe` from the [releases](https://github.com/axertheaxe/unitymain-patch/releases) or [compile from source](#Compilation).
2. Move the downloaded `patch.exe` to the location of the games executable.
3. Delete or rename the games executable.
4. Rename `patch.exe` to the original name of the games executable.

**Note:** Make sure to back up the original game executable before deleting it.

## Compilation

The patch needs to be compiled with a Windows C compiler, which can be done on Linux using a cross-compiler like [MinGW](https://www.mingw-w64.org).

```sh
git clone https://github.com/axertheaxe/unitymain-patch.git
cd unitymain-patch
x86_64-w64-mingw32-gcc -municode patch.c -o patch.exe
```

Nix users can make use of the included flake.nix as a development shell to automatically obtain all dependencies.

```sh
git clone https://github.com/axertheaxe/unitymain-patch.git
cd unitymain-patch
nix develop
x86_64-w64-mingw32-gcc -municode patch.c -o patch.exe
```

## Contributing

Whether you have code to submit or want to add a game to the list of known fixes, feel free to [open an issue](https://github.com/axertheaxe/unitymain-patch/issues/new)
or [submit a pull request](https://github.com/axertheaxe/unitymain-patch/compare).

## Unlicense

This project is under the [Unlicense](UNLICENSE).
