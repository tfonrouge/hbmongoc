/*
 * hbmongoc
 * execute command example : get stats from collection
 */

#include "hbmongoc.ch"

PROCEDURE main()
    LOCAL client
    LOCAL collection
    LOCAL command
    LOCAL reply
    LOCAL error

    LOCAL v1, v2, v3

    hb_bson_version( @v1, @v2, @v3 )
    ? hb_ntos( v1 ) + "." + hb_ntos( v2 ) + "." + hb_ntos( v3 )

    /* REQUIRED to initialize mongoc internals */
    mongoc_init()

    // uncomment following line to return bson_t type from hbmongoc funcs
    hb_mongoc_set_return_bson_value_type("BSON")

    client := mongoc_client_new ( "mongodb://localhost:27017/?appname=executing-example" )
    collection := mongoc_client_get_collection( client, "db_name", "coll_name" )

    command := hb_jsonEncode( {"collStats" => "coll_name"} )

    IF mongoc_collection_command_simple ( collection, command, nil, @reply, @error )
        ? "Server reply:", reply
        IF hb_mongoc_set_return_bson_value_type() == "BSON"
            ? "bson_as_json( reply, nil ):", bson_as_json( reply, nil )
            ? "bson_as_canonical_extended_json( reply, nil ):", bson_as_canonical_extended_json( reply, nil )
        ENDIF
    ELSE
        ? "Failed to run command:", HBBSON_ERROR_MESSAGE( error )
    ENDIF

    WAIT

    /* REQUIRED to cleanup mongoc internals */
    mongoc_cleanup()

RETURN
