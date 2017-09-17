//
//  hb_bson_iter.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/17/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_bson_iter.h"

#include "hb_bson.h"

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
