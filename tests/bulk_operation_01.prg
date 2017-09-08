/*
 * import dbf example
 */

#include "hbmongoc.ch"
#include "dbstruct.ch"

#define db_name     "hbmongoc"
#define coll_name   "test.dbf"

PROCEDURE main( uriString )
    LOCAL client
    LOCAL database
    LOCAL arrayCollections
    LOCAL collection
    LOCAL error
    LOCAL millis
    LOCAL filter

    /* REQUIRED to initialize mongoc internals */
    mongoc_init()

    CLS

    WAIT "press any key to start..."

    USE test

    IF neterr()
        ? "error opening test.dbf"
        QUIT
    ENDIF

    IF empty( uriString )
        uriString := "localhost"
    ENDIF

    uriString := "mongodb://" + uriString

    ? "Server:", uriString

    client := mongoc_client_new( uriString )
    database := mongoc_client_get_database( client, db_name )
    collection := mongoc_database_get_collection( database, coll_name )

    /* drops collection (if exists) */
    arrayCollections := mongoc_database_get_collection_names( database, @error )
    IF error = nil
        IF aScan( arrayCollections, coll_name ) > 0
            ? "Drop existing collection", coll_name, ":", mongoc_collection_drop( collection, @error )
            IF error != nil
                ? "Error:", HB_BSON_ERROR_MESSAGE( error )
            ENDIF
        ENDIF
    ELSE
        ? "Error:", HB_BSON_ERROR_MESSAGE( error )
    ENDIF

    millis := hb_milliSeconds()
    importDocs( collection )

    ? hb_nToS( TEST->( recCount() ) ), "records imported in", hb_nToS( hb_milliSeconds() - millis ), "milliseconds"

    WAIT

    filter := bson_new()
    WAIT "press any key to show ALL docs in collection... filter: " + bson_as_json( filter )

    displayDocs( collection, filter )

    WAIT

    filter := bson_new()
    BSON_APPEND_UTF8( filter, "FIRST", "John" )
    WAIT "press any key to show docs in collection with filter: " + bson_as_json( filter )

    displayDocs( collection, filter )

    WAIT "press any key to exit..."

    /* REQUIRED to cleanup mongoc internals */
    mongoc_cleanup()

RETURN

STATIC PROCEDURE displayDocs( collection, filter )
    LOCAL opts
    LOCAL cursor
    LOCAL doc

    opts := bson_new()

    cursor := mongoc_collection_find_with_opts( collection, filter, opts )

    WHILE mongoc_cursor_next( cursor, @doc )
        ? bson_as_json( doc )
    ENDDO

RETURN

STATIC PROCEDURE importDocs( collection )
    LOCAL bulk
    LOCAL dbfStruct
    LOCAL itm
    LOCAL doc
    LOCAL recNo
    LOCAL result
    LOCAL reply
    LOCAL error

    ? "Initiating bulk operation:"

    bulk = mongoc_collection_create_bulk_operation( collection, .T. )

    dbfStruct := dbStruct()

    recNo := 1
    TEST->(dbGoTo( 1 ) )

    WHILE ! TEST->( eof() )

        doc := bson_new()

        BSON_APPEND_INT32( doc, "_id", recNo )  /* using dbf recNo() as doc _id */

        FOR EACH itm IN dbfStruct
            SWITCH itm[ DBS_TYPE ]
            CASE "C"
                BSON_APPEND_UTF8( doc, itm[ DBS_NAME ], rTrim( fieldGet( itm:__enumIndex ) ) )
                EXIT
            CASE "D"
                BSON_APPEND_DATE_TIME( doc, itm[ DBS_NAME ], hb_dToT( fieldGet( itm:__enumIndex ) ) ) /* date type is converted to dateTime */
                EXIT
            CASE "L"
                BSON_APPEND_BOOL( doc, itm[ DBS_NAME ], fieldGet( itm:__enumIndex ) )
                EXIT
            CASE "N"
                BSON_APPEND_DOUBLE( doc, itm[ DBS_NAME ], fieldGet( itm:__enumIndex ) )
                EXIT
            OTHERWISE
                BSON_APPEND_NULL( doc, itm[ DBS_NAME ] )
            ENDSWITCH
        NEXT

        mongoc_bulk_operation_insert( bulk, doc )

        TEST->( dbGoto( ++recNo ) )

        ?? "."

    ENDDO

    result := mongoc_bulk_operation_execute( bulk, @reply, @error )

    ? "mongoc_bulk_operation_execute reply:", bson_as_json( reply )

    IF result = 0
        ? "Error:", HB_BSON_ERROR_MESSAGE( error )
    ENDIF

RETURN
