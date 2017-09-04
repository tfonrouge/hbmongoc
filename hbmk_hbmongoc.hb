
#pragma -w3

#if defined( __HBSCRIPT__HBMK_PLUGIN )

FUNCTION hbmk_plugin_hbmongoc( hbmk )
    LOCAL itm
    LOCAL libbsonIncPath
    LOCAL buffer
    LOCAL a
    LOCAL v
    LOCAL hbbson_inc_name

    SWITCH hbmk[ "cSTATE" ]
    CASE "pre_all"
        EXIT
    CASE "pre_c"
        EXIT
    CASE "post_all"
        libbsonIncPath := GetEnv( "HBMK_DIR_LIBBSON" )
        hbbson_inc_name := "hbbson.ch"
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
#pragma __stream | buffer := %s
//
//  hbmongoc.ch
//  hbmongoc
//
//  Automatically Created for the hbmongoc library
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
                hb_memoWrit( hbbson_inc_name, buffer )
            ENDIF
        ENDIF
        IF empty( libbsonIncPath )
            hbmk_OutErr( hbmk, e"Error: can't build \"" + hbbson_inc_name + e"\"..." )
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
