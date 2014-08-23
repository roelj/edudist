#include "command.h"

#define DATABASE_NAME "moefel.db"

int h_command_enable (const char* domain)
{
  curl_global_init (CURL_GLOBAL_ALL);

  /* Get the name and packages from the remote host. */
  dt_http_response* response;
  response = net_http_get ("http", domain, "moefel.repo", 80, NULL);

  curl_global_cleanup();

  /* Only when the status code is 200 we should process the entry. */
  if (response->status != 200)
    {
      dt_http_response_free (response);
      return E_NETWORK;
    }

  /* Parse the gathered information and store it in a 'dt_repository'. */
  dt_repository* repo;
  if (p_repository_from_buffer (&repo, response->body) == 0) return E_PARSER;

  /* Clean up the memory allocated for the HTTP response. */
  dt_http_response_free (response);

  /* Set the domain name. */
  repo->domain = calloc (1, strlen (domain) + 1);
  repo->domain = strcpy (repo->domain, domain);

  /* Set the current local time as 'created_at'. */
  repo->created_at = m_current_timestamp();

  /* Add the repository to the database. */
  if (!db_repositories_add (DATABASE_NAME, repo))
    {
      dt_repository_free (repo);
      return E_DATABASE;
    }

  /* Clean up the dynamically allocated memory. */
  dt_repository_free (repo);

  return 1;
}

int
h_command_update ()
{
  curl_global_init (CURL_GLOBAL_ALL);
  dt_list* repos = NULL;
  if (!db_repositories_get_all (DATABASE_NAME, &repos)) return E_DATABASE;

  dt_list* r = repos;
  while (r != NULL)
    {
      dt_repository* repo = r->data;
      r = r->next;

      /* Skip disabled repositories. */
      if (repo->is_enabled == 0) 
	{
	  dt_repository_free (repo);
	  continue;
	}

      /* Get the name and packages from the remote host. */
      dt_http_response* response;
      response = net_http_get ("http", repo->domain, "moefel.repo", 80, NULL);

      if (response->status == 200)
	{
	  dt_repository* content;
	  p_repository_from_buffer (&content, response->body);
	  content->domain = calloc (1, strlen (repo->domain) + 1);
	  content->domain = strcpy (content->domain, repo->domain);
	  db_repositories_add (DATABASE_NAME, content);
	  dt_repository_free (content);
	}

      /* Clean up and move on. */
      dt_http_response_free (response);
      dt_repository_free (repo);
    }

  dt_list_free (repos);
  curl_global_cleanup();

  return 1;
}

int h_command_get (const char* package, const char* repo)
{
  curl_global_init (CURL_GLOBAL_ALL);

  /* Get the package data from the local database. */
  dt_package* pkg;
  if (db_packages_get_by_name (DATABASE_NAME, package, repo,  &pkg)
      && pkg != NULL)
    {
      puts ("Downloading package...");

      char pkg_full_name[5 + strlen (pkg->name)];
      strcpy ((char *)pkg_full_name, pkg->name);
      strcat ((char *)pkg_full_name, ".pkg");

      /* Download the package. */
      if (net_http_get_to_file ("http", pkg->domain, pkg->location, 80, (char *)pkg_full_name))
	{
	  char* output;
	  int file_size;
	  char checksum[65];
	  if (pkg->checksum != NULL
	      && m_file_to_buffer ((char *)pkg_full_name, &file_size, &output) == 1
	      && m_buffer_sha256 (output, file_size, checksum) == 1)
	    {
	      if (strcmp (pkg->checksum, checksum))
		{
		  puts ("Checksums don't match!\nWhat you've downloaded is"
			" not the same as what the provider intended to"
			" send.\n");
		  return 1;
		}
	      free (output);
	    }
	  else
	    {
	      printf ("Could not check whether the data you've downloaded "
		      "is\nthe same as the publisher intended to send.\n"
		      "\nDo you want to continue? [y/N]: ");

	      char answer = getchar ();
	      if (answer != 'y' && answer != 'Y')
		return 1;
	    }

	  /* Extract it into the library. */
	  puts ("Unpacking package...");

	  char* repo_root = NULL;
	  if (!fs_create_repository_path (DATABASE_NAME, pkg->domain, &repo_root))
	    {
	      puts ("Couldn't create local directories.");
	      free (repo_root);
	      return 1;
	    }

	  if (packagers_zip_unpack ((char *)pkg_full_name, repo_root))
	    {
	      unlink ((char *)pkg_full_name);

	      db_packages_set_local (DATABASE_NAME, pkg->name, 1);
	      puts ("Done!");
	    }
	  else
	    puts ("There was an error while unpacking the package.");

	  free (repo_root);
	}
      else
	puts ("There was an error while downloading the package.");
    }
  else
    puts ("There was an error while processing package data.");

  /* TODO: Add a size field to the packages table to be able to show a
   * progress bar. */

  dt_package_free (pkg);
  curl_global_cleanup();

  return 1;
}
