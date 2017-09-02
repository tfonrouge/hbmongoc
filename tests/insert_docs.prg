
#include "hbmongoc.ch"

PROCEDURE main( uri )
    LOCAL client
    LOCAL collection
    LOCAL command
    LOCAL reply
    LOCAL error
    LOCAL bson

    CLS

    IF empty( uri )
        uri := "mongodb://localhost"
    ENDIF

    ? "Server:", uri

    client := mongoc_client_new( uri )

    command := { "ping" => 1 }

    ? "sending ping to server.."

    IF ! mongoc_client_command_simple( client, "admin", command, nil, @reply )
	alert("Error sending command to server...")
	QUIT
    ENDIF

    ? "Server response:", bson_as_json( reply )

    collection := mongoc_client_get_collection( client, "hbmongoc", "coll_test" )

    bson := bson_new()

    BSON_APPEND_UTF8( bson, "Name", "John" )

    mongoc_collection_insert( collection, MONGOC_INSERT_NONE, bson, nil, @error )

    WAIT

RETURN
