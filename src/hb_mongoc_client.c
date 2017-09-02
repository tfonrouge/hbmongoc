//
//  hb_mongoc_client.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/1/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc_client.h"
#include "hb_mongoc.h"

HB_FUNC( MONGOC_CLIENT_COMMAND_SIMPLE )
{
    PHB_MONGOC client = hbmongoc_param( 1, _hb_client_t_ );
    bson_t * command = bson_hbparam( 3, HB_IT_POINTER | HB_IT_STRING | HB_IT_HASH );
    const char * db_name = hb_parc( 2 );

    if ( client && db_name && command && HB_ISBYREF( 5 ) ) {
        const mongoc_read_prefs_t * read_prefs = NULL;
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_client_command_simple( client->p, db_name, command, read_prefs, &reply, &error);

        hbmongoc_return_byref_bson( 5, bson_copy( &reply ) );

        if ( HB_ISBYREF( 6 ) ) {
            if ( result ) {
                hb_stor( 6 );
            } else {
                hb_storc( error.message, 6 );
            }
        }

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( command && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( command );
    }
}

HB_FUNC( MONGOC_CLIENT_DESTROY )
{
    PHB_MONGOC client = hbmongoc_param( 1, _hb_client_t_ );

    if( client ) {
        mongoc_client_destroy( client->p );
        client->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_GET_COLLECTION )
{
    PHB_MONGOC client = hbmongoc_param( 1, _hb_client_t_ );
    const char * db = hb_parc( 2 );
    const char * szCollection = hb_parc( 3 );

    if ( client && db && szCollection ) {
        mongoc_collection_t * collection = mongoc_client_get_collection( client->p, db, szCollection);
        if ( collection ) {
            PHB_MONGOC pMongoCollection = hbmongoc_new_dataContainer( collection, _hb_collection_t_ );
            hb_retptrGC( pMongoCollection );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_GET_DATABASE )
{
    PHB_MONGOC client = hbmongoc_param( 1, _hb_client_t_ );
    const char * name = hb_parc( 2 );

    if ( client && name ) {
        mongoc_database_t * database = mongoc_client_get_database( client->p, name );
        if ( database ) {
            PHB_MONGOC pMongoDatabase = hbmongoc_new_dataContainer( database, _hb_database_t_ );
            hb_retptrGC( pMongoDatabase );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_NEW )
{
    const char *uri_string = hb_parc( 1 );

    if ( uri_string ) {
        mongoc_client_t * client = mongoc_client_new( uri_string );
        if ( client ) {
            PHB_MONGOC phMongo = hbmongoc_new_dataContainer( client, _hb_client_t_ );
            hb_retptrGC( phMongo );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_CLIENT_NEW_FROM_URI )
{
    const mongoc_uri_t * uri = mongoc_hbparam( 1, _hb_uri_t_ );

    if ( uri ) {
        mongoc_client_t * client = mongoc_client_new_from_uri( uri );
        if ( client ) {
            PHB_MONGOC phMongo = hbmongoc_new_dataContainer( client, _hb_client_t_ );
            hb_retptrGC( phMongo );
        } else {
            hb_ret();
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

#if MONGOC_CHECK_VERSION( 1, 5, 0 )
HB_FUNC( MONGOC_CLIENT_SET_APPNAME )
{
    PHB_MONGOC pMongoc_client = hbmongoc_param( 1, _hb_client_t_ );
    const char * appname = hb_parc( 2 );

    if ( pMongoc_client && appname ) {
        bool result = mongoc_client_set_appname( pMongoc_client->p, appname );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
#endif
