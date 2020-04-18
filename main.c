#include <stdio.h>
#include <mongoc.h>

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

   mongoc_init ();
   client = mongoc_client_new (uri_str);
   mongoc_client_set_appname (client, "connect-example");
   database = mongoc_client_get_database (client, "db_name");
   collection = mongoc_client_get_collection (client, "db_name", "coll_name");

   mongoc_cursor_t *cursor;
   query = (bson_t *)malloc(1024); //XXX
   retval = bson_init_from_json(query,
                    "{ \"a\": { \"$in\": [0, 1, 2 ] } }",
                    -1,
                    NULL);
   cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
   while (mongoc_cursor_next (cursor, &doc)) {
      str = bson_as_canonical_extended_json (doc, NULL);
      printf ("%s\n", str);
      bson_free (str);
   }

   bson_destroy (query);
   mongoc_cursor_destroy (cursor);
   mongoc_collection_destroy (collection);
   mongoc_database_destroy (database);
   mongoc_client_destroy (client);
   mongoc_cleanup ();

   return 0;
}
