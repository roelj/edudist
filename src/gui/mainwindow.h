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

#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H

/**
 * @file   gui/mainwindow.h
 * @brief  The main window of the graphical user interface.
 * @author Roel Janssen
 */

/**
 * @namespace gui::mainwindow
 * The graphical user interface's main window.
 * 
 * @note The prefix for this namespace is "gui_mainwindow_".
 */

#include <gtk/gtk.h>

/**
 * Starting the graphical user inteface can be done using this function.
 * @param argc  The number of arguments given to the program.
 * @param argv  The argument's data.
 */
void gui_mainwindow_init (int argc, char** argv);

/**
 * This function is called when the graphical user interface closes.
 */
void gui_mainwindow_quit ();


#endif//GUI_MAINWINDOW_H
