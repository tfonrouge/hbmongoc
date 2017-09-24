/*
 * libbson example but instead of using BCON using Harbour HASH
 */

#include "hbmongoc.ch"

PROCEDURE main( ... )
    LOCAL i
    LOCAL n
    LOCAL bcon
    LOCAL foo, bar, baz
    LOCAL bson := bson_new()
    LOCAL millis

    setMode( 40, 120 )

    bson_init (bson)

    IF pCount() != 2
        outErr( ;
        e"usage: bcon-speed NUM_ITERATIONS [y|n]\n" + ;
        e"\n" + ;
        e"  y = perform speed tests with hash\n" + ;
        e"  n = perform speed tests with bson_append\n" + ;
        e"\n")
        RETURN
    ENDIF

    //BSON_ASSERT (argc == 3);

    WAIT

    outStd( e"\n" )

    n := val( hb_pValue( 1 ) )
    bcon := iif( hb_pValue( 2 ) = "y", 1, 0 )

    millis := hb_milliSeconds()

    FOR i := 1 TO n
        IF bcon = 1
            /*
            BCON_APPEND (bson, ;
                         "foo", ;
                         "{", ;
                         "bar", ;
                         "{", ;
                         "baz", ;
                         "[", ;
                         BCON_INT32 (1), ;
                         BCON_INT32 (2), ;
                         BCON_INT32 (3), ;
                         "]", ;
                         "}", ;
                         "}")
             */
            hb_bson_append( bson, { "foo" => { "bar" => { "baz" => { 1, 2, 3 } } } } )
        ELSE
            bson_append_document_begin (bson, "foo", -1, @foo)
            bson_append_document_begin (foo, "bar", -1, @bar)
            bson_append_array_begin (bar, "baz", -1, @baz)
            bson_append_int32 (baz, "0", -1, 1)
            bson_append_int32 (baz, "1", -1, 2)
            bson_append_int32 (baz, "2", -1, 3)
            bson_append_array_end (bar, baz)
            bson_append_document_end (foo, bar)
            bson_append_document_end (bson, foo)
        ENDIF

//        outStd( "BSON:", hb_bson_as_json( bson ), e"\n" )

        bson_reinit(bson)

    NEXT

    bson_destroy (bson)

    outStd( "done in ", hb_nToS(hb_milliSeconds() - millis), e"milliseconds \n" )
    WAIT

return

