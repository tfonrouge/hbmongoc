/*
 * test: bson_test01
 *
 *
 */

#include "hbmongoc.ch"

#define coll_name "bios_test"

#define numRegs 100000

PROCEDURE main()
    LOCAL client
    LOCAL database
    LOCAL collection
    LOCAL filter
    LOCAL opts
    LOCAL cursor
    LOCAL doc
    LOCAL i
    LOCAL millis

    /* REQUIRED to initialize mongoc internals */
    mongoc_init()

    hb_mongoc_set_return_bson_value_type("BSON")
//    hb_mongoc_set_return_bson_value_type("JSON")
//    hb_mongoc_set_return_bson_value_type("HASH")

    client := mongoc_client_new( "mongodb://localhost" )

    database := mongoc_client_get_database( client, "hbmongoc" );

    collection := mongoc_database_get_collection( database, coll_name )

    build_collection( collection )

    CLS

    opts := bson_new()

    WAIT "press any key to display result set..."

    millis := hb_milliSeconds()

    FOR i := 1 TO numRegs

        filter := bson_new()
        BSON_APPEND_INT32( filter, "_id", hb_randomInt( 10 ) )

        ? i, "Find:", hb_bson_as_json( filter ), "->", ""

        cursor := mongoc_collection_find_with_opts( collection, filter, opts )

        IF mongoc_cursor_next( cursor, @doc )

            ?? "FOUND:"

            SWITCH hb_mongoc_set_return_bson_value_type()
            CASE "BSON"
                ? hb_bson_as_json( doc )
                EXIT
            CASE "JSON"
                ? doc
                EXIT
            CASE "HASH"
                ? i, doc["_id"], doc["name"]["first"], doc["name"]["last"]
                ?
                EXIT
            ENDSWITCH

        ELSE
            ?? "NOT FOUND"
        ENDIF

    NEXT

    ? "finded", hb_ntos( numRegs ), " records in ", hb_ntos( hb_milliSeconds() - millis ), "milliseconds"

    /* REQUIRED to cleanup mongoc internals */
    mongoc_cleanup()

    WAIT

RETURN

STATIC PROCEDURE build_collection( collection )

    /* drop collection */
    mongoc_collection_drop( collection )

    /* using mongoimport tool to import documents at "bios.json" */
    hb_run(e"mongoimport --quiet -h=\"localhost\" -d=\"hbmongoc\" -c=\"" + coll_name + e"\" --file=bios.json")

RETURN
