/*
 * bson_oid test
 */

#include "hbmongoc.ch"

PROCEDURE main()
    LOCAL json
    LOCAL i
    LOCAL error
    LOCAL bson
    LOCAL iter
    LOCAL len
    LOCAL array
    LOCAL doc

#pragma __stream | json := %s
{
    "_id" : 1,
    "name" : {
        "first" : "John",
        "last" : "Backus"
    },
    "contribs" : [
                  "Fortran",
                  "ALGOL",
                  "Backus-Naur Form",
                  "FP"
                  ],
    "awards" : [
                {
                "award" : "W.W. McDowell Award",
                "year" : 1967,
                "by" : "IEEE Computer Society"
                },
                {
                "award" : "National Medal of Science",
                "year" : 1975,
                "by" : "National Science Foundation"
                },
                {
                "award" : "Turing Award",
                "year" : 1977,
                "by" : "ACM"
                },
                {
                "award" : "Draper Prize",
                "year" : 1993,
                "by" : "National Academy of Engineering"
                }
                ]
}
ENDTEXT

    /* check for memory hogs */
    FOR i := 1 TO 1000000

        CLS

        bson := bson_new_from_json( json, nil, @error )

        ? i
        ? "JSON:", hb_bson_as_json( bson )
        ?

        IF error != nil
            ? "ERROR:", HB_BSON_ERROR_MESSAGE( error )
            QUIT
        ENDIF

        IF bson_iter_init( @iter, bson )
            WHILE bson_iter_next( iter )
                ? "key:", bson_iter_key( iter )
                IF bson_iter_type( iter ) = BSON_TYPE_ARRAY
                    bson_iter_array( iter, @len, @array )
                    doc := bson_new_from_data( array, len )
                    ? "ARRAY:", hb_bson_as_json( doc )
                ENDIF
            ENDDO
        ENDIF

    NEXT

    WAIT

RETURN
