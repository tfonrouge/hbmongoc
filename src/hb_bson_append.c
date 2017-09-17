//
//  hb_bson_append.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/16/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_bson_append.h"

#include "hb_bson.h"
#include "hbjson.h"
#include "hbdate.h"

#define HBMONGOC_MAX_ARRAYDOCUMENT_LEVEL 100

static bson_t s_bson_documentStack[ HBMONGOC_MAX_ARRAYDOCUMENT_LEVEL ];
static int documentLevel = 0;

static bson_t s_bson_arrayStack[ HBMONGOC_MAX_ARRAYDOCUMENT_LEVEL ];
static int arrayLevel = 0;

HB_FUNC( BSON_APPEND_ARRAY )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );
    bson_t * array = bson_hbparam( 4 , HB_IT_ANY );

    if ( bson && key && array ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        bool result = bson_append_array( bson, key, key_length, array );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }

    if ( array && ! HB_ISPOINTER( 4 ) ) {
        bson_destroy( array );
    }
}

HB_FUNC( BSON_APPEND_ARRAY_BEGIN )
{
    bson_t * parent = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );

    if ( parent && key && HB_ISBYREF( 4 ) && arrayLevel <= HBMONGOC_MAX_ARRAYDOCUMENT_LEVEL ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        bson_t * child = &s_bson_arrayStack[ arrayLevel++ ];
        bool result = bson_append_array_begin( parent, key, key_length, child );
        PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_bson_t_, child );
        hb_storptrGC( phBson, 4 );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_ARRAY_END )
{
    bson_t * parent = bson_hbparam( 1, HB_IT_POINTER );
    bson_t * child = bson_hbparam( 2, HB_IT_POINTER );

    if ( parent && child ) {
        bool result = bson_append_array_end( parent, child );
        if ( result ) {
            --arrayLevel;
        }
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_BINARY )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );
    const char * binary = hb_parc( 5 );

    if ( bson && key && binary ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        int subtype = hb_parnidef( 4, BSON_SUBTYPE_BINARY );
        int length = hb_parnidef( 6, ( int ) hb_parclen( 5 ) );
        bool result = bson_append_binary( bson, key, key_length, subtype, ( const uint8_t * ) binary, length );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_BOOL )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );

    if ( bson && key && HB_ISLOG( 4 ) ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        bool value = hb_parl( 4 );
        bool result = bson_append_bool( bson, key, key_length, value );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_CODE )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );
    const char * javascript = hb_parc( 4 );

    if ( bson && key && javascript ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        bool result = bson_append_code( bson, key, key_length, javascript );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_CODE_WITH_SCOPE )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );
    const char * javascript = hb_parc( 4 );
    bson_t * scope = bson_hbparam( 5 , HB_IT_ANY );

    if ( bson && key && javascript && scope ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        bool result = bson_append_code_with_scope( bson, key, key_length, javascript, scope );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }

    if ( scope && ! HB_ISPOINTER( 5 ) ) {
        bson_destroy( scope );
    }
}

HB_FUNC( BSON_APPEND_DATE_TIME )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );
    PHB_ITEM pItem = hb_param( 4, HB_IT_LONG | HB_IT_DATETIME );

    if ( bson && key && pItem ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        uint64_t value;
        if ( hb_itemType( pItem ) & HB_IT_DATETIME ) {
            value = hbbson_dateTimeToUnix( pItem, false );
        } else {
            value = hb_itemGetNL( pItem );
        }
        bool result = bson_append_date_time( bson, key, key_length, value );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_DECIMAL128 )
#if BSON_CHECK_VERSION( 1, 5, 0 )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );
    bson_decimal128_t * dec = bson_decimal128_hbparam( 4 );

    if ( bson && key && dec ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        bool result = bson_append_decimal128( bson, key, key_length, dec );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}
#else
{
    HBBSON_ERR_NOFUNC();
}
#endif

HB_FUNC( BSON_APPEND_DOCUMENT )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );
    bson_t * child = bson_hbparam( 4, HB_IT_ANY );

    if ( bson && key && child ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        bool result = bson_append_document( bson, key, key_length, child );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }

    if ( child && ! HB_ISPOINTER( 4 ) ) {
        bson_destroy( child );
    }
}

HB_FUNC( BSON_APPEND_DOCUMENT_BEGIN )
{
    bson_t * parent = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );

    if ( parent && key && HB_ISBYREF( 4 ) && documentLevel <= HBMONGOC_MAX_ARRAYDOCUMENT_LEVEL ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        bson_t * child = &s_bson_documentStack[ documentLevel++ ];
        bool result = bson_append_document_begin( parent, key, key_length, child );
        PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_bson_t_, child );
        hb_storptrGC( phBson, 4 );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_DOCUMENT_END )
{
    bson_t * parent = bson_hbparam( 1, HB_IT_POINTER );
    bson_t * child = bson_hbparam( 2, HB_IT_POINTER );

    if ( parent && child ) {
        bool result = bson_append_document_end( parent, child );
        if ( result ) {
            --documentLevel;
        }
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_DOUBLE )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );

    if ( bson && key && HB_ISNUM( 4 ) ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        double value = hb_parnd( 4 );
        bool result = bson_append_double( bson, key, key_length, value );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_INT32 )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );

    if ( bson && key && HB_ISNUM( 4 ) ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        int32_t value = hb_parni( 4 );
        bool result = bson_append_int32( bson, key, key_length, value );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_INT64 )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );

    if ( bson && key && HB_ISNUM( 4 ) ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        int64_t value = hb_parnl( 4 );
        bool result = bson_append_int64( bson, key, key_length, value );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_NOW_UTC )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );

    if ( bson && key ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        bool result = bson_append_now_utc( bson, key, key_length );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_NULL )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );

    if ( bson && key ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        bool result = bson_append_null( bson, key, key_length );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_OID )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );
    const bson_oid_t * oid = bson_oid_hbparam( 4 );

    if ( bson && key && oid ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        bool result = bson_append_oid( bson, key, key_length, oid );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_REGEX )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );
    const char * regex = hb_parc( 4 );

    if( bson && key && regex ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        const char * options = hb_parc( 5 );
        bool result = bson_append_regex( bson, key, key_length, regex, options );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }

}

HB_FUNC( BSON_APPEND_UTF8 )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );
    const char * value = hb_parc( 4 );

    if ( bson && key && value ) {
        int key_length = hb_parnidef( 3, ( int ) hb_parclen( 2 ) );
        int length = hb_parnidef( 5, ( int ) hb_parclen( 4 ) );
        bool result = bson_append_utf8( bson, key, key_length, value, length);
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}
