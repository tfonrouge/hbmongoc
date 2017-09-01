
#pragma -w3

#if defined( __HBSCRIPT__HBMK_PLUGIN )

FUNCTION hbmk_plugin_hbmongoc( hbmk )
    LOCAL aInc
    LOCAL itm
    LOCAL libbsonIncPath
    LOCAL buffer
    LOCAL a
    LOCAL v

    SWITCH hbmk[ "cSTATE" ]
    CASE "pre_all"
        EXIT
    CASE "pre_c"
        EXIT
    CASE "post_all"
        aInc := hb_hValueAt( hbmk, len( hbmk ) )[ 26 ]
        FOR EACH itm IN aInc
            IF "libbson-1.0" $ itm
                libbsonIncPath := itm
            ENDIF
        NEXT
        IF ! empty( libbsonIncPath )
            a := { "#define BSON_MAJOR_VERSION", "#define BSON_MINOR_VERSION", "#define BSON_MICRO_VERSION" }
            buffer := hb_memoRead( libbsonIncPath + "/bson-version.h" )
            IF ! empty( buffer )
                libbsonIncPath := {=>}
                FOR EACH itm IN hb_aTokens( buffer, e"\n" )
                    FOR EACH v IN a
                        IF itm = v
                            libbsonIncPath[ v ] := itm
                        ENDIF
                    NEXT
                NEXT
            ENDIF
#pragma __stream | buffer := %s
//
//  hbmongoc.ch
//  hbmongoc
//
//  Automatically Created by Teo Fonrouge on 8/26/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#ifndef hbbson_ch
#define hbbson_ch

ENDTEXT
            FOR EACH v IN libbsonIncPath
                buffer += v + e"\n"
            NEXT
#pragma __stream | buffer += %s

#define BSON_CHECK_VERSION(major,minor,micro)   (BSON_MAJOR_VERSION > (major) .OR. (BSON_MAJOR_VERSION == (major) .AND. BSON_MINOR_VERSION > (minor)) .OR. (BSON_MAJOR_VERSION == (major) .AND. BSON_MINOR_VERSION == (minor) .AND. BSON_MICRO_VERSION >= (micro)))

#endif /* hbbson_ch */

ENDTEXT
            hb_memoWrit( "hbbson.ch", buffer )
        ELSE
            hbmk_OutErr( hbmk, "libbson-1.0 include dir not found..." )
        ENDIF
        EXIT
    ENDSWITCH

RETURN NIL

#else

PROCEDURE Main()

    ?? "Cannot be run in standalone mode. Use it with -plugin= option of hbmk2."
    ?

    RETURN

#endif
