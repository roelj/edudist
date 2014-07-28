#include "mainwindow.h"
#include "packages_tab.h"

void
gui_mainwindow_init (int argc, char** argv)
{
  /* Initalization
   * ------------------------------------------------------------------------ */
  gtk_init (&argc, &argv);

  GtkWidget* window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  GtkWidget* tabs = gtk_notebook_new ();

  GtkWidget* pkgs_tab = gui_packages_tab_create ();
  GtkWidget* repos_tab = gtk_label_new ("Repositories tab..");

  GtkWidget* tabs_lbl_pkgs = gtk_label_new ("Packages");
  GtkWidget* tabs_lbl_repos = gtk_label_new ("Repositories");

  /* Further setup
   * ------------------------------------------------------------------------ */
  gtk_window_set_title (GTK_WINDOW (window), "Moefel");
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);

  gtk_notebook_append_page (GTK_NOTEBOOK (tabs), pkgs_tab, tabs_lbl_pkgs);
  gtk_notebook_append_page (GTK_NOTEBOOK (tabs), repos_tab, tabs_lbl_repos);

  /* Containers
   * ------------------------------------------------------------------------ */
  gtk_container_add (GTK_CONTAINER (window), tabs);

  /* Signal handlers
   * ------------------------------------------------------------------------ */
  g_signal_connect (G_OBJECT (window), "destroy",
		    G_CALLBACK (gui_mainwindow_quit), NULL);

  /* Display
   * ------------------------------------------------------------------------ */
  gtk_widget_show_all (window);
  gtk_main ();
}

void
gui_mainwindow_quit ()
{
  gtk_main_quit();
}
