//
//  hb_bson_iter.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/17/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_bson_iter.h"

#include "hb_bson.h"

HB_FUNC( BSON_ITER_ARRAY )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_ARRAY( iter ) ) {

        uint32_t array_len;
        const uint8_t * array;
        bson_iter_array( iter, &array_len, &array );

        if ( HB_ISBYREF( 2 ) ) {
            hb_stornl( array_len, 2 );
        }

        if ( HB_ISBYREF( 3 ) ) {
            hb_storclen( ( const char * ) array, array_len, 3 );
        }

    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_AS_BOOL )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        bool result = bson_iter_as_bool( iter );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_AS_DOUBLE )
#if BSON_CHECK_VERSION( 1, 6, 3 )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        double result = bson_iter_as_double( iter );
        hb_retnd( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}
#else
{
    HBBSON_ERR_NOFUNC();
}
#endif

HB_FUNC( BSON_ITER_AS_INT64 )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        HB_LONGLONG result = bson_iter_as_int64( iter );
        hb_retnll( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_BINARY )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_BINARY( iter ) && HB_ISBYREF( 4 ) ) {
        bson_subtype_t subtype;
        uint32_t binary_len;
        const uint8_t * binary;

        bson_iter_binary( iter, &subtype, &binary_len, &binary );

        if ( HB_ISBYREF( 2 ) ) {
            hb_storni( subtype, 2 );
        }
        if ( HB_ISBYREF( 3 ) ) {
            hb_stornl( binary_len, 3 );
        }

        hb_storclen( ( const char * ) binary, binary_len, 4 );

    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_BOOL )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_BOOL( iter ) ) {
        bool result = bson_iter_bool( iter );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_CODE )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_CODE( iter ) ) {
        uint32_t length;
        const char * code = bson_iter_code( iter, &length );
        if ( HB_ISBYREF( 2 ) ) {
            hb_stornl( length, 2 );
        }
        hb_retc( code );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_DATE_TIME )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_DATE_TIME( iter ) ) {
        HB_LONGLONG dt = bson_iter_date_time( iter );
        hb_retnll( dt );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_DECIMAL128 )
#if BSON_CHECK_VERSION( 1, 5, 0 )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_DECIMAL128( iter ) && HB_ISBYREF( 2 ) ) {
        bson_decimal128_t * dec = hb_xgrab( sizeof( bson_decimal128_t ) );
        bool result = bson_iter_decimal128( iter, dec );
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

HB_FUNC( BSON_ITER_DOCUMENT )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if (iter && BSON_ITER_HOLDS_DOCUMENT(iter)) {
        uint32_t document_len;
        const uint8_t * document;
        bson_iter_document(iter, &document_len, &document);
        bson_t *doc = bson_new_from_data(document, document_len);

        if (HB_ISBYREF(2)) {
            hb_stornl(document_len, 2);
        }

        if (HB_ISBYREF(3)) {
            hb_storclen((const char * ) document, document_len, 3);
        }

        if (doc) {
            PHB_BSON phBson = hbbson_new_dataContainer(_hbbson_t_, doc);
            hb_retptrGC(phBson);
        } else {
            hb_ret();
        }

    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_DOUBLE )
{
    const bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_DOUBLE( iter ) ) {
        double result = bson_iter_double( iter );
        hb_retnd( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_FIND )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );
    const char * key = hb_parc( 2 );

    if ( iter && key ) {
        bool result = bson_iter_find( iter, key );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_FIND_CASE )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );
    const char * key = hb_parc( 2 );

    if ( iter && key ) {
        bool result = bson_iter_find_case( iter, key );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_FIND_DESCENDANT )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );
    const char * key = hb_parc( 2 );
    bson_iter_t * descendant = bson_iter_hbparam( 3 );

    if ( iter && key && descendant ) {
        bool result = bson_iter_find_descendant( iter, key, descendant );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }

}

HB_FUNC( BSON_ITER_INIT )
{
    const bson_t * bson = bson_hbparam( 2, HB_IT_POINTER );

    if ( HB_ISBYREF( 1 ) && bson ) {

        PHB_BSON phBson = hbbson_param( 1, _hbbson_iter_t_ );
        bson_iter_t * iter = NULL;

        if ( phBson ) {
            iter = phBson->p;
        } else {
            iter = hb_xgrab( sizeof( bson_iter_t ) );
        }

        bool result = bson_iter_init( iter, bson );

        if ( phBson == NULL ) {
            phBson = hbbson_new_dataContainer( _hbbson_iter_t_, iter );
            hb_storptrGC( phBson, 1 );
        }

        hb_retl( result );

    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_INIT_FIND )
{
    const bson_t * bson = bson_hbparam( 2, HB_IT_POINTER );
    const char * key = hb_parc( 3 );

    if ( HB_ISBYREF( 1 ) && bson && key ) {

        PHB_BSON phBson = hbbson_param( 1, _hbbson_iter_t_ );
        bson_iter_t * iter = NULL;

        if ( phBson ) {
            iter = phBson->p;
        } else {
            iter = hb_xgrab( sizeof( bson_iter_t ) );
        }

        bool result = bson_iter_init_find( iter, bson, key );

        if ( phBson == NULL ) {
            phBson = hbbson_new_dataContainer( _hbbson_iter_t_, iter );
            hb_storptrGC( phBson, 1 );
        }

        hb_retl( result );

    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_INIT_FIND_CASE )
{
    const bson_t * bson = bson_hbparam( 2, HB_IT_POINTER );
    const char * key = hb_parc( 3 );

    if ( HB_ISBYREF( 1 ) && bson && key ) {

        PHB_BSON phBson = hbbson_param( 1, _hbbson_iter_t_ );
        bson_iter_t * iter = NULL;

        if ( phBson ) {
            iter = phBson->p;
        } else {
            iter = hb_xgrab( sizeof( bson_iter_t ) );
        }

        bool result = bson_iter_init_find_case( iter, bson, key );

        if ( phBson == NULL ) {
            phBson = hbbson_new_dataContainer( _hbbson_iter_t_, iter );
            hb_storptrGC( phBson, 1 );
        }

        hb_retl( result );

    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_INT32 )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_INT32( iter ) ) {
        int32_t result = bson_iter_int32( iter );
        hb_retni( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_INT64 )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_INT64( iter ) ) {
        HB_LONGLONG result = bson_iter_int64( iter );
        hb_retnll( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_KEY )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        const char * key = bson_iter_key( iter );
        hb_retc( key );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_NEXT )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        bool result = bson_iter_next( iter );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_OID )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter && BSON_ITER_HOLDS_OID( iter ) ) {
        const bson_oid_t * oid = bson_iter_oid( iter );
        bson_oid_t * new = hb_xgrab( sizeof( bson_oid_t ) );
        bson_oid_copy( oid, new );
        PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_oid_t_, new );
        hb_retptrGC( phBson );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_TYPE )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        int type = bson_iter_type( iter );
        hb_retni( type );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_ITER_UTF8 )
{
    bson_iter_t * iter = bson_iter_hbparam( 1 );

    if ( iter ) {
        uint32_t length;
        const char * utfBuffer = bson_iter_utf8(iter, &length);
        if (HB_ISBYREF(2)) {
            hb_storni(length, 2);
        }
        hb_retc( utfBuffer );
    } else {
        HBBSON_ERR_ARGS();
    }
}
