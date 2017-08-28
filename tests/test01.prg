/*
 * hbmongoc : test
 * connect to server at localhost:27017
 * set app name
 * get database
 * get collection
 * ping to server (using mongoc_client_command_simple)
 * retrieve reply from last command sended to server
 * insert documents to collection
 */

#include "hbmongoc.ch"

#define MONGOC_BSON_RETURN_TYPE 1

PROCEDURE main()
    LOCAL client
    LOCAL database
    LOCAL collection
    LOCAL command
    LOCAL retval
    LOCAL error
    LOCAL reply
    LOCAL insert
    LOCAL i

    mongoc_init()

#if MONGOC_BSON_RETURN_TYPE
    hbmongoc_setReturnValueType("bson")
#endif

    client := mongoc_client_new("mongodb://localhost:27017")

    ? "mongoc_client_new():", client

    ? "mongoc_client_set_appname():", mongoc_client_set_appname(client, "connect-example")

    database := mongoc_client_get_database(client, "db_name")

    ? "mongoc_client_get_database():", database

    ? "mongoc_client_get_collection():", collection := mongoc_client_get_collection(client, "db_name", "coll_name")

    //command := e"{\"ping\":1}"
    command := hb_jsonEncode( { "ping" => 1 } )

    ? "mongoc_client_command_simple():", retval := mongoc_client_command_simple( client, "admin", command, nil, @reply )

    IF ! retval
        ? "error"
    ENDIF

#if MONGOC_BSON_RETURN_TYPE
    ? "reply: ", reply, bson_as_json( reply )
#else
    ? "reply: ", reply
#endif

    FOR i := 1 TO 10

        insert := hb_jsonEncode( { "hello" => "world", "year" => year( date() ), "time" => time(), "seconds" => seconds() } )

        IF ! mongoc_collection_insert( collection, MONGOC_INSERT_NONE, insert, nil, @error )
            ? "Error:", error
        ELSE
            ? "document inserted..."
        ENDIF

    NEXT

    WAIT

    mongoc_cleanup()

RETURN
