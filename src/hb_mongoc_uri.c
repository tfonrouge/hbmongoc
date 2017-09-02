//
//  hb_mongoc_uri.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/1/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc_uri.h"
#include "hb_mongoc.h"

HB_FUNC( MONGOC_URI_NEW )
{
    const char * uri_string = hb_parc( 1 );

    if ( uri_string ) {
        mongoc_uri_t * uri = mongoc_uri_new( uri_string );
        if ( uri ) {
            PHB_MONGOC phMongo = hbmongoc_new_dataContainer( uri, _hb_uri_t_ );
            hb_retptrGC( phMongo );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
