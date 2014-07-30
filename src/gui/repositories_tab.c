#include "repositories_tab.h"
#include "../database/repositories.h"
#include "../high/command.h"

#define T_NAME    0
#define T_DOMAIN  1
#define T_ENABLED 2
#define DATABASE_NAME "moefel.db"

static GtkTreeStore* store;
static GtkWidget* search_btn;
static GtkWidget* search_inp;
static GtkWidget* menu_item_add;
static GtkWidget* menu_item_new;
static GtkWidget* menu_item_search;


void
gui_repositories_populate_store (GtkTreeStore *store, dt_list* repositories, bool cleanup)
{
  dt_list* temp = repositories;
  while (temp != NULL)
    {
      dt_repository* repo = temp->data;
      GtkTreeIter i;
      gtk_tree_store_append (store, &i, NULL);
      gtk_tree_store_set (store, &i, 
			  T_NAME, repo->name,
			  T_DOMAIN, repo->domain,
			  T_ENABLED, repo->is_enabled, -1);      

      if (cleanup) dt_repository_free (repo);
      temp = temp->next;
    }

  if (cleanup) dt_list_free (repositories);
}

void
gui_repositories_handle_search (GtkWidget* widget, void* data)
{
  GtkEntry* input = (GtkEntry*)data;
  const char* keyword = gtk_entry_get_text (input);

  /* Handle "search" action.
   * ------------------------------------------------------------------------ */
  const char* label = gtk_button_get_label (GTK_BUTTON (search_btn));
  if (!strcmp (label, "Search"))
    {
      dt_list* dataset = NULL;
      if (db_repositories_get_by_keyword (DATABASE_NAME, keyword, &dataset))
	gtk_tree_store_clear (store),
	gui_repositories_populate_store (store, dataset, TRUE);
    }

  /* Handle "add" action.
   * ------------------------------------------------------------------------ */
  else if (!strcmp (label, "Add"))
    {
      if (strcmp (keyword, "") && h_command_enable (keyword))
	{
	  GdkRGBA bg;
	  gdk_rgba_parse (&bg, "#cdde87");
	  gtk_widget_override_background_color (search_inp, GTK_STATE_FLAG_NORMAL, &bg);
	}

      dt_list* dataset = NULL;
      if (db_repositories_get_all (DATABASE_NAME, &dataset))
	gtk_tree_store_clear (store),
	gui_repositories_populate_store (store, dataset, TRUE);
    }
}

GtkWidget*
gui_repositories_tab_create ()
{
  /* Widget initalization
   * ------------------------------------------------------------------------ */
  search_inp = gtk_entry_new ();
  search_btn = gtk_button_new_with_label ("Search");
  GtkWidget* vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  GtkWidget* top_bar = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  GtkWidget* repo_bar = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);

  GtkWidget* menu = gtk_menu_new ();
  GtkWidget* menu_btn = gtk_menu_button_new ();
  menu_item_add = gtk_menu_item_new_with_label ("Add");
  menu_item_new = gtk_menu_item_new_with_label ("Create");
  menu_item_search = gtk_menu_item_new_with_label ("Search");

  /* Construct the table.
   * ------------------------------------------------------------------------ */
  store = gtk_tree_store_new (3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN);

  dt_list* dataset = NULL;
  if (db_repositories_get_all (DATABASE_NAME, &dataset))
    gui_repositories_populate_store (store, dataset, TRUE);

  GtkWidget* repo_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
  g_object_set (G_OBJECT (repo_view), "rules-hint", TRUE, NULL);

  GtkCellRenderer* renderer;
  GtkTreeViewColumn* column;

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Name", renderer, "text", T_NAME, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (repo_view), column);
  g_object_set (G_OBJECT (renderer), "width", 350, NULL);
  g_object_set (G_OBJECT (renderer), "height", 25, NULL);

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Domain", renderer, "text", T_DOMAIN, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (repo_view), column);

  renderer = gtk_cell_renderer_toggle_new ();
  column = gtk_tree_view_column_new_with_attributes ("Active", renderer, "active", T_ENABLED, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (repo_view), column);

  /* Further setup
   * ------------------------------------------------------------------------ */
  gtk_widget_set_size_request (search_inp, 0, 32);
  gtk_widget_set_size_request (search_btn, 0, 32);
  gtk_widget_set_margin_start (search_inp, 10);

  gtk_menu_button_set_direction (GTK_MENU_BUTTON (menu_btn), GTK_ARROW_DOWN);
  gtk_menu_button_set_popup (GTK_MENU_BUTTON (menu_btn), menu);
  gtk_menu_attach (GTK_MENU (menu), menu_item_search, 0, 1, 0, 1);
  gtk_menu_attach (GTK_MENU (menu), menu_item_add, 0, 1, 1, 2);
  gtk_menu_attach (GTK_MENU (menu), menu_item_new, 0, 1, 2, 3);

  gtk_widget_show (GTK_WIDGET (menu_item_add));
  gtk_widget_show (GTK_WIDGET (menu_item_new));

  /* Containers
   * ------------------------------------------------------------------------ */
  gtk_box_pack_start (GTK_BOX (top_bar), search_inp, 1, 1, 0);
  gtk_box_pack_start (GTK_BOX (top_bar), search_btn, 0, 0, 0);
  gtk_box_pack_end (GTK_BOX (top_bar), menu_btn, 0, 0, 10);
  gtk_box_pack_start (GTK_BOX (vbox), top_bar, 0, 0, 10);
  gtk_box_pack_start (GTK_BOX (repo_bar), repo_view, 1, 1, 10);
  gtk_box_pack_end (GTK_BOX (vbox), repo_bar, 1, 1, 10);

  /* Signal handlers
   * ------------------------------------------------------------------------ */
  g_signal_connect (G_OBJECT (search_btn), "clicked",
		    G_CALLBACK (gui_repositories_handle_search), search_inp);

  g_signal_connect (G_OBJECT (search_inp), "activate",
		    G_CALLBACK (gui_repositories_handle_search), search_inp);

  g_signal_connect (G_OBJECT (menu_item_add), "activate",
		    G_CALLBACK (gui_repositories_tab_change), menu_item_add);

  g_signal_connect (G_OBJECT (menu_item_search), "activate",
		    G_CALLBACK (gui_repositories_tab_change), menu_item_search);

  g_signal_connect (G_OBJECT (vbox), "destroy",
		    G_CALLBACK (gui_repositories_tab_quit), NULL);

  return vbox;
}

void
gui_repositories_tab_change (GtkWidget* widget, void* data)
{
  GtkMenuItem* menu_item = (GtkMenuItem*)data;
  const char* label = gtk_button_get_label (GTK_BUTTON (search_btn));
  const char* active_menu = gtk_menu_item_get_label (GTK_MENU_ITEM (menu_item));

  /* Change the button's label text. */
  gtk_button_set_label (GTK_BUTTON (search_btn), active_menu);

  /* Make sure the right menu items are displayed. */
  gtk_widget_show (menu_item_search);
  gtk_widget_show (menu_item_add);
  gtk_widget_show (menu_item_new);
  gtk_widget_hide (GTK_WIDGET (menu_item));
}

void
gui_repositories_tab_quit ()
{
  gtk_tree_store_clear (store);  
}
