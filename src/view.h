/*
 *  L3afpad - GTK+ based simple text editor
 *  Copyright (C) 2004-2005 Tarot Osuji
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _VIEW_H
#define _VIEW_H

gint get_current_keyval(void);
void clear_current_keyval(void);
void scroll_to_cursor(GtkTextBuffer *buffer, gdouble within_margin);
void force_call_cb_modified_changed(GtkWidget *view);
void force_block_cb_modified_changed(GtkWidget *view);
void force_unblock_cb_modified_changed(GtkWidget *view);
void set_view_scroll(void);
gint check_text_modification(void);
GtkWidget *create_text_view(void);
gboolean autosave_get_state(void);
void autosave_set_state(gboolean state);
guint autosave_get_timer(void);
void autosave_set_timer(guint milliseconds);
gboolean autosave_get_same_dir(void);
void autosave_set_same_dir(gboolean state);
guint autosave_get_immediate_changes(void);
void autosave_set_immediate_changes(guint num_changes);
void autosave_reset_num_changes(void);

#endif /* _VIEW_H */
