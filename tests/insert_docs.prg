
#include "hbmongoc.ch"

PROCEDURE main( uri )
    LOCAL client
    LOCAL collection
    LOCAL error
    LOCAL bson

    /* REQUIRED to initialize mongoc internals */
    mongoc_init()

    CLS

    IF empty( uri )
        uri := "mongodb://localhost"
    ENDIF

    ? "Server:", uri

    client := mongoc_client_new( uri )

    WAIT "press any key to insert document..."

    collection := mongoc_client_get_collection( client, "hbmongoc", "coll_test" )

    bson := bson_new()

    BSON_APPEND_UTF8( bson, "Name", "John" )
    BSON_APPEND_DATE_TIME( bson, "datetime", hb_dateTime() )

    ?

    IF ! mongoc_collection_insert( collection, MONGOC_INSERT_NONE, bson, nil, @error )
        ? "insert error:", HBBSON_ERROR_MESSAGE( error )
    ELSE
        ? "document inserted ok."
    ENDIF

    ?

    WAIT "press any key to finish."

    /* REQUIRED to cleanup mongoc internals */
    mongoc_cleanup()

RETURN
