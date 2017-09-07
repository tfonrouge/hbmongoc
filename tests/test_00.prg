
#include "hbmongoc.ch"

PROCEDURE main()
    LOCAL read_prefs
    LOCAL bson
    LOCAL i

    /* REQUIRED to initialize mongoc internals */
    mongoc_init()

    CLS

    read_prefs := mongoc_read_prefs_new( MONGOC_READ_PRIMARY )

    FOR i := 1 TO 10
        bson := bson_new()
        BSON_APPEND_INT32( bson, "tag" + hb_ntos( i ), i )
        mongoc_read_prefs_add_tag( read_prefs, bson )
    NEXT

    WAIT

    bson := mongoc_read_prefs_get_tags( read_prefs )

    ? bson_as_json( bson )

    WAIT

    FOR i := 1 TO 1000000
        bson := mongoc_read_prefs_get_tags( read_prefs )
        ? bson_as_json( bson )
    NEXT

    WAIT

    read_prefs := nil

    WAIT

    /* REQUIRED to cleanup mongoc internals */
    mongoc_cleanup()

RETURN
