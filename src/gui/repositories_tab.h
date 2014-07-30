/*
 * Copyright (C) 2014  Roel Janssen <roel@moefel.org>
 *
 * This file is part of moefel.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GUI_REPOSITORIES_TAB_H
#define GUI_REPOSITORIES_TAB_H

/**
 * @file   gui/repositories_tab.h
 * @brief  The "Repositories" tab container of the graphical user interface.
 * @author Roel Janssen
 */

/**
 * @namespace gui::repositories_tab
 * The graphical user interface's "Repositories" tab container.
 * 
 * @note The prefix for this namespace is "gui_repositories_tab_".
 */

#include <gtk/gtk.h>

/**
 * Starting the graphical user inteface can be done using this function.
 * @return a GtkWidget that can be set as the notebook's tab content.
 */
GtkWidget* gui_repositories_tab_create ();

/**
 * This function should be called when the user wants to add a repository.
 */
void gui_repositories_tab_change (GtkWidget* widget, void* data);

/**
 * This function should be called to properly clean up the allocated memory
 * used by this component.
 */
void gui_repositories_tab_quit ();

#endif//GUI_REPOSITORIES_TAB_H
