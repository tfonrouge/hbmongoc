//
//  hb_bson.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 8/26/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_bson.h"
#include "hbjson.h"
#include "hbdate.h"

#define HBMONGOC_MAX_ARRAYDOCUMENT_LEVEL 100

enum hb_return_json_type
{
#if BSON_CHECK_VERSION( 1, 7, 0 )
    _HBRETJSON_CANONICAL_,
    _HBRETJSON_RELAXED_,
#endif
    _HBRETJSON_SIMPLE_
};

static enum hb_return_json_type s_hbmongoc_return_json_type =
#if BSON_CHECK_VERSION( 1, 7, 0 )
    _HBRETJSON_RELAXED_;
#else
    _HBRETJSON_SIMPLE_;
#endif

static const char * _STR_JSON_SIMPLE_       = "SIMPLE";
#if BSON_CHECK_VERSION( 1, 7, 0 )
static const char * _STR_JSON_CANONICAL_    = "CANONICAL";
static const char * _STR_JSON_RELAXED_      = "RELAXED";
#endif

static PHB_BSON hbbson_hbparam( PHB_ITEM pItem, hbbson_t_ hbbson_type );

static bson_t s_bson_documentStack[ HBMONGOC_MAX_ARRAYDOCUMENT_LEVEL ];
static int documentLevel = 0;

static bson_t s_bson_arrayStack[ HBMONGOC_MAX_ARRAYDOCUMENT_LEVEL ];
static int arrayLevel = 0;

/*
 hbbson_destroy
 */
static HB_GARBAGE_FUNC( hbbson_gc_func )
{
    PHB_BSON phBson = Cargo;

    if ( phBson ) {
        switch (phBson->hbbson_type) {
            case _hbbson_bson_t_:
                if (phBson->p) {
                    bson_destroy( ( bson_t * ) phBson->p );
                    phBson->p = NULL;
                }
                break;
#if BSON_CHECK_VERSION( 1, 5, 0 )
            case _hbbson_decimal128_t_:
                if ( phBson->p ) {
                    hb_xfree( phBson->p );
                    phBson->p = NULL;
                }
                break;
#endif
        }
    }
}

static const HB_GC_FUNCS s_gc_bson_funcs = {
    hbbson_gc_func,
    hb_gcDummyMark
};

#if BSON_CHECK_VERSION( 1, 5, 0 )
bson_decimal128_t * bson_decimal128_hbparam( int iParam )
{
    PHB_BSON phBson = hbbson_param( iParam, _hbbson_decimal128_t_ );

    if ( phBson ) {
        return phBson->p;
    }
    return NULL;
}
#endif

bson_t * bson_hbparam( int iParam, long lMask )
{
    PHB_ITEM pItem = hb_param( iParam, lMask );

    if ( pItem ) {
        if ( hb_itemType( pItem ) & HB_IT_POINTER ) {
            PHB_BSON phBson = hbbson_hbparam( pItem, _hbbson_bson_t_ );
            if ( phBson ) {
                return phBson->p;
            }
        } else if ( hb_itemType( pItem ) & HB_IT_STRING ) {
            const char * szJSON = NULL;
            szJSON = hb_itemGetC( pItem );
            bson_t * bson = bson_new_from_json( ( const uint8_t * ) szJSON, -1, NULL );
            if ( bson ) {
                return bson;
            }
        } else if ( hb_itemType( pItem ) & ( HB_IT_HASH | HB_IT_ARRAY ) ) {
            char * szJSON = hb_jsonEncode( pItem, NULL, false );
            if ( szJSON ) {
                bson_t * bson = bson_new_from_json( ( const uint8_t * ) szJSON, -1, NULL );
                hb_xfree( szJSON );
                if ( bson ) {
                    return bson;
                }
            }
        }
    }
    return NULL;
}

void bson_hbstor_byref_error( int iParam, bson_error_t * error, HB_BOOL valid )
{
    if ( HB_ISBYREF( iParam ) ) {
        if ( ! valid && error && strlen( error->message ) > 0 ) {
            PHB_ITEM pItemHash = hb_itemNew( NULL );
            hb_hashNew( pItemHash );

            PHB_ITEM pItemKey;
            PHB_ITEM pItemValue;

            pItemKey = hb_itemNew( NULL );
            pItemValue = hb_itemNew( NULL );
            hb_itemPutC( pItemKey, "domain" );
            hb_itemPutNI( pItemValue, error->domain );
            hb_hashAdd( pItemHash, pItemKey, pItemValue );
            hb_itemRelease( pItemKey );
            hb_itemRelease( pItemValue );

            pItemKey = hb_itemNew( NULL );
            pItemValue = hb_itemNew( NULL );
            hb_itemPutC( pItemKey, "code" );
            hb_itemPutNI( pItemValue, error->code );
            hb_hashAdd( pItemHash, pItemKey, pItemValue );
            hb_itemRelease( pItemKey );
            hb_itemRelease( pItemValue );

            pItemKey = hb_itemNew( NULL );
            pItemValue = hb_itemNew( NULL );
            hb_itemPutC( pItemKey, "message" );
            hb_itemPutC( pItemValue, error->message );
            hb_hashAdd( pItemHash, pItemKey, pItemValue );
            hb_itemRelease( pItemKey );
            hb_itemRelease( pItemValue );
            
            hb_itemParamStoreRelease( iParam, pItemHash );
        } else {
            hb_stor( iParam );
        }
    }
}

char * hbbson_as_json( const bson_t * bson )
{
    char * szJSON = NULL;
    switch ( s_hbmongoc_return_json_type ) {
        case _HBRETJSON_SIMPLE_:
            szJSON = bson_as_json( bson, NULL );
            break;
#if BSON_CHECK_VERSION( 1, 7, 0 )
        case _HBRETJSON_CANONICAL_:
            szJSON = bson_as_canonical_extended_json( bson, NULL );
            break;
        case _HBRETJSON_RELAXED_:
            szJSON = bson_as_relaxed_extended_json( bson, NULL );
            break;
#endif
    }
    return szJSON;
}

static uint64_t hbbson_juliantimeToUnix( long lJulian, long lMillis, HB_BOOL utc )
{
    long utfOffset = utc ? hb_timeUTCOffset() * 1000 : 0;
    return (lJulian - 2440588) * 86400000 + lMillis - utfOffset;
}

static uint64_t hbbson_dateTimeToUnix( PHB_ITEM pItem, HB_BOOL utc )
{
    long lJulian; long lMillis;
    hb_itemGetTDT( pItem, &lJulian, &lMillis );
    return hbbson_juliantimeToUnix( lJulian, lMillis, utc );
//    return ( ( hb_itemGetTD( pItem ) - 2440587.5 ) * 86400000 );
}

HB_FUNC( HB_DTTOUNIX )
{
    if ( HB_ISDATETIME( 1 ) ) {
        PHB_ITEM pItem = hb_param( 1, HB_IT_DATETIME );
        hb_retnl( hbbson_dateTimeToUnix( pItem, hb_parl( 2 ) ) );
    } else if ( HB_ISNIL( 1 ) ) {
        long lJulian, lMillis;
        hb_timeStampGet( &lJulian, &lMillis );
        hb_retnl( hbbson_juliantimeToUnix( lJulian, lMillis, hb_parl( 2 ) ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( HB_UNIXTOT )
{
    PHB_ITEM pItem = hb_param( 1, HB_IT_NUMERIC );

    if ( pItem ) {
        double julian = ( hb_itemGetND( pItem ) / 86400000 ) + 2440588;
        hb_rettd( julian );
    } else {
        HBBSON_ERR_ARGS();
    }
}

static PHB_BSON hbbson_hbparam( PHB_ITEM pItem, hbbson_t_ hbbson_type )
{
    PHB_BSON phBson = hb_itemGetPtrGC( pItem, &s_gc_bson_funcs );

    return phBson && phBson->hbbson_type == hbbson_type ? phBson : NULL;
}

PHB_BSON hbbson_new_dataContainer( hbbson_t_ hbbson_type, void * p )
{
    if ( p ) {
        PHB_BSON phBson = hb_gcAllocate( sizeof( HB_BSON ), &s_gc_bson_funcs );

        phBson->hbbson_type = hbbson_type;

        switch ( hbbson_type ) {
            case _hbbson_bson_t_:
                phBson->p = p;
                break;
#if BSON_CHECK_VERSION( 1, 5, 0 )
            case _hbbson_decimal128_t_:
                phBson->p = p;
                break;
#endif
        }
        
        return phBson;

    } else {
        HBBSON_ERR_ARGS();
    }

    return NULL;
}

PHB_BSON hbbson_param( int iParam, hbbson_t_ hbbson_type )
{
    PHB_ITEM pItem = hb_param( iParam, HB_IT_POINTER );

    if ( pItem ) {
        PHB_BSON phBson = hbbson_hbparam( pItem, hbbson_type );
        if ( phBson && phBson->hbbson_type == hbbson_type ) {
            switch ( hbbson_type ) {
                case _hbbson_bson_t_:
                    if ( phBson->p ) {
                        return phBson;
                    }
                    break;
#if BSON_CHECK_VERSION( 1, 5, 0 )
                case _hbbson_decimal128_t_:
                    if ( phBson->p ) {
                        return phBson;
                    }
                    break;
#endif
            }
        }
    }
    return NULL;
}

/* Harbour api */

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

HB_FUNC( BSON_ARRAY_AS_JSON )
{
    const bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        size_t length;
        const char * result = bson_array_as_json( bson, &length );
        if ( HB_ISBYREF( 2 ) ) {
            hb_stornl( (HB_LONG) length, 2 );
        }
        hb_retc( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_AS_CANONICAL_EXTENDED_JSON )
#if BSON_CHECK_VERSION( 1, 7, 0 )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        char * szJSON = bson_as_canonical_extended_json( bson, NULL );
        if( szJSON ) {
            hb_retc( szJSON );
            bson_free( szJSON );
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}
#else
{
    HBBSON_ERR_NOFUNC();
}
#endif

HB_FUNC( BSON_AS_JSON )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        char * szJSON = bson_as_json( bson, NULL );
        if( szJSON ) {
            hb_retc( szJSON );
            bson_free( szJSON );
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_AS_RELAXED_EXTENDED_JSON )
#if BSON_CHECK_VERSION( 1, 7, 0 )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        char * szJSON = bson_as_relaxed_extended_json( bson, NULL );
        if ( szJSON ) {
            hb_retc( szJSON );
            bson_free( szJSON );
        }
    }
    else {
        HBBSON_ERR_ARGS();
    }
}
#else
{
    HBBSON_ERR_NOFUNC();
}
#endif

HB_FUNC( BSON_CHECK_VERSION )
{
    if ( HB_ISNUM( 1 ) && HB_ISNUM( 2 ) && HB_ISNUM( 3 ) ) {
        hb_retl( bson_check_version( hb_parni( 1 ), hb_parni( 2 ), hb_parni( 3 ) ) );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_COPY )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        bson_t * copy = bson_copy( bson );
        PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_bson_t_, copy );
        hb_retptrGC( phBson );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_DECIMAL128_FROM_STRING )
#if BSON_CHECK_VERSION( 1, 5, 0 )
{
    const char * string = hb_parc( 1 );

    if ( string && HB_ISBYREF( 2 ) ) {
        bson_decimal128_t * dec = hb_xgrab( sizeof( bson_decimal128_t ) );
        bool result = bson_decimal128_from_string( string, dec );
        PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_decimal128_t_, dec );
        hb_storptrGC( phBson, 2 );
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

HB_FUNC( BSON_DECIMAL128_TO_STRING )
#if BSON_CHECK_VERSION( 1, 5, 0 )
{
    const bson_decimal128_t * dec = bson_decimal128_hbparam( 1 );

    if ( dec ) {
        char string[ BSON_DECIMAL128_STRING ];
        bson_decimal128_to_string( dec, string );
        if ( HB_ISBYREF( 2 ) ) {
            hb_storc( string, 2 );
        }
        hb_retc( string );
    } else {
        HBBSON_ERR_ARGS();
    }
}
#else
{
    HBBSON_ERR_NOFUNC();
}
#endif

HB_FUNC( BSON_DESTROY )
{
    PHB_BSON phBson = hbbson_param( 1, _hbbson_bson_t_ );

    if ( phBson ) {
        bson_destroy( phBson->p );
        phBson->p = NULL;
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_GET_MAJOR_VERSION )
{
    hb_retni( bson_get_major_version() );
}

HB_FUNC( BSON_GET_MICRO_VERSION )
{
    hb_retni( bson_get_micro_version() );
}

HB_FUNC( BSON_GET_MINOR_VERSION )
{
    hb_retni( bson_get_minor_version() );
}

HB_FUNC( BSON_GET_VERSION )
{
    hb_retc( bson_get_version() );
}

HB_FUNC( BSON_HAS_FIELD )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );

    if ( bson && key ) {
        bool result = bson_has_field( bson, key );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_NEW )
{
    if ( hb_pcount() == 0 ) {
        bson_t * bson = bson_new();
        if ( bson ) {
            PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_bson_t_, bson );
            hb_retptrGC( phBson );
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_NEW_FROM_JSON )
{
    PHB_ITEM pItem = hb_param( 1, HB_IT_ANY );
    bson_t * bson = NULL;
    bson_error_t error;

    if ( hb_itemType( pItem ) & HB_IT_STRING ) {
        const char * data = hb_parc( 1 );
        int len = hb_parnidef( 2, ( int ) hb_parclen( 1 ) );
        bson = bson_new_from_json( ( const uint8_t * ) data, len, &error );
    } else if ( hb_itemType( pItem ) & ( HB_IT_HASH | HB_IT_ARRAY ) ) {
        char * szJSON = hb_jsonEncode( pItem, NULL, false );
        bson = bson_new_from_json( ( const uint8_t * ) szJSON, -1, &error );
        hb_xfree( szJSON );
    } else {
        HBBSON_ERR_ARGS();
    }

    bson_hbstor_byref_error( 3, &error, bson != NULL );

    if ( bson ) {
        PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_bson_t_, bson );
        hb_retptrGC( phBson );
    } else {
        hb_ret();
    }
}

HB_FUNC( BSON_VALIDATE )
{
    const bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        bson_validate_flags_t flags = hb_parnidef( 2, BSON_VALIDATE_NONE );
        size_t offset;
        bool result = bson_validate( bson, flags, &offset );
        if ( HB_ISBYREF( 3 ) ) {
            hb_storns( offset, 3 );
        }
        hb_retl( result );
    }
}

HB_FUNC( HB_BSON_AS_HASH )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        char * szJSON;
        szJSON = hbbson_as_json( bson );
        if ( szJSON ) {
            PHB_ITEM pItem = hb_itemNew( NULL );
            hb_jsonDecode( szJSON, pItem );
            bson_free( szJSON );
            hb_itemReturnRelease( pItem );
        } else {
            hb_ret();
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( HB_BSON_AS_JSON )
{
    bson_t * bson = bson_hbparam( 1, HB_IT_POINTER );

    if ( bson ) {
        char * szJSON;
        szJSON = hbbson_as_json( bson );
        if ( szJSON ) {
            hb_retc( szJSON );
            bson_free( szJSON );
        } else {
            hb_ret();
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( HB_BSON_SET_RETURN_JSON_TYPE )
{
    if ( hb_pcount() > 0 ) {
        if ( hb_stricmp( hb_parc( 1 ), _STR_JSON_SIMPLE_ ) == 0 ) {
            s_hbmongoc_return_json_type = _HBRETJSON_SIMPLE_;
#if BSON_CHECK_VERSION( 1, 7, 0 )
        } else if ( hb_stricmp( hb_parc( 1 ), _STR_JSON_CANONICAL_ ) == 0 ) {
            s_hbmongoc_return_json_type = _HBRETJSON_CANONICAL_;
        } else if ( hb_stricmp( hb_parc( 1 ), _STR_JSON_RELAXED_ ) == 0 ) {
            s_hbmongoc_return_json_type = _HBRETJSON_CANONICAL_;
#endif
        } else {
            HBBSON_ERR_ARGS();
        }
    }
    switch ( s_hbmongoc_return_json_type ) {
        case _HBRETJSON_SIMPLE_:
            hb_retc( _STR_JSON_SIMPLE_ );
            break;
#if BSON_CHECK_VERSION( 1, 7, 0 )
        case _HBRETJSON_CANONICAL_:
            hb_retc( _STR_JSON_CANONICAL_ );
            break;
        case _HBRETJSON_RELAXED_:
            hb_retc( _STR_JSON_RELAXED_ );
            break;
#endif
    }
}

HB_FUNC( HB_BSON_VERSION )
{
    hb_storni( BSON_MAJOR_VERSION, 1 );
    hb_storni( BSON_MINOR_VERSION, 2 );
    hb_storni( BSON_MICRO_VERSION, 3 );
}
