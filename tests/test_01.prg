/*
 * stress test : memory consumption
 *
 * connect to server at localhost:27017
 * set app name
 * get database
 * get collection
 * ping to server (using mongoc_client_command_simple)
 * retrieve reply from last command sended to server
 * insert documents to collection
 */

#include "hbmongoc.ch"

#define NUM_REGS    1000000

PROCEDURE main( serverConn )
    LOCAL client
    LOCAL database
    LOCAL collection
    LOCAL command
    LOCAL retval
    LOCAL error
    LOCAL reply
    LOCAL insert
    LOCAL name
//    LOCAL lastMiddle
    LOCAL i
    LOCAL bool
    LOCAL secs
    LOCAL dec
    LOCAL array,item1, item2
    LOCAL printOne := .t.

    /* REQUIRED to initialize mongoc internals */
    mongoc_init()

    CLS

    // uncomment following line to return by ref bson_t type as a BSON (native libbson bson_t ) from hbmongoc funcs
    // hb_mongoc_set_return_bson_value_type("BSON") // DEFAULT

    // uncomment following line to return by ref bson_t type as a JSON (Harbour string) from hbmongoc funcs
    // hb_mongoc_set_return_bson_value_type("JSON")

    // uncomment following line to return by ref bson_t type as a HASH (Harbour hash) from hbmongoc funcs
    // hb_mongoc_set_return_bson_value_type("HASH")

    IF serverConn = nil
        serverConn := "mongodb://localhost:27017"
    ENDIF

    client := mongoc_client_new( serverConn )

    ? "Server:", serverConn

    ? "mongoc_client_new():", client

    IF BSON_CHECK_VERSION( 1, 5, 0 )
        ? "mongoc_client_set_appname():", mongoc_client_set_appname(client, "connect-example")
    ENDIF

    database := mongoc_client_get_database(client, "db_name")

    ? "mongoc_client_get_database():", database

    ? "mongoc_client_get_collection():", collection := mongoc_client_get_collection(client, "db_name", "coll_name")

    //command := e"{\"ping\":1}"
    command := { "ping" => 1 }

    ? "mongoc_client_command_simple():", retval := mongoc_client_command_simple( client, "admin", command, nil, @reply )

    IF ! retval
        ? "error"
    ENDIF

    altD()

    SWITCH hb_mongoc_set_return_bson_value_type()
    CASE "BSON"
        ? "reply: ", reply, bson_as_json( reply )
        EXIT
    CASE "JSON"
        ? "reply: ", reply, reply
        EXIT
    CASE "HASH"
        ? "reply: ", reply, hb_jsonEncode( reply )
        EXIT
    ENDSWITCH

    bool := .T.

    WAIT

    ?
    ? "Inserting " + hb_ntos( NUM_REGS ) + " documents..."

    secs := seconds()

    FOR i := 1 TO NUM_REGS

        insert := bson_new()

        BSON_APPEND_UTF8( insert, "time", time() )
        BSON_APPEND_DATE_TIME( insert, "dateTime", hb_dateTime() )
        BSON_APPEND_BOOL( insert, "bool", bool := ! bool )
        BSON_APPEND_DOUBLE( insert, "seconds", seconds() )
        BSON_APPEND_INT32( insert, "int32", 2 ^ 31 - 1 )
        BSON_APPEND_INT64( insert, "int64", 2 ^ 62 - 1 )
        BSON_APPEND_BINARY( insert, "binary", e"\000\001\0020123456789" )
        BSON_APPEND_NULL( insert, "null" )
        BSON_APPEND_REGEX( insert, "regex", "\bJohn\b", "i" )

        IF BSON_CHECK_VERSION( 1, 5, 0 )
            bson_decimal128_from_string( "100.00", @dec )
            BSON_APPEND_DECIMAL128( insert, "decimal128", dec )
        ENDIF

        BSON_APPEND_CODE( insert, "javascript", e"return \"any javaScript code\"" )

        BSON_APPEND_ARRAY( insert, "array1", {1,2,3,4} )
        BSON_APPEND_ARRAY( insert, "array2", {"one","two"} )

        item1 := bson_new()
        BSON_APPEND_UTF8( item1, "name", "John" )
        BSON_APPEND_UTF8( item1, "age", "41" )
        BSON_APPEND_UTF8( item1, "genre", "male" )

        item2 := bson_new()
        BSON_APPEND_UTF8( item2, "name", "Mary" )
        BSON_APPEND_UTF8( item2, "age", "31" )
        BSON_APPEND_UTF8( item2, "genre", "female" )

        array = bson_new()
        BSON_APPEND_DOCUMENT( array, "0", item1 )   /* key MUST start with "0" */
        BSON_APPEND_DOCUMENT( array, "1", item2 )   /* ..."n" */
        BSON_APPEND_ARRAY( insert, "array3", array )

        BSON_APPEND_DOCUMENT_BEGIN( insert, "name", @name )
            BSON_APPEND_UTF8( name, "first", "Juana" )
            BSON_APPEND_UTF8( name, "last", "La Cubana" )
        bson_append_document_end( insert, @name )

        /*
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
         */

        IF printOne
            ? e"BSON_AS_JSON:\n", bson_as_json( insert )
            ? e"BSON_AS_CANONICAL_EXTENDED_JSON:\n", bson_as_canonical_extended_json( insert )
            printOne := .F.
        ENDIF

        BSON_APPEND_DOCUMENT( insert, "childs",  {"1"=>"Arel","2"=>"Moises","3"=>"Israel","4"=>"Avril","5"=>"Lucy","6"=>"Nathalie","7"=>"Michal" } )

        IF ! mongoc_collection_insert( collection, MONGOC_INSERT_NONE, insert, nil, @error )
            ? e"\nError:", HBBSON_ERROR_MESSAGE( error )
        ENDIF

    NEXT

    ?
    ? seconds() - secs, "seconds"
    ?

    WAIT

    /* REQUIRED to cleanup mongoc internals */
    mongoc_cleanup()

RETURN
