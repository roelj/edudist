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

#ifndef GUI_PACKAGES_TAB_H
#define GUI_PACKAGES_TAB_H

/**
 * @file   gui/packages_tab.h
 * @brief  The "Packages" tab container of the graphical user interface.
 * @author Roel Janssen
 */

/**
 * @namespace gui::packages_tab
 * The graphical user interface's "Packages" tab container.
 * 
 * @note The prefix for this namespace is "gui_packages_tab_".
 */

#include <gtk/gtk.h>

/**
 * Starting the graphical user inteface can be done using this function.
 * @return a GtkWidget that can be set as the notebook's tab content.
 */
GtkWidget* gui_packages_tab_create ();

/**
 * This function should be called to properly clean up the allocated memory
 * used by this component.
 */
void gui_packages_quit ();

#endif//GUI_PACKAGES_TAB_H
