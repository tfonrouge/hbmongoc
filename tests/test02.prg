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

    // uncomment following line to return bson_t type from hbmongoc funcs
    hbmongoc_setReturnValueType("BSON")

    client := mongoc_client_new ( "mongodb://localhost:27017/?appname=executing-example" )
    collection := mongoc_client_get_collection( client, "db_name", "coll_name" )

    command := hb_jsonEncode( {"collStats" => "coll_name"} )

    IF mongoc_collection_command_simple ( collection, command, nil, @reply, @error )
        ? "Server reply:", reply
        IF hbmongoc_setReturnValueType() == "BSON"
            ? "bson_as_json( reply, nil ):", bson_as_json( reply, nil )
#if BSON_CHECK_VERSION( 1, 7, 0 )
            ? "bson_as_canonical_extended_json( reply, nil ):", bson_as_canonical_extended_json( reply, nil )
#endif
        ENDIF
    ELSE
        ? "Failed to run command:", error
    ENDIF

    WAIT

    mongoc_cleanup()

RETURN
