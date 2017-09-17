/*
 * bson_oid test
 */

#include "hbmongoc.ch"

PROCEDURE main()
    LOCAL client
    LOCAL collection
    LOCAL i
    LOCAL filter
    LOCAL cursor
    LOCAL doc
    LOCAL iter
    LOCAL subtype
    LOCAL binary_len
    LOCAL binary

    /* REQUIRED to initialize mongoc internals */
    mongoc_init()

    WAIT

    client := mongoc_client_new( "mongodb://localhost" )

    collection := mongoc_client_get_collection( client, "ArelDb", "OTRAB_HDR" )

    FOR i := 1 TO 10000

        CLS

        filter := bson_new()

        cursor := mongoc_collection_find_with_opts( collection, filter )
        
        WHILE mongoc_cursor_next( cursor, @doc )
            bson_iter_init( @iter, doc )
            WHILE bson_iter_next( iter )
                ? "key, type:", bson_iter_key( iter ), bson_iter_type( iter )
                IF bson_iter_type( iter ) = BSON_TYPE_BINARY
                    bson_iter_binary( iter, @subtype, @binary_len, @binary )
                    ? "Subtype:", subtype
                    ? "binary len:", binary_len
                    ? "Binary:", len( binary )
                ENDIF
            ENDDO
        ENDDO

    NEXT

    WAIT

    /* REQUIRED to cleanup mongoc internals */
    mongoc_cleanup()

RETURN
