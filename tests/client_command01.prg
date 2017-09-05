
#include "hbmongoc.ch"

PROCEDURE main( uri )
    LOCAL client
    LOCAL command
    LOCAL reply
    LOCAL i

    /* REQUIRED to initialize mongoc internals */
    mongoc_init()

    hbmongoc_set_return_bson_value_type("HASH")

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
        ? "sending ping to server...", i, ""

        IF ! mongoc_client_command_simple( client, "admin", command, nil, @reply )
            alert("Error sending command to server...")
            QUIT
        ENDIF

        AltD()
        ?? "Server response:", bson_as_json( reply )

        hb_idleSleep( 1 )

    NEXT

    ?

    WAIT "press any key to finish."

    /* REQUIRED to cleanup mongoc internals */
    mongoc_cleanup()

RETURN
