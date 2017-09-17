/*
 * bson_oid test
 */

#include "hbmongoc.ch"

PROCEDURE main()
    LOCAL oid

    bson_oid_init( @oid )
    ? oid
    ? bson_oid_to_string( oid )
    ?

    bson_oid_init( @oid )
    ? oid
    ? bson_oid_to_string( oid )
    ?

    bson_oid_init( @oid )
    ? oid
    ? bson_oid_to_string( oid )
    ?

    ? bson_oid_hash( oid )

    WAIT

RETURN
