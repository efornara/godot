This is a backport of the GLES2 renderer to the 3.0 branch (stable releases).
Once a 3.1 release candidate is available, this branch will become inactive.

Since master might start to diverge from 3.0, backporting of new features might
soon become unfeasible. In that case, and if you need the additional features,
you are better off tracking master, for which frequent builds should be
available.

The main use case for this branch is a beginner who focuses on simple 2D
games mostly to learn the engine, prefers a more stable editor compared
to master, but has on old PC and/or a low-end Android phone. For this,
this branch should be fairly useable.

It goes without saying that this branch is unsupported and most likely buggy.
Users looking to release games to the public are advised to wait for the
official release of godot 3.1.

Usage
-----

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
stable release, so, the selection of the renderer is decided at run / export
time instead.

Android
-------

A new export option is available:

	graphics/api

It is used to select the renderer and to set the glEsVersion attribute in
the manifest.

Known Bugs and Limitations
--------------------------

- 2D Only

- No particle effects

- Unknown issues with some textures (e.g. float format)

- Releases compiled without SSL support

- No support for MacOS/iPhone
