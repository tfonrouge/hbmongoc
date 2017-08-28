/*
 * hbmongoc
 * execute command example : get stats from collection
 */

PROCEDURE main()
    LOCAL client
    LOCAL collection
    LOCAL command
    LOCAL reply
    LOCAL error

    mongoc_init()

    client := mongoc_client_new ( "mongodb://localhost:27017/?appname=executing-example" )
    collection := mongoc_client_get_collection( client, "db_name", "coll_name" )

    command := hb_jsonEncode( {"collStats" => "coll_name"} )

    IF mongoc_collection_command_simple ( collection, command, nil, @reply, @error )
        ? "Server reply:", reply
        ? "bson_as_canonical_extended_json( reply, nil ):", bson_as_canonical_extended_json( reply, nil )
    ELSE
        ? "Failed to run command:", error
    ENDIF

    WAIT

    mongoc_cleanup()

RETURN
