/*************************************************************************/
/*  context_gl_x11_egl.cpp                                                   */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2019 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2019 Godot Engine contributors (cf. AUTHORS.md)    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "context_gl_x11.h"

#include <EGL/egl.h>

struct ContextGL_X11_Private {

	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
};

void ContextGL_X11::release_current() {

	eglMakeCurrent(p->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
}

void ContextGL_X11::make_current() {

	eglMakeCurrent(p->display, p->surface, p->surface, p->context);
}

void ContextGL_X11::swap_buffers() {

	eglSwapBuffers(p->display, p->surface);
}

Error ContextGL_X11::initialize() {

	static const EGLint cfg_attrs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_DEPTH_SIZE, 24,
		EGL_NONE
	};

	static const EGLint ctx_attrs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	EGLBoolean result;
	EGLConfig config;
	EGLint num_config;

	Window root = DefaultRootWindow(x11_display);
	XSetWindowAttributes swa;
	x11_window = XCreateWindow(x11_display, root, 0, 0,
		OS::get_singleton()->get_video_mode().width,
		OS::get_singleton()->get_video_mode().height,
		0, CopyFromParent, InputOutput, CopyFromParent, 0, &swa);
	if (!x11_window)
		goto error;
	XSelectInput(x11_display, x11_window, StructureNotifyMask);
	XMapWindow(x11_display, x11_window);

	p->display = eglGetDisplay((EGLNativeDisplayType)x11_display);
	if (p->display == EGL_NO_DISPLAY)
		goto error;
	result = eglInitialize(p->display, 0, 0);
	if (result == EGL_FALSE)
		goto error;
	result = eglChooseConfig(p->display, cfg_attrs, &config, 1, &num_config);
	if (result == EGL_FALSE)
		goto error;
	result = eglBindAPI(EGL_OPENGL_ES_API);
	if (result == EGL_FALSE)
		goto error;
	p->context = eglCreateContext(p->display, config, EGL_NO_CONTEXT, ctx_attrs);
	if (p->context == EGL_NO_CONTEXT)
		goto error;

	p->surface = eglCreateWindowSurface(p->display, config, (EGLNativeWindowType)x11_window, 0);
	if (p->surface == EGL_NO_SURFACE)
		goto error;
	make_current();

	return OK;

error:
	if (p->surface != EGL_NO_SURFACE)
		eglDestroySurface(p->display, p->surface);
	if (p->context != EGL_NO_CONTEXT)
		eglDestroyContext(p->display, p->context);
	if (p->display != EGL_NO_DISPLAY)
		eglTerminate(p->display);
	return ERR_UNCONFIGURED;
}

int ContextGL_X11::get_window_width() {

	XWindowAttributes xwa;
	XGetWindowAttributes(x11_display, x11_window, &xwa);

	return xwa.width;
}

int ContextGL_X11::get_window_height() {

	XWindowAttributes xwa;
	XGetWindowAttributes(x11_display, x11_window, &xwa);

	return xwa.height;
}

void ContextGL_X11::set_use_vsync(bool p_use) {

	eglSwapInterval(p->display, p_use ? 1 : 0);
	use_vsync = p_use;
}

bool ContextGL_X11::is_using_vsync() const {

	return use_vsync;
}

ContextGL_X11::ContextGL_X11(::Display *p_x11_display, ::Window &p_x11_window, const OS::VideoMode &p_default_video_mode) :
		x11_window(p_x11_window) {

	default_video_mode = p_default_video_mode;
	x11_display = p_x11_display;

	p = memnew(ContextGL_X11_Private);
	p->display = EGL_NO_DISPLAY;
	p->context = EGL_NO_CONTEXT;
	p->surface = EGL_NO_SURFACE;

	use_vsync = false;
}

ContextGL_X11::~ContextGL_X11() {

	release_current();
	eglDestroySurface(p->display, p->surface);
	eglDestroyContext(p->display, p->context);
	eglTerminate(p->display);
	memdelete(p);
}
