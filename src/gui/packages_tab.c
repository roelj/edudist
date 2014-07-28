#include "packages_tab.h"
#include "../database/packages.h"

#include <stdio.h>

#define T_NAME  0
#define T_REPO  1
#define T_LOCAL 2
#define DATABASE_NAME "moefel.db"

static GtkTreeStore* store;

void
gui_packages_populate_store (GtkTreeStore *store, dt_list* packages, bool cleanup)
{
  dt_list* temp = packages;
  while (temp != NULL)
    {
      dt_package* pkg = temp->data;
      GtkTreeIter i;
      gtk_tree_store_append (store, &i, NULL);
      gtk_tree_store_set (store, &i, 
			  T_NAME, pkg->name,
			  T_REPO, pkg->domain,
			  T_LOCAL, pkg->is_local, -1);      

      if (cleanup) dt_package_free (pkg);
      temp = temp->next;
    }

  if (cleanup) dt_list_free (packages);
}

void
gui_packages_handle_search (GtkWidget* widget, void* data)
{
  GtkEntry* input = (GtkEntry*)data;
  const char* keyword = gtk_entry_get_text (input);

  dt_list* dataset = NULL;
  if (db_packages_get_by_keyword (DATABASE_NAME, keyword, &dataset))
    gtk_tree_store_clear (store),
    gui_packages_populate_store (store, dataset, TRUE);
}

GtkWidget*
gui_packages_tab_create ()
{
  /* Widget initalization
   * ------------------------------------------------------------------------ */
  GtkWidget* search_inp = gtk_entry_new ();
  GtkWidget* search_btn = gtk_button_new_with_label ("Search");
  GtkWidget* vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  GtkWidget* top_bar = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  GtkWidget* pkg_bar = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);

  /* Use the MVC pattern that is recommended by Gtk to display a table. */
  store = gtk_tree_store_new (3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN);

  dt_list* dataset = NULL;
  if (db_packages_get_all (DATABASE_NAME, &dataset))
    gui_packages_populate_store (store, dataset, TRUE);

  GtkWidget* pkg_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
  g_object_set (G_OBJECT (pkg_view), "rules-hint", TRUE, NULL);

  GtkCellRenderer* renderer;
  GtkTreeViewColumn* column;

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Name", renderer, "text", T_NAME, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (pkg_view), column);
  g_object_set (G_OBJECT (renderer), "width", 350, NULL);

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Repository", renderer, "text", T_REPO, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (pkg_view), column);
  g_object_set (G_OBJECT (renderer), "background", "#ee9999", "background-set", TRUE, NULL);

  renderer = gtk_cell_renderer_toggle_new ();
  column = gtk_tree_view_column_new_with_attributes ("In library", renderer, "active", T_LOCAL, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (pkg_view), column);
  g_object_set (G_OBJECT (renderer), "xalign", 0, NULL);

  /* Further setup
   * ------------------------------------------------------------------------ */
  gtk_widget_set_size_request (search_inp, 0, 30);
  gtk_widget_set_size_request (search_btn, 0, 30);
  gtk_widget_set_margin_start (search_inp, 10);

  /* Containers
   * ------------------------------------------------------------------------ */
  gtk_box_pack_start (GTK_BOX (top_bar), search_inp, 1, 1, 0);
  gtk_box_pack_end (GTK_BOX (top_bar), search_btn, 0, 0, 10);
  gtk_box_pack_start (GTK_BOX (vbox), top_bar, 0, 0, 10);
  gtk_box_pack_start (GTK_BOX (pkg_bar), pkg_view, 1, 1, 10);
  gtk_box_pack_end (GTK_BOX (vbox), pkg_bar, 1, 1, 10);

  /* Signal handlers
   * ------------------------------------------------------------------------ */
  g_signal_connect (G_OBJECT (search_btn), "clicked",
		    G_CALLBACK (gui_packages_handle_search), search_inp);

  g_signal_connect (G_OBJECT (search_inp), "activate",
		    G_CALLBACK (gui_packages_handle_search), search_inp);

  g_signal_connect (G_OBJECT (vbox), "destroy",
		    G_CALLBACK (gui_packages_quit), NULL);

  return vbox;
}

void
gui_packages_quit ()
{
  gtk_tree_store_clear (store);  
}
