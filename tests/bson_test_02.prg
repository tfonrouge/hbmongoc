/*
 * test: bson_test01
 *
 * convert bson types to json
 *
 */

#include "hbclass.ch"
#include "hbmongoc.ch"

PROCEDURE main()
    LOCAL bson
    LOCAL doc
    LOCAL json
    LOCAL size

    /* REQUIRED to initialize mongoc internals */
    mongoc_init()

    CLS

    /* build a simple bson array */
    doc := bson_new()
    BSON_APPEND_UTF8( doc, "name", "Fred" )
    BSON_APPEND_INT32( doc, "age", 30 )

    bson := bson_new()
    BSON_APPEND_ARRAY( bson, "array1", { 1, 2, 3, 4, 5, "John" } )
    BSON_APPEND_ARRAY( bson, "array2", { "jan", "feb", "mar" } )
    BSON_APPEND_ARRAY( bson, "array3", { .T., .F., hb_dateTime() } )
    BSON_APPEND_DOCUMENT( bson, "doc", doc )

    /* json from bson array */
    json := bson_array_as_json( bson, @size )
    ? "Result:", size, ":", json

    WAIT

    /* REQUIRED to cleanup mongoc internals */
    mongoc_cleanup()

RETURN
