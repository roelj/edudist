#include "packages_tab.h"
#include "../database/packages.h"
#include "../high/command.h"

enum {
  T_NAME = 0,
  T_REPO,
  T_LICENSE,
  T_LOCAL,
  T_DATE
} PACKAGE_MEMBERS;

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
			  T_LICENSE, pkg->license,
			  T_LOCAL, pkg->is_local,
			  T_DATE, pkg->created_at, -1);      

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

void
gui_packages_refresh (GtkWidget* widget, void* data)
{
  GtkEntry* input = (GtkEntry*)data;
  const char* keyword = gtk_entry_get_text (input);

  h_command_update ();

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
  GtkWidget* refresh_btn = gtk_button_new_from_icon_name ("gtk-refresh", GTK_ICON_SIZE_MENU);
  GtkWidget* vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  GtkWidget* top_bar = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  GtkWidget* pkg_bar = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);

  store = gtk_tree_store_new (5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_STRING);

  dt_list* dataset = NULL;
  if (db_packages_get_all (DATABASE_NAME, &dataset))
    gui_packages_populate_store (store, dataset, TRUE);

  GtkWidget* pkg_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
  g_object_set (G_OBJECT (pkg_view), "rules-hint", TRUE, NULL);

  GtkCellRenderer* renderer;
  GtkTreeViewColumn* column;

  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "width", 350, NULL);
  g_object_set (G_OBJECT (renderer), "height", 25, NULL);

  column = gtk_tree_view_column_new_with_attributes ("Name", renderer, "text", T_NAME, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (pkg_view), column);

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("License", renderer, "text", T_LICENSE, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (pkg_view), column);

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Repository", renderer, "text", T_REPO, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (pkg_view), column);

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Published at", renderer, "text", T_DATE, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (pkg_view), column);

  renderer = gtk_cell_renderer_toggle_new ();
  column = gtk_tree_view_column_new_with_attributes ("In library", renderer, "active", T_LOCAL, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (pkg_view), column);

  /* Further setup
   * ------------------------------------------------------------------------ */
  gtk_widget_set_size_request (search_inp, 0, 32);
  gtk_widget_set_size_request (search_btn, 0, 32);
  gtk_widget_set_margin_start (search_inp, 10);

  /* Containers
   * ------------------------------------------------------------------------ */
  gtk_box_pack_start (GTK_BOX (top_bar), search_inp, 1, 1, 0);
  gtk_box_pack_start (GTK_BOX (top_bar), search_btn, 0, 0, 0);
  gtk_box_pack_end (GTK_BOX (top_bar), refresh_btn, 0, 0, 10);
  gtk_box_pack_start (GTK_BOX (vbox), top_bar, 0, 0, 10);
  gtk_box_pack_start (GTK_BOX (pkg_bar), pkg_view, 1, 1, 10);
  gtk_box_pack_end (GTK_BOX (vbox), pkg_bar, 1, 1, 10);

  /* Signal handlers
   * ------------------------------------------------------------------------ */
  g_signal_connect (G_OBJECT (search_btn), "clicked",
		    G_CALLBACK (gui_packages_handle_search), search_inp);

  g_signal_connect (G_OBJECT (refresh_btn), "clicked",
		    G_CALLBACK (gui_packages_refresh), search_inp);

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
