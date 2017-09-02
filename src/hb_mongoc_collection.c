//
//  hb_mongoc_collection.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/1/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc_collection.h"
#include "hb_mongoc.h"

HB_FUNC( MONGOC_COLLECTION_COMMAND_SIMPLE )
{
    PHB_MONGOC collection = hbmongoc_param( 1, _hb_collection_t_ );
    bson_t * command = bson_hbparam( 2, HB_IT_POINTER | HB_IT_STRING );

    if ( collection && command && HB_ISBYREF( 4 ) ) {

        const mongoc_read_prefs_t *read_prefs = NULL;
        bson_t * reply = bson_new();
        bson_error_t error;

        bool result = mongoc_collection_command_simple( collection->p, command, read_prefs, reply, &error);

        hbmongoc_return_byref_bson( 4, reply );

        if ( HB_ISBYREF( 5 ) ) {
            if ( result ) {
                hb_stor( 5 );
            } else {
                hb_storc( error.message, 5 );
            }
        }

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( command && HB_ISCHAR( 2 ) ) {
        bson_destroy( command );
    }
}

HB_FUNC( MONGOC_COLLECTION_DESTROY )
{
    PHB_MONGOC collection = hbmongoc_param( 1, _hb_collection_t_ );

    if( collection ) {
        mongoc_collection_destroy( collection->p );
        collection->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_INSERT )
{
    PHB_MONGOC collection = hbmongoc_param( 1, _hb_collection_t_ );
    bson_t * document = bson_hbparam( 3, HB_IT_POINTER | HB_IT_STRING );
    bool result = false;

    if ( collection && document ) {

        mongoc_insert_flags_t flags;

        if ( HB_ISNUM( 2 ) ) {
            flags = hb_parni( 2 );
        } else {
            flags = MONGOC_INSERT_NONE;
        }

        const mongoc_write_concern_t * write_concern = NULL;
        bson_error_t error;

        result = mongoc_collection_insert( collection->p, flags, document, write_concern, &error );

        if ( HB_ISBYREF( 5 ) ) {
            if ( result ) {
                hb_stor( 5 );
            } else {
                const char *szError = error.message;
                hb_storc( szError, 5 );
            }
        }

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }
    
    if ( document && HB_ISCHAR( 3 ) ) {
        bson_destroy( document );
    }
}
