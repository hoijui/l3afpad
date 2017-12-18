/*
 *  L3afpad - GTK+ based simple text editor
 *  Copyright (C) 2017 Tarot Osuji
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

#include "l3afpad.h"
//#include <gdk/gdkkeysyms.h>
//#include <string.h>
#include <stdint.h>
#include <libgen.h>
#include <stdlib.h>

#define MAX_FILE_PATH_SIZE 256

static gboolean auto_save = FALSE;
static guint auto_save_timer = 10000;
static gboolean auto_save_same_dir = TRUE;
static guint auto_save_immediate_changes = 150;

typedef struct {
	/** GTK handler/signal ID of the auto-save timer or 0 if none. */
	guint timer_id;
	/** Number of changes to the text buffer since last (auto-)save. */
	guint changes;
	/** Name of the temporary auto-save file. Empty ([0] == '\0') if none. */
	gchar filename[256];
} AutoSaveData;

static AutoSaveData auto_save_data = { 0, 0, "" };

static void AutoSaveData_init(AutoSaveData *data)
{
	data->timer_id = 0;
	data->changes = 0;
	data->filename[0] = '\0';
}

gboolean autosave_get_state(void)
{
	return auto_save;
}

void autosave_set_state(gboolean state)
{
	auto_save = state;
}

guint autosave_get_timer(void)
{
	return auto_save_timer;
}

void autosave_set_timer(guint milliseconds)
{
	auto_save_timer = milliseconds;
}

gboolean autosave_get_same_dir(void)
{
	return auto_save_same_dir;
}

void autosave_set_same_dir(gboolean state)
{
	auto_save_same_dir = state;
}

guint autosave_get_immediate_changes(void)
{
	return auto_save_immediate_changes;
}

void autosave_set_immediate_changes(guint num_changes)
{
	auto_save_immediate_changes = num_changes;
}

static void autosave_reset_num_changes(void)
{
	auto_save_data.changes = 0;
}

/** @see https://stackoverflow.com/questions/11871245/knuth-multiplicative-hash */
static inline uint32_t hash(const uint32_t value)
{
	return value * UINT32_C(2654435761);
}

static gboolean idle_handler(GtkWidget *view) {

	// mark this timer as done
	auto_save_data.timer_id = 0;

	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
	if (gtk_text_buffer_get_modified(buffer)) {
		// Create a hash from our buffers pointer value,
		// in order to circumvent the case of multiple opened files
		// with the same name overwriting each others buffer.
		// NOTE In case of a 64bit system, we discard the upper 32bits here,
		//   but as the addresses are very likley to be different in the lower 32bits,
		//   there should generally be no problem.
		const uint32_t pointer_hash = hash((uint32_t) buffer);
		gchar tmp_file_name[MAX_FILE_PATH_SIZE];
		if (pub->fi->filename == NULL) {
			// We are editing a text buffer without having chosen a filename yet
			char *home;
			home = getenv("HOME");
			sprintf(tmp_file_name, "%s/.cache/l3afpad/buffers/%u.txt", home, pointer_hash);
		} else {
			// Known/chosen filename
			gchar real_filename_base_buf[MAX_FILE_PATH_SIZE];
			strcpy(real_filename_base_buf, pub->fi->filename);
			char* real_filename_base = basename(real_filename_base_buf);
			if (auto_save_same_dir) {
				gchar real_filename_dir_buf[MAX_FILE_PATH_SIZE];
				strcpy(real_filename_dir_buf, pub->fi->filename);
				char* real_filename_dir = dirname(real_filename_dir_buf);
				sprintf(tmp_file_name, "%s/.%u_%s", real_filename_dir, pointer_hash, real_filename_base);
			} else {
				char *home;
				home = getenv("HOME");
				sprintf(tmp_file_name, "%s/.cache/l3afpad/buffers/.%u_%s", home, pointer_hash, real_filename_base);
			}
		}

g_print("l3afpad: auto-saving to file: '%s'\n", tmp_file_name);
		gchar *real_filename = pub->fi->filename;
		pub->fi->filename = tmp_file_name;
		/*const gint save_err = */file_save_real(view, pub->fi);
		pub->fi->filename = real_filename;
		gtk_text_buffer_set_modified(buffer, TRUE);

		autosave_reset_num_changes();
	}

	// stop this idle signal
	return FALSE;
}

static gboolean time_handler(GtkWidget *view) {

	/*guint idle_id = */g_idle_add((GSourceFunc) idle_handler, view);

	// stop this timer
	return FALSE;
}

void autosave_cb_buffer_changed(GtkTextBuffer *buffer, GtkWidget *view)
{
	auto_save_data.changes++;
	if (auto_save) {
		if (auto_save_data.timer_id > 0) {
			g_source_remove(auto_save_data.timer_id);
		}
		if (auto_save_data.changes >= auto_save_immediate_changes) {
			time_handler(view);
		} else {
			auto_save_data.timer_id = g_timeout_add(auto_save_timer, (GSourceFunc) time_handler, view);
		}
	}
}

void autosave_cb_file_saved()
{
	autosave_reset_num_changes();
}

