//
//  hb_mongoc_collection.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/1/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc_collection.h"
#include "hb_mongoc.h"

HB_FUNC( MONGOC_COLLECTION_CREATE_BULK_OPERATION )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );

    if ( collection && HB_ISLOG( 2 ) ) {
        HB_BOOL ordered = hb_parl( 2 );
        const mongoc_write_concern_t * write_concern = mongoc_hbparam( 3, _hbmongoc_write_concern_t_ );
        mongoc_bulk_operation_t * bulk = mongoc_collection_create_bulk_operation( collection, ordered, write_concern );
        PHB_MONGOC phMongo = hbmongoc_new_dataContainer( _hbmongoc_bult_operation_t_, bulk );
        hb_retptrGC( phMongo );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_COMMAND_SIMPLE )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * command = bson_hbparam( 2, HB_IT_ANY );

    if ( collection && command && HB_ISBYREF( 4 ) ) {

        const mongoc_read_prefs_t *read_prefs = mongoc_hbparam( 3, _hbmongoc_read_prefs_t_ );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_collection_command_simple( collection, command, read_prefs, &reply, &error);

        hbmongoc_return_byref_bson( 4, bson_copy( &reply ) );

        if ( HB_ISBYREF( 5 ) ) {
            if ( result ) {
                hb_stor( 5 );
            } else {
                bson_hbstor_ref_error( 5, &error );
            }
        }

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( command && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( command );
    }
}

HB_FUNC( MONGOC_COLLECTION_DESTROY )
{
    PHB_MONGOC collection = hbmongoc_param( 1, _hbmongoc_collection_t_ );

    if( collection ) {
        mongoc_collection_destroy( collection->p );
        collection->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_DROP )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );

    if ( collection ) {
        bson_error_t error;

        bool result = mongoc_collection_drop( collection, &error );

        if ( HB_ISBYREF( 2 ) ) {
            if ( result ) {
                hb_stor( 2 );
            } else {
                bson_hbstor_ref_error( 2, &error );
            }
        }

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_DROP_WITH_OPTS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * opts = bson_hbparam( 2, HB_IT_ANY );

    if ( collection && opts ) {
        bson_error_t error;

        bool result = mongoc_collection_drop_with_opts( collection, opts, &error );

        if ( HB_ISBYREF( 3 ) ) {
            if ( result ) {
                hb_stor( 3 );
            } else {
                bson_hbstor_ref_error( 3, &error );
            }
        }

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( opts && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( opts );
    }
}

HB_FUNC( MONGOC_COLLECTION_FIND_WITH_OPTS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * filter = bson_hbparam( 2, HB_IT_ANY );
    bson_t * opts = bson_hbparam( 3, HB_IT_ANY );

    if ( collection && filter && opts ) {
        const mongoc_read_prefs_t *read_prefs = mongoc_hbparam( 4, _hbmongoc_read_prefs_t_ );

        mongoc_cursor_t * cursor = mongoc_collection_find_with_opts( collection, filter, opts, read_prefs );

        PHB_MONGOC phCursor = hbmongoc_new_dataContainer( _hbmongoc_cursor_t_, cursor );

        hb_retptrGC( phCursor );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( filter && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( filter );
    }

    if ( opts && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( opts );
    }
}

HB_FUNC( MONGOC_COLLECTION_INSERT )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * document = bson_hbparam( 3, HB_IT_ANY );

    if ( collection && document ) {
        mongoc_insert_flags_t flags = HB_ISNUM( 2 ) ? hb_parni( 2 ) : MONGOC_INSERT_NONE;
        const mongoc_write_concern_t * write_concern = mongoc_hbparam( 4, _hbmongoc_write_concern_t_ );
        bson_error_t error;

        bool result = mongoc_collection_insert( collection, flags, document, write_concern, &error );

        if ( HB_ISBYREF( 5 ) ) {
            if ( result ) {
                hb_stor( 5 );
            } else {
                bson_hbstor_ref_error( 5, &error );
            }
        }

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }
    
    if ( document && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( document );
    }
}

HB_FUNC( MONGOC_COLLECTION_REMOVE )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * selector = bson_hbparam( 3, HB_IT_ANY );

    if ( collection && selector ) {
        int flags = HB_ISNUM( 2 ) ? hb_parni( 2 ) : MONGOC_REMOVE_NONE;
        const mongoc_write_concern_t * write_concern = mongoc_hbparam( 4, _hbmongoc_write_concern_t_ );
        bson_error_t error;

        bool result = mongoc_collection_remove( collection, flags, selector, write_concern, &error );

        if ( HB_ISBYREF( 5 ) ) {
            if ( result ) {
                hb_stor( 5 );
            } else {
                bson_hbstor_ref_error( 5, &error );
            }
        }

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( selector );
    }
}
