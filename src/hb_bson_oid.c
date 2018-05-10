//
//  hb_bson_oid.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/16/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_bson_oid.h"

#include "hb_bson.h"

HB_FUNC( BSON_OID_COMPARE )
{
    const bson_oid_t * oid1 = bson_oid_hbparam( 1 );
    const bson_oid_t * oid2 = bson_oid_hbparam( 2 );

    if ( oid1 && oid2 ) {
        int result = bson_oid_compare( oid1, oid2 );
        hb_retni( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_OID_COPY )
{
    const bson_oid_t * src = bson_oid_hbparam( 1 );

    if ( src ) {
        bson_oid_t * dst = hb_xgrab( sizeof( bson_oid_t ) );
        bson_oid_copy( src, dst );
        PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_oid_t_, dst );
        if ( HB_ISBYREF( 2 ) ) {
            hb_storptrGC( phBson, 2 );
        }
        hb_retptrGC( phBson );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_OID_EQUAL )
{
    const bson_oid_t * oid1 = bson_oid_hbparam(1);
    const bson_oid_t * oid2 = bson_oid_hbparam(2);

    if (oid1 && oid2) {
        hb_retl(bson_oid_equal( oid1, oid2 ));
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_OID_GET_TIME_T )
{
    const bson_oid_t * oid = bson_oid_hbparam( 1 );

    if ( oid ) {
        time_t time = bson_oid_get_time_t( oid );
        hb_retnll( time );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_OID_HASH )
{
    const bson_oid_t * oid = bson_oid_hbparam( 1 );

    if ( oid ) {
        uint32_t hash = bson_oid_hash( oid );
        hb_retnl( hash );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_OID_INIT )
{
    if ( HB_ISBYREF( 1 ) ) {

        PHB_BSON phBson = hbbson_param( 1, _hbbson_oid_t_ );
        bson_oid_t * oid = NULL;

        if ( phBson ) {
            oid = phBson->p;
        } else {
            oid = hb_xgrab( sizeof( bson_oid_t ) );
        }

        bson_context_t * context = bson_context_hbparam( 2 );

        bson_oid_init( oid, context );

        if ( phBson == NULL ) {
            phBson = hbbson_new_dataContainer( _hbbson_oid_t_, oid );
            hb_storptrGC( phBson, 1 );
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_OID_INIT_FROM_DATA )
{
    const char * data = hb_parc( 2 );

    if ( HB_ISBYREF( 1 ) && data ) {

        PHB_BSON phBson = hbbson_param( 1, _hbbson_oid_t_ );
        bson_oid_t * oid = NULL;

        if ( phBson ) {
            oid = phBson->p;
        } else {
            oid = hb_xgrab( sizeof( bson_oid_t ) );
        }

        bson_oid_init_from_data( oid, ( const uint8_t * ) data );

        if ( phBson == NULL ) {
            phBson = hbbson_new_dataContainer( _hbbson_oid_t_, oid );
            hb_storptrGC( phBson, 1 );
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_OID_INIT_FROM_STRING )
{
    const char * str = hb_parc( 2 );

    if ( HB_ISBYREF( 1 ) && str ) {

        PHB_BSON phBson = hbbson_param( 1, _hbbson_oid_t_ );
        bson_oid_t * oid = NULL;

        if ( phBson ) {
            oid = phBson->p;
        } else {
            oid = hb_xgrab( sizeof( bson_oid_t ) );
        }

        bson_oid_init_from_string( oid, str );

        if ( phBson == NULL ) {
            phBson = hbbson_new_dataContainer( _hbbson_oid_t_, oid );
            hb_storptrGC( phBson, 1 );
        }
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_OID_IS_VALID )
{
    const char * str = hb_parc( 1 );

    if ( str ) {
        size_t length = hb_parnsdef( 2, hb_parclen( 1 ) );
        bool result = bson_oid_is_valid( str, length );
        hb_retl( result );
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_OID_TO_STRING )
{
    const bson_oid_t * oid = bson_oid_hbparam( 1 );

    if ( oid ) {
        char str[ 25 ];
        bson_oid_to_string( oid, str );
        if ( HB_ISBYREF( 2 ) ) {
            hb_storc( str, 2 );
        }
        hb_retc( str );
    } else {
        HBBSON_ERR_ARGS();
    }
}
