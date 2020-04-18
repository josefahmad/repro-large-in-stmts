#include <stdio.h>
#include <mongoc.h>

#define IN_ENTRIES 10000

static char str_val[7];
static char str_query[IN_ENTRIES * 10];

int
main (int argc, char *argv[])
{
   const char *uri_str = "mongodb://localhost:27017";
   mongoc_client_t *client;
   mongoc_database_t *database;
   mongoc_collection_t *collection;
   bson_t *command, reply, *insert;
   bson_error_t error;
   char *str;
   bool retval;
   bson_t *query;
   const bson_t *doc;

  int i;
  sprintf(str_query, "{ \"a\": { \"$in\": [");
  for (i = 0; i < IN_ENTRIES; i++) {
    sprintf(str_val, " %d ",  i);
    strcat(str_query, str_val);
    if (i != IN_ENTRIES - 1) {
      strcat(str_query, ","); 
    }
  }
  strcat(str_query, "] } }");

//  printf("%s\n", str_query);

   mongoc_init ();
   client = mongoc_client_new (uri_str);
   mongoc_client_set_appname (client, "connect-example");
   database = mongoc_client_get_database (client, "db_name");
   collection = mongoc_client_get_collection (client, "db_name", "coll_name");

   mongoc_cursor_t *cursor;
   query = bson_new_from_json(str_query,
                    -1,
                    NULL);
   cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
   mongoc_cursor_next (cursor, &doc);
 //  while (mongoc_cursor_next (cursor, &doc)) {
 //     str = bson_as_canonical_extended_json (doc, NULL);
 //     printf ("%s\n", str);
 //     bson_free (str);
 //  }

   bson_destroy (query);
   mongoc_cursor_destroy (cursor);
   mongoc_collection_destroy (collection);
   mongoc_database_destroy (database);
   mongoc_client_destroy (client);
   mongoc_cleanup ();

   return 0;
}
