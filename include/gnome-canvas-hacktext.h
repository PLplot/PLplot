/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 *  gnome-canvas-hacktext.c: Hacktext CanvasItem, used for the PrintPreview context
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public License
 *  as published by the Free Software Foundation; either version 2 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Authors:
 *    Federico Mena <federico@nuclecu.unam.mx>
 *    Raph Levien <raph@acm.org>
 *    Lauris Kaplinski <lauris@helixcode.com>
 *
 *  Copyright (C) 1998-1999 The Free Software Foundation
 *  Copyright (C) 2000-2002 Ximian Inc.
 *
 */

#ifndef __GNOME_CANVAS_HACKTEXT_H__
#define __GNOME_CANVAS_HACKTEXT_H__

#include <glib.h>

G_BEGIN_DECLS

/* Hacktext item for the canvas.  The API is totally unstable - it needs to be replaced with one
 * that supports Unicode and the merged GnomeText/GScript API. However, I need a text item now,
 * and the GnomeText/GScript integration is going to take a bit more effort.
 *
 * The following object arguments are available:
 *
 * name			type			read/write	description
 * ------------------------------------------------------------------------------------------
 * text			char *			RW		The string of the text item.
 * glyphlist            GnomeGlyphList *        W               Glyphlist
 * fill_color		string			W		X color specification for fill color,
 *							        or NULL pointer for no color (transparent).
 * fill_color_gdk	GdkColor*		RW		Allocated GdkColor for fill.
 */

#define GNOME_TYPE_CANVAS_HACKTEXT        (gnome_canvas_hacktext_get_type ())
#define GNOME_CANVAS_HACKTEXT(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), GNOME_TYPE_CANVAS_HACKTEXT, GnomeCanvasHacktext))
#define GNOME_CANVAS_HACKTEXT_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST    ((k), GNOME_TYPE_CANVAS_HACKTEXT, GnomeCanvasHacktextClass))
#define GNOME_IS_CANVAS_HACKTEXT(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), GNOME_TYPE_CANVAS_HACKTEXT))
#define GNOME_IS_CANVAS_HACKTEXT_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE    ((k), GNOME_TYPE_CANVAS_HACKTEXT))

typedef struct _GnomeCanvasHacktext      GnomeCanvasHacktext;
typedef struct _GnomeCanvasHacktextPriv  GnomeCanvasHacktextPriv;
typedef struct _GnomeCanvasHacktextClass GnomeCanvasHacktextClass;

#include <libgnomecanvas/libgnomecanvas.h>

struct _GnomeCanvasHacktext {
	GnomeCanvasItem item;

	char *text;			/* String of the text item */
	guint fill_color;		/* Fill color, RGBA */
	gulong fill_pixel;		/* Color for fill */
	guint fill_set : 1;		/* Is fill color set? */

	double size;			/* size in user units */
	double x, y;			/* x, y coords of text origin */

	/* Antialiased specific stuff follows */
	guint32 fill_rgba;		/* RGBA color for filling */
	GnomeCanvasHacktextPriv *priv;	/* Private data */
};

struct _GnomeCanvasHacktextClass {
	GnomeCanvasItemClass parent_class;
};

GType gnome_canvas_hacktext_get_type (void);

G_END_DECLS

#endif /* __GNOME_CANVAS_HACKTEXT_H__ */
