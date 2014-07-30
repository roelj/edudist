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
