//
//  hb_hbmongoc.prg
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/9/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "../hbmongoc.ch"

FUNCTION hb_bson_append( bson, key, value )
    LOCAL itm
    LOCAL begin
    LOCAL result

    SWITCH valType( key )
    CASE "A"
        FOR EACH itm IN key
            hb_bson_append( bson, hb_nToS( itm:__enumIndex - 1 ), itm )
        NEXT
        EXIT
    CASE "H"
        FOR EACH itm IN key
            hb_bson_append( bson, itm:__enumKey, itm )
        NEXT
        EXIT
    OTHERWISE
        SWITCH valType( value )
        CASE "A"
            BSON_APPEND_ARRAY_BEGIN( bson, key, @begin )
            FOR EACH itm IN value
                hb_bson_append( begin, hb_nToS( itm:__enumIndex - 1 ), itm )
            NEXT
            BSON_APPEND_ARRAY_END( bson, begin )
            EXIT
        CASE "H"
            BSON_APPEND_DOCUMENT_BEGIN( bson, key, @begin )
            FOR EACH itm IN value
                hb_bson_append( begin, itm:__enumKey, itm )
            NEXT
            BSON_APPEND_DOCUMENT_END( bson, begin )
            EXIT
        OTHERWISE
            SWITCH valType( value )
            CASE "M"
            CASE "C"
                result := BSON_APPEND_UTF8( bson, key, value )
                EXIT
            CASE "N"
                SWITCH hb_numType( value )
                CASE "I"
                    result := BSON_APPEND_INT32( bson, key, value )
                    EXIT
                CASE "L"
                    result := BSON_APPEND_INT64( bson, key, value )
                    EXIT
                CASE "D"
                    result := BSON_APPEND_DOUBLE( bson, key, value )
                    EXIT
                ENDSWITCH
                EXIT
            CASE "D"
            CASE "T"
                result := BSON_APPEND_DATE_TIME( bson, key, value )
                EXIT
            CASE "L"
                result := BSON_APPEND_BOOL( bson, key, value )
                EXIT
            CASE "A"
            CASE "H"
            CASE "O"
                result := BSON_APPEND_BINARY( bson, key, hb_serialize( value ) )
                EXIT
            CASE "P"
                result := BSON_APPEND_DOCUMENT( bson, key, value )
                EXIT
            OTHERWISE
                result := BSON_APPEND_NULL( bson, key )
            ENDSWITCH
            EXIT
        ENDSWITCH
    ENDSWITCH

RETURN result

FUNCTION hb_bson_getValue(bson, key, defValue)
    LOCAL iter

    IF bson_iter_init_find(@iter, bson, key)
/*
#define BSON_TYPE_EOD           0x00
#define BSON_TYPE_DOUBLE
#define BSON_TYPE_UTF8
#define BSON_TYPE_DOCUMENT
#define BSON_TYPE_ARRAY         0x04
#define BSON_TYPE_BINARY        0x05
#define BSON_TYPE_UNDEFINED     0x06
#define BSON_TYPE_OID           0x07
#define BSON_TYPE_BOOL
#define BSON_TYPE_DATE_TIME
#define BSON_TYPE_NULL          0x0A
#define BSON_TYPE_REGEX         0x0B
#define BSON_TYPE_DBPOINTER     0x0C
#define BSON_TYPE_CODE          0x0D
#define BSON_TYPE_SYMBOL        0x0E
#define BSON_TYPE_CODEWSCOPE    0x0F
#define BSON_TYPE_INT32
#define BSON_TYPE_TIMESTAMP     0x11
#define BSON_TYPE_INT64
#define BSON_TYPE_MAXKEY        0x7F
#define BSON_TYPE_MINKEY        0xFF
*/
        SWITCH bson_iter_type(iter)
        CASE BSON_TYPE_DOUBLE
            RETURN bson_iter_double(iter)
        CASE BSON_TYPE_UTF8
            RETURN bson_iter_utf8(iter)
        CASE BSON_TYPE_DOCUMENT
            RETURN bson_iter_document(iter)
        CASE BSON_TYPE_BOOL
            RETURN bson_iter_bool(iter)
        CASE BSON_TYPE_DATE_TIME
            RETURN hb_unixToT(bson_iter_date_time(iter))
        CASE BSON_TYPE_INT32
            RETURN bson_iter_int32(iter)
        CASE BSON_TYPE_INT64
            RETURN bson_iter_int64(iter)
        ENDSWITCH
    ENDIF

RETURN defValue
