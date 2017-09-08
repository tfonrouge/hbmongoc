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
    PHB_MONGOC database = hbmongoc_param( 1, _hbmongoc_database_t_ );

    if( database ) {
        mongoc_database_destroy( database->p );
        database->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_DATABASE_GET_COLLECTION )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    const char * name = hb_parc( 2 );

    if ( database && name ) {
        mongoc_collection_t * collection = mongoc_database_get_collection( database, name );
        if ( collection ) {
            PHB_MONGOC phCollection = hbmongoc_new_dataContainer( _hbmongoc_collection_t_, collection );
            hb_retptrGC( phCollection );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_DATABASE_GET_COLLECTION_NAMES )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );

    if ( database ) {
        bson_error_t error;

        char ** names = mongoc_database_get_collection_names( database, &error );

        PHB_ITEM pItemArray = hb_itemNew( NULL );
        hb_arrayNew( pItemArray, 0 );
        for ( int i = 0; names[ i ]; ++i ) {
            PHB_ITEM pItem = hb_itemNew( NULL );
            hb_itemPutC( pItem, names[ i ] );
            hb_arrayAdd( pItemArray, pItem );
            hb_itemRelease( pItem );
        }

        bson_strfreev( names );

        if ( HB_ISBYREF( 2 ) ) {
            if ( error.code == 0 && error.domain == 0 && strlen( error.message ) == 0 ) {
                hb_stor( 2 );
            } else {
                bson_hbstor_ref_error( 2, &error );
            }
        }

        hb_itemReturnRelease( pItemArray );

    } else {
        HBMONGOC_ERR_ARGS();
    }
}
