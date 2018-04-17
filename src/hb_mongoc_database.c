//
//  hb_mongoc_database.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/1/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc_database.h"
#include "hb_mongoc.h"

HB_FUNC( MONGOC_DATABASE_CREATE_COLLECTION )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    const char * name = hb_parc( 2 );
    bson_t * opts = bson_hbparam( 3, HB_IT_ANY );

    if ( database && name ) {
        bson_error_t error;

        mongoc_collection_t * collection = mongoc_database_create_collection( database, name, opts, &error );

        bson_hbstor_byref_error( 4, &error, collection != NULL );

        PHB_MONGOC phCollection = hbmongoc_new_dataContainer( _hbmongoc_collection_t_, collection );
        hb_retptrGC( phCollection );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( opts && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( opts );
    }
}

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
    bson_t * opts = bson_hbparam( 2, HB_IT_ANY );

    if ( database ) {
        bson_error_t error;
        bson_set_error( &error, 0, 0, "%s", "" );

        char ** names = mongoc_database_get_collection_names_with_opts(database, opts, &error);

        bson_hbstor_byref_error( 3, &error, false );

        if ( names ) {
            PHB_ITEM pItemArray = hb_itemNew( NULL );
            hb_arrayNew( pItemArray, 0 );

            for ( int i = 0; names[ i ]; ++i ) {
                PHB_ITEM pItem = hb_itemNew( NULL );
                hb_itemPutC( pItem, names[ i ] );
                hb_arrayAdd( pItemArray, pItem );
                hb_itemRelease( pItem );
            }

            bson_strfreev( names );

            hb_itemReturnRelease( pItemArray );
        } else {
            hb_ret();
        }

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( opts && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( opts );
    }
}

HB_FUNC( MONGOC_DATABASE_WRITE_COMMAND_WITH_OPTS )
#if MONGOC_CHECK_VERSION( 1, 9, 0 )
{
    mongoc_database_t * database = mongoc_hbparam( 1, _hbmongoc_database_t_ );
    bson_t * command = bson_hbparam( 2, HB_IT_ANY );
    bson_t * opts = bson_hbparam( 3, HB_IT_ANY );

    if ( database && command ) {
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_database_write_command_with_opts( database, command, opts, &reply, &error );

        hbmongoc_return_byref_bson( 4, bson_copy( &reply ) );
        bson_destroy(&reply);
        bson_hbstor_byref_error( 5, &error, result );

        hb_retl( result );
        
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( command && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( command );
    }

    if ( opts && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( opts );
    }
}
#else
{
    HBMONGOC_ERR_NOFUNC();
}
#endif
