//
//  hb_bson.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 8/26/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_bson.h"

#include "hbapi.h"
#include "hbapiitm.h"
#include "hbapierr.h"
#include "hbapifs.h"
#include "hbapistr.h"
#include "hbstack.h"
#include "hbvm.h"

#include <bson.h>

#define HBBSON_ERR_ARGS()  ( hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS ) )

#define HBMONGOC_MAX_DOCUMENT_LEVEL 100

static bson_t s_bson[ HBMONGOC_MAX_DOCUMENT_LEVEL ];
static int documentLevel = 0;

/*
 hbbson_destroy
 */
static HB_GARBAGE_FUNC( hbbson_gc_func )
{
    PHB_BSON pBson = Cargo;

    if ( pBson ) {
        switch (pBson->type_t) {
            case _hb_bson_t_:
                if (pBson->bson) {
                    bson_destroy( ( bson_t * ) pBson->bson );
                }
                break;
        }
        pBson->bson = NULL;
    }
}

/*
 s_gc_mongoc_client_t
 */
static const HB_GC_FUNCS s_gc_bson_funcs = {
    hbbson_gc_func,
    hb_gcDummyMark
};

bson_t * bson_param( int iParam, int type )
{
    if ( hb_param( iParam, type ) ) {
        PHB_BSON phBson = hbbson_param( iParam );
        if ( phBson ) {
            return phBson->bson;
        } else {
            if( HB_ISCHAR( iParam ) ) {
                const char * szJSON = hb_parc( iParam );
                bson_t * bson = bson_new_from_json( ( const uint8_t * ) szJSON, -1, NULL );
                if ( bson ) {
                    return bson;
                }
            }
        }
    }
    return NULL;
}

static uint64_t hbbson_dateTimeToUnix( int iParam ) {
    long lJulian;
    long lMillis;

    hb_partdt( &lJulian, &lMillis, iParam );

    return (lJulian - 2440588) * 86400000 + lMillis;
}

PHB_BSON hbbson_param( int iParam )
{
    if ( HB_ISPOINTER( iParam ) ) {
        PHB_BSON phBson = hb_parptrGC( &s_gc_bson_funcs, iParam );
        if ( phBson && phBson->bson && phBson->type_t == _hb_bson_t_ && ( documentLevel == 0 || ( documentLevel == phBson->documentLevel ) ) ) {
            return phBson;
        }
    }
    return NULL;
}

PHB_BSON hbbson_new_dataContainer( hbbson_t_ type, void * p )
{
    PHB_BSON phBson = hb_gcAllocate( sizeof( HB_BSON ), &s_gc_bson_funcs );

    phBson->type_t = type;
    phBson->documentLevel = 0;
    switch ( type ) {
        case _hb_bson_t_:
            phBson->bson = p;
            break;
    }

    return phBson;
}

/* Harbour api */

HB_FUNC( BSON_APPEND_BINARY )
{
    bson_t * bson = bson_param( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );
    const char * binary = hb_parc( 5 );

    if ( bson && key && binary ) {
        int key_length = HB_ISNUM( 3 ) ? hb_parni( 3 ) : ( int ) hb_parclen( 2 );
        int subtype = HB_ISNUM( 4 ) ? hb_parni( 4 ) : BSON_SUBTYPE_BINARY;
        int length = HB_ISNUM( 6 ) ? hb_parni( 6 ) : ( int ) hb_parclen( 5 );
        bool result = bson_append_binary( bson, key, key_length, subtype, ( const uint8_t * ) binary, length );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_BOOL )
{
    bson_t * bson = bson_param( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );

    if ( bson && key ) {
        int key_length = HB_ISNUM( 3 ) ? hb_parni( 3 ) : ( int ) hb_parclen( 2 );
        bool value = hb_parl( 4 );
        bool result = bson_append_bool( bson, key, key_length, value );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_DATE_TIME )
{
    bson_t * bson = bson_param( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );

    if ( bson && key && HB_ISTIMESTAMP( 4 ) ) {
        int key_length = HB_ISNUM( 3 ) ? hb_parni( 3 ) : ( int ) hb_parclen( 2 );
        uint64_t value = hbbson_dateTimeToUnix( 4 );
        bool result = bson_append_date_time( bson, key, key_length, value );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_DOCUMENT )
{
    bson_t * bson = bson_param( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );
    bson_t * child = bson_param( 4, HB_IT_POINTER | HB_IT_STRING );

    if ( bson && key && child ) {
        int key_length = HB_ISNUM( 3 ) ? hb_parni( 3 ) : ( int ) hb_parclen( 2 );
        bool result = bson_append_document( bson, key, key_length, child );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }

    if ( child && HB_ISCHAR( 4 ) ) {
        bson_destroy( child );
    }
}

HB_FUNC( BSON_APPEND_DOCUMENT_BEGIN )
{
    bson_t * parent = bson_param( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );

    if ( parent && key && HB_ISBYREF( 4 ) && documentLevel <= HBMONGOC_MAX_DOCUMENT_LEVEL ) {
        int key_length = HB_ISNUM( 3 ) ? hb_parni( 3 ) : ( int ) hb_parclen( 2 );
        bson_t * child = &s_bson[ documentLevel++ ];
        bool result = bson_append_document_begin( parent, key, key_length, child );
        PHB_BSON phBson = hbbson_new_dataContainer( _hb_bson_t_, child );
        phBson->documentLevel = documentLevel;
        hb_storptrGC( phBson, 4 );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_DOCUMENT_END )
{
    if ( documentLevel > 0 ) {
        --documentLevel;
        bson_t * parent = bson_param( 1, HB_IT_POINTER );
        if ( parent ) {
            ++documentLevel;
            bson_t * child = bson_param( 2, HB_IT_POINTER );
            if ( child ) {
                --documentLevel;
                bool result = bson_append_document_end( parent, child );
                hb_retl( result );
                return;
            }
        }
    }
    HBBSON_ERR_ARGS();
}

HB_FUNC( BSON_APPEND_DOUBLE )
{
    bson_t * bson = bson_param( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );

    if ( bson && key && HB_ISNUM( 4 ) ) {
        int key_length = HB_ISNUM( 3 ) ? hb_parni( 3 ) : ( int ) hb_parclen( 2 );
        double value = hb_parnd( 4 );
        bool result = bson_append_double( bson, key, key_length, value );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_INT32 )
{
    bson_t * bson = bson_param( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );

    if ( bson && key && HB_ISNUM( 4 ) ) {
        int key_length = HB_ISNUM( 3 ) ? hb_parni( 3 ) : ( int ) hb_parclen( 2 );
        int32_t value = hb_parni( 4 );
        bool result = bson_append_int32( bson, key, key_length, value );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_INT64 )
{
    bson_t * bson = bson_param( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );

    if ( bson && key && HB_ISNUM( 4 ) ) {
        int key_length = HB_ISNUM( 3 ) ? hb_parni( 3 ) : ( int ) hb_parclen( 2 );
        int64_t value = hb_parnl( 4 );
        bool result = bson_append_int64( bson, key, key_length, value );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_APPEND_NULL )
{
    bson_t * bson = bson_param( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );

    if ( bson && key ) {
        int key_length = HB_ISNUM( 3 ) ? hb_parni( 3 ) : ( int ) hb_parclen( 2 );
        bool result = bson_append_null( bson, key, key_length );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}
HB_FUNC( BSON_APPEND_UTF8 )
{
    bson_t * bson = bson_param( 1, HB_IT_POINTER );
    const char * key = hb_parc( 2 );
    const char * value = hb_parc( 4 );

    if ( bson && key && value ) {
        int key_length = HB_ISNUM( 3 ) ? hb_parni( 3 ) : ( int ) hb_parclen( 2 );
        int length = HB_ISNUM( 5 ) ? hb_parni( 5 ) : ( int ) hb_parclen( 4 );
        bool result = bson_append_utf8( bson, key, key_length, value, length);
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_AS_JSON )
{
    bson_t * bson = bson_param( 1, HB_IT_POINTER );

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

#if BSON_CHECK_VERSION( 1, 7, 0 )
HB_FUNC( BSON_AS_CANONICAL_EXTENDED_JSON )
{
    bson_t * bson = bson_param( 1, HB_IT_POINTER );

    if ( bson ) {
        char * szJSON = bson_as_canonical_extended_json( bson, NULL );
        if ( szJSON ) {
            hb_retc( szJSON );
            bson_free( szJSON );
        }
    }
    else {
        HBBSON_ERR_ARGS();
    }
}
#endif

HB_FUNC( BSON_DESTROY )
{
    PHB_BSON phBson = hbbson_param( 1 );

    if ( phBson ) {
        bson_destroy( phBson->bson );
        phBson->bson = NULL;
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_NEW )
{
    if ( hb_pcount() == 0 ) {
        bson_t * bson = bson_new();
        if ( bson ) {
            PHB_BSON phBson = hbbson_new_dataContainer( _hb_bson_t_, bson );
            hb_retptrGC( phBson );
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}
