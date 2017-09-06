//
//  hb_mongoc_database.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/1/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc_database.h"
#include "hb_mongoc.h"

HB_FUNC( MONGOC_DATABASE_DESTROY )
{
    PHB_MONGOC database = hbmongoc_param( 1, _hb_database_t_ );

    if( database ) {
        mongoc_database_destroy( database->p );
        database->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_DATABASE_GET_COLLECTION )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hb_database_t_ );
    const char * name = hb_parc( 2 );

    if ( database && name ) {
        mongoc_collection_t * collection = mongoc_database_get_collection( database, name );
        if ( collection ) {
            PHB_MONGOC phCollection = hbmongoc_new_dataContainer( collection, _hb_collection_t_ );
            hb_retptrGC( phCollection );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
