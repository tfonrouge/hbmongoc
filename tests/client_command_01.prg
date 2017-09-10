
#include "hbmongoc.ch"

PROCEDURE main( uri )
    LOCAL client
    LOCAL command
    LOCAL reply
    LOCAL error
    LOCAL i

    /* REQUIRED to initialize mongoc internals */
    mongoc_init()

    hb_mongoc_set_return_bson_value_type("JSON")

    CLS

    IF empty( uri )
        uri := "mongodb://localhost"
    ENDIF

    ? "Server:", uri

    client := mongoc_client_new( uri )

    /* bson document can be represented as a Harbour hash */
    command := { "ping" => 1 }

    WAIT "press any key to ping server..."

    ?

    FOR i := 1 TO 10
        ? "ping", hb_nToS( i ), ": "

        IF mongoc_client_command_simple( client, "admin", command, nil, @reply, @error )
            ?? "response:", reply
        ELSE
            ?? "error:", HB_BSON_ERROR_MESSAGE( error )
        ENDIF

        hb_idleSleep( 1 )

    NEXT

    ?

    WAIT "press any key to finish."

    /* REQUIRED to cleanup mongoc internals */
    mongoc_cleanup()

RETURN
