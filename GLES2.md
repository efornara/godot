GLES2
=====

This is a backport of the GLES2 renderer to the 3.0 branch (stable releases).
Once a 3.1 release candidate is available, this branch will become inactive.

Since master might start to diverge from 3.0, backporting of new features might
soon become unfeasible. In that case, and if you need the additional features,
you are better off tracking master, for which frequent builds should be
available.

Usage
-----

Download the pre-built binaries
[here](https://github.com/efornara/godot/releases).

By default, the editor uses the GLES2 renderer. The GLES3 renderer is still
included, but if you do want to use it, you need to invoke the editor like
this:

	godot.windows.opt.tools.32.exe --video-driver GLES3

The game will then also run using the GLES3 renderer. You can then force the
use of the GLES2 renderer when running / debugging the game. There is a new
option in the editor settings for that:

	run/video_driver/force_gles2

In godot 3.1, the selection of the renderer is part of the project settings.
This branch aims for identical behaviour compared to its corresponding
official stable 3.0.x release, so, the selection of the renderer is decided
at run / export time instead.

Android
-------

A new export option is available:

	graphics/api

It is used to select the renderer and to set the glEsVersion attribute in
the manifest.

Scope and Features
------------------

The main use case for this branch is a beginner who focuses on simple 2D
games (mostly to learn the engine) and prefers a more stable editor compared
to master, but has on old PC and/or a low-end Android phone.

Only 2D is available (e.g. 3D viewports are blank), and visual effects
like particles and 2D lighting are not. However, every element of the
project is kept between saves, so you should be able to share your
projects and collaborate with people using the corresponding official
3.0.x-stable release. Frequent backups are nevertheless advised.

In other words, the more you focus on the game logic, the more you will
be able to use this branch; the more you focus on polishing the look of
the game, the more you will encounter problems.

For example, if you grab some 2D resources like these:
https://opengameart.org/users/kenney and focus on learning project
organization, nodes, scenes, events, sprites, animations, scripting, etc...
your experience should feel pretty much the same as the experience of someone
using the corresponding official 3.0.x-stable release.

It goes without saying that this branch is unsupported and most likely buggy.
Users looking to release games to the public are advised to wait for the
official release of godot 3.1.

Master Builds
-------------

You can find the latest master builds
[here](https://archive.hugo.pro/godot/master/).
Given the code in master, I suspect you might have some small problems
running it. If that is the case, here are some tips that might help.

Start it with:

	godot.exe --video-driver GLES2

That should allow you to create a new project. If you cannot edit the
newly created project, open the command prompt, go to the project directory
and run it from there like this:

	godot.exe --video-driver GLES2 -e

Go to the project settings and change
Rendering / Quality / Driver / Driver Name to GLES2.

Known Bugs and Limitations
--------------------------

- 2D Only

- No 2D lighting

- No particle effects

- Unknown issues with some textures (e.g. float format)

- Only 32-bit Windows editor and Android templates released

- Releases compiled without SSL support

- No support for MacOS/iPhone

Issues:

- [17429](https://github.com/godotengine/godot/issues/17429) In the editor, Texture Region window for Sprite does not pan or zoom properly GLES2

Reporting Bugs
--------------

The most relevant bugs to report on this branch are hard crashes at startup
on a Windows OpenGL 2.1 machine, making sure first that the official
Godot-2.1.x runs.

Graphical glitches should usually be reported on the master repository, but
_only after reproducing them on a master build_.
