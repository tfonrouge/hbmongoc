/*
 * stress test : memory consumption
 *
 * creates a bson_uri_t value
 * creates a bson_client_t value from bson_uri_t
 * ping server 1000000 times
 *
 */

#include "hbmongoc.ch"

PROCEDURE main()
    LOCAL uri
    LOCAL client
    LOCAL uriString
    LOCAL commandPing
    LOCAL retVal
    LOCAL reply
    LOCAL error
    LOCAL i

    /* REQUIRED to initialize mongoc internals */
    mongoc_init()

    CLS

    uriString := "mongodb://localhost:27017"

    ? "Uri string:", uriString

    uri := mongoc_uri_new( uriString )

    IF uri = nil
        alert( "URI not valid...")
        QUIT
    ENDIF

    client := mongoc_client_new_from_uri( uri )

    WAIT "Press any key to start ping test..."

//    commandPing := { "ping" => 1 }
//    commandPing := hb_jsonEncode( { "ping" => 1 } )

    FOR i := 1 TO 1000000

        commandPing := bson_new()
        BSON_APPEND_INT32( commandPing, "ping", 1 )

        retVal := mongoc_client_command_simple( client, "admin", commandPing, nil, @reply, @error )

        IF retVal
            ? "Server reply:", i, hb_bson_as_json( reply )
        ELSE
            ? "Server error:", HB_BSON_ERROR_MESSAGE( error )
            EXIT
        ENDIF

    NEXT

    WAIT

    /* REQUIRED to cleanup mongoc internals */
    mongoc_cleanup()

RETURN
