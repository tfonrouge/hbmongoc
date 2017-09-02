/*
 * test: client_connection
 */

#include "hbmongoc.ch"

PROCEDURE main()
    LOCAL uri
    LOCAL client

    CLS

    mongoc_init()

    uri := mongoc_uri_new( "mongodb://localhost:27017" )

    IF uri = nil
        alert( "URI not valid...")
        QUIT
    ENDIF

    client := mongoc_client_new_from_uri( uri )

    ? client

    WAIT

    mongoc_cleanup()

RETURN
