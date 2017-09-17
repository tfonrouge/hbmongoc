//
//  hb_bson_context.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/16/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_bson_context.h"

#include "hb_bson.h"

HB_FUNC( BSON_CONTEXT_DESTROY )
{
    PHB_BSON phBson = hbbson_param( 1, _hbbson_context_t_ );

    if ( phBson ) {
        bson_context_destroy( phBson->p );
        phBson->p = NULL;
    } else {
        HBBSON_ERR_ARGS();
    }
}

HB_FUNC( BSON_CONTEXT_GET_DEFAULT )
{
    bson_context_t * context = bson_context_get_default();
    PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_context_t_, context );
    hb_retptrGC( phBson );
}

HB_FUNC( BSON_CONTEXT_NEW )
{
    if ( HB_ISNUM( 1 ) ) {
        bson_context_flags_t flags = hb_parni( 1 );
        bson_context_t * context = bson_context_new( flags );
        PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_context_t_, context );
        hb_retptrGC( phBson );
    } else {
        HBBSON_ERR_ARGS();
    }
}
