/*
 * hbmongoc : test
 * connect to server at localhost:27017
 * set app name
 * get database
 * get collection
 * ping to server (using mongoc_client_command_simple)
 * retrieve reply from last command sended to server
 * insert documents to collection
 */

#include "hbmongoc.ch"

#define NUM_REGS    10

PROCEDURE main()
    LOCAL client
    LOCAL database
    LOCAL collection
    LOCAL command
    LOCAL retval
    LOCAL error
    LOCAL reply
    LOCAL insert
    LOCAL name
    LOCAL lastMiddle
    LOCAL i
    LOCAL bool
    LOCAL secs

    CLS

    mongoc_init()

    // uncomment following line to return bson_t type from hbmongoc funcs
    hbmongoc_setReturnValueType("BSON")

    client := mongoc_client_new("mongodb://localhost:27017")

    ? "mongoc_client_new():", client

    ? "mongoc_client_set_appname():", mongoc_client_set_appname(client, "connect-example")

    database := mongoc_client_get_database(client, "db_name")

    ? "mongoc_client_get_database():", database

    ? "mongoc_client_get_collection():", collection := mongoc_client_get_collection(client, "db_name", "coll_name")

    //command := e"{\"ping\":1}"
    command := hb_jsonEncode( { "ping" => 1 } )

    ? "mongoc_client_command_simple():", retval := mongoc_client_command_simple( client, "admin", command, nil, @reply )

    IF ! retval
        ? "error"
    ENDIF

    IF hbmongoc_setReturnValueType() == "BSON"
        ? "reply: ", reply, bson_as_json( reply )
    ENDIF

    IF hbmongoc_setReturnValueType() == "JSON"
        ? "reply: ", reply
    ENDIF

    bool := .T.

    WAIT

    ?
    ? "Inserting " + hb_ntos( NUM_REGS ) + " documents..."

    secs := seconds()

    FOR i := 1 TO NUM_REGS

        insert = bson_new()

        BSON_APPEND_UTF8( insert, "time", time() )
        BSON_APPEND_DATE_TIME( insert, "dateTime", hb_dateTime() )
        BSON_APPEND_BOOL( insert, "bool", bool := ! bool )
        BSON_APPEND_DOUBLE( insert, "seconds", seconds() )
        BSON_APPEND_INT32( insert, "int32", 2 ^ 31 - 1 )
        BSON_APPEND_INT64( insert, "int64", 2 ^ 62 - 1 )
        BSON_APPEND_BINARY( insert, "binary", e"\000\001\0020123456789" )

        IF BSON_APPEND_DOCUMENT_BEGIN( insert, "name", @name )
            BSON_APPEND_UTF8( name, "first", "Juana" )
            IF BSON_APPEND_DOCUMENT_BEGIN( name, "lastMiddle", @lastMiddle )
                BSON_APPEND_UTF8( lastMiddle, "last", "La Cubana" )
                BSON_APPEND_UTF8( lastMiddle, "middle", "Navajas" )
                bson_append_document_end( name, lastMiddle )
            ENDIF
            IF BSON_APPEND_DOCUMENT_BEGIN( name, "parents", @lastMiddle )
                BSON_APPEND_UTF8( lastMiddle, "father", "M" )
                BSON_APPEND_UTF8( lastMiddle, "mother", "F" )
                bson_append_document_end( name, lastMiddle )
            ENDIF
            bson_append_document_end( insert, name )
        ENDIF

        BSON_APPEND_DOCUMENT( insert, "childs",  hb_jsonEncode( {"1"=>"Arel","2"=>"Moises","3"=>"Israel","4"=>"Avril","5"=>"Lucy","6"=>"Nathalie","7"=>"Michal" } ) )

        IF ! mongoc_collection_insert( collection, MONGOC_INSERT_NONE, insert, nil, @error )
//            ? "Error: " + error
        ELSE
//            ? "#", i
        ENDIF

//        bson_destroy( insert )
//        bson_destroy( name )
//        insert := nil
//        child := nil

//        inkey( 0 )

    NEXT

    ?? seconds() - secs, "seconds"
    ?

    WAIT

    mongoc_cleanup()

RETURN
