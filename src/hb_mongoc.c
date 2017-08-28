//
//  hb_mongoc.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 8/26/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc.h"
#include "hb_bson.h"

#include "hbapi.h"
#include "hbapiitm.h"
#include "hbapierr.h"
#include "hbapifs.h"
#include "hbapistr.h"
#include "hbstack.h"
#include "hbvm.h"

#include <mongoc.h>

#define MONGOC_ERR_ARGS()  ( hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS ) )

static bool s_mongoc_inited = false;
static char * s_hbmongoc_returnValueType = "H";

/*
 hbmongoc_client_destroy
 */
static HB_GARBAGE_FUNC( hbmongoc_funcs_destroy )
{
    PHB_MONGOC pMongo = Cargo;

    if ( pMongo && pMongo->p ) {
        switch (pMongo->type_t) {
            case _hb_client_t_:
                mongoc_client_destroy( ( mongoc_client_t * ) pMongo->p );
                break;
            case _hb_database_t_:
                mongoc_database_destroy( ( mongoc_database_t * ) pMongo->p );
                break;
            case _hb_collection_t_:
                mongoc_collection_destroy( ( mongoc_collection_t * ) pMongo->p );
                break;
        }
        pMongo->p = NULL;
    }
}

/*
 s_gc_mongoc_client_t
 */
static const HB_GC_FUNCS s_gc_mongoc_funcs = {
    hbmongoc_funcs_destroy,
    hb_gcDummyMark
};

static PHB_MONGOC hbmongoc_new_dataContainer( hbmongoc_t_ type, void * p )
{
    PHB_MONGOC pMongoc = hb_gcAllocate( sizeof( HB_MONGOC ), &s_gc_mongoc_funcs );

    pMongoc->type_t = type;
    pMongoc->p = p;

    return pMongoc;
}

/*
 hbmongoc_check_inited
 */
static void hbmongoc_check_inited()
{
    if( ! s_mongoc_inited )
        {
        mongoc_init();
        s_mongoc_inited = true;
        }
}

/*
 hbmongoc_param
 */
static void * hbmongoc_param( int iParam, hbmongoc_t_ type )
{
    PHB_MONGOC pMongoc = hb_parptrGC( &s_gc_mongoc_funcs, iParam );

    return pMongoc && pMongoc->type_t == type ? pMongoc : NULL;
}

/*
 hbmongoc_return_byref_bson
 */
void hbmongoc_return_byref_bson( int iParam, bson_t * bson )
{
    PHB_BSON pBson;
    char * szJson;

    switch ( s_hbmongoc_returnValueType[ 0 ] ) {
        case 'H':
            szJson = bson_as_json( bson, NULL );
            hb_storc( szJson, iParam );
            bson_free( szJson );
            bson_destroy( bson );
            break;
        case 'B':
            pBson = hbbson_new_dataContainer( _hb_bson_t_, bson );
            hb_storptrGC( pBson, iParam );
            break;
        default:
            break;
    }
}


/* Harbour API */

HB_FUNC( HBMONGOC_SETRETURNVALUETYPE )
{
    if ( hb_pcount() > 0 ) {
        if ( hb_stricmp( hb_parc( 1 ), "HARBOUR" ) == 0 ) {
            s_hbmongoc_returnValueType = "H";
        } else if ( hb_stricmp( hb_parc( 1 ), "BSON" ) == 0 ) {
            s_hbmongoc_returnValueType = "B";
        } else {
            MONGOC_ERR_ARGS();
        }
    }
    switch (s_hbmongoc_returnValueType[0]) {
        case 'H':
            hb_retc( "Harbour" );
            break;
        case 'B':
            hb_retc( "BSON" );
            break;
        default:
            break;
    }
}

/*
 mongoc_init
 */
HB_FUNC( MONGOC_INIT )
{
    hbmongoc_check_inited();
}

/*
 mongoc_cleanup
 */
HB_FUNC( MONGOC_CLEANUP )
{
    if( s_mongoc_inited ) {
        mongoc_cleanup();
    }
}

/*
 mongoc_client_command_simple
 */
HB_FUNC( MONGOC_CLIENT_COMMAND_SIMPLE )
{
    PHB_MONGOC pMongoc_client = hbmongoc_param( 1, _hb_client_t_ );
    PHB_BSON pBson_command = hbbson_param( 3, _hb_bson_t_ );

    if ( pMongoc_client && pBson_command ) {
        const char *db_name = hb_parc( 2 );
        const mongoc_read_prefs_t *read_prefs = NULL;
        bson_t * reply = bson_new();
        bson_error_t error;

        bool result = mongoc_client_command_simple( pMongoc_client->p, db_name, pBson_command->p, read_prefs, reply, &error);

        if ( result && HB_ISBYREF( 5 ) ) {
            hbmongoc_return_byref_bson( 5, reply );
        } else {
            bson_destroy( reply );
        }

        if ( HB_ISBYREF( 6 ) ) {
            if ( result ) {
                hb_stor( 6 );
            } else {
                hb_storc( error.message, 6 );
            }
        }

        hb_retl( result );

    } else {
        MONGOC_ERR_ARGS();
    }
}

/*
 mongoc_client_new
 */
HB_FUNC( MONGOC_CLIENT_NEW )
{
    if ( HB_ISCHAR( 1 ) ) {
        const char *uri_string = hb_parc( 1 );
        mongoc_client_t * client = mongoc_client_new( uri_string );
        if ( client ) {
            PHB_MONGOC pMongoc = hbmongoc_new_dataContainer( _hb_client_t_, client );
            hb_retptrGC( pMongoc );
        } else {
            hb_ret();
        }
    } else {
        MONGOC_ERR_ARGS();
    }
}

/*
 mongoc_client_get_collection
 */
HB_FUNC( MONGOC_CLIENT_GET_COLLECTION )
{
    PHB_MONGOC pMongoc_client = hbmongoc_param( 1, _hb_client_t_ );

    if ( pMongoc_client && HB_ISCHAR( 2 ) && HB_ISCHAR( 3 ) ) {
        const char *db = hb_parc( 2 );
        const char *szCollection = hb_parc( 3 );
        mongoc_collection_t * collection = mongoc_client_get_collection( pMongoc_client->p, db, szCollection);
        PHB_MONGOC pMongoCollection = hbmongoc_new_dataContainer( _hb_collection_t_, collection );
        hb_retptrGC( pMongoCollection );
    } else {
        MONGOC_ERR_ARGS();
    }
}

/*
 mongoc_collection_destroy
 */
HB_FUNC( MONGOC_COLLECTION_DESTROY )
{
    PHB_MONGOC pMongoc_collection = hbmongoc_param( 1, _hb_collection_t_ );

    if( pMongoc_collection ) {
        mongoc_collection_destroy( pMongoc_collection->p );
        pMongoc_collection->p = NULL;
    } else {
        MONGOC_ERR_ARGS();
    }
}

/*
 mongoc_client_destroy
 */
HB_FUNC( MONGOC_CLIENT_DESTROY )
{
    PHB_MONGOC pMongoc_client = hbmongoc_param( 1, _hb_client_t_ );

    if( pMongoc_client ) {
        mongoc_client_destroy( pMongoc_client->p );
        pMongoc_client->p = NULL;
    } else {
        MONGOC_ERR_ARGS();
    }
}

/*
 mongoc_client_get_database
 */
HB_FUNC( MONGOC_CLIENT_GET_DATABASE )
{
    PHB_MONGOC pMongoc_client = hbmongoc_param( 1, _hb_client_t_ );

    if ( pMongoc_client && HB_ISCHAR( 2 ) ) {
        const char *name = hb_parc( 2 );
        mongoc_database_t * database = mongoc_client_get_database( pMongoc_client->p, name );
        PHB_MONGOC pMongoDatabase = hbmongoc_new_dataContainer( _hb_database_t_, database );
        hb_retptrGC( pMongoDatabase );
    } else {
        MONGOC_ERR_ARGS();
    }
}

/*
 mongoc_client_set_appname
 */
HB_FUNC( MONGOC_CLIENT_SET_APPNAME )
{
    PHB_MONGOC pMongoc_client = hbmongoc_param( 1, _hb_client_t_ );

    if ( pMongoc_client && HB_ISCHAR( 2 ) ) {
        const char *appname = hb_parc( 2 );
        bool result = mongoc_client_set_appname( pMongoc_client->p, appname );
        hb_retl( result );
    } else {
        MONGOC_ERR_ARGS();
    }
}

/*
 mongoc_collection_command_simple
 */
HB_FUNC( MONGOC_COLLECTION_COMMAND_SIMPLE )
{
    PHB_MONGOC pMongoc_collection = hbmongoc_param( 1, _hb_collection_t_ );
    PHB_BSON pBson_command = hbbson_param( 2, _hb_bson_t_ );

    if ( pMongoc_collection && pBson_command ) {
        
        const mongoc_read_prefs_t *read_prefs = NULL;
        bson_t * reply = bson_new();
        bson_error_t error;

        bool result = mongoc_collection_command_simple( pMongoc_collection->p, pBson_command->p, read_prefs, reply, &error);

        if ( result && HB_ISBYREF( 4 ) ) {
            hbmongoc_return_byref_bson( 4, reply );
        } else {
            bson_destroy( reply );
        }

        if ( HB_ISBYREF( 5 ) ) {
            if ( result ) {
                hb_stor( 5 );
            } else {
                hb_storc( error.message, 5 );
            }
        }

        hb_retl( result );

    } else {
        MONGOC_ERR_ARGS();
    }
}

/*
 mongoc_collection_insert
 */
HB_FUNC( MONGOC_COLLECTION_INSERT )
{
    PHB_MONGOC pMongoc_collection = hbmongoc_param( 1, _hb_collection_t_ );
    PHB_BSON pBson_document = hbbson_param( 3, _hb_bson_t_ );
    bool result = false;

    if ( pMongoc_collection && pBson_document ) {

        mongoc_insert_flags_t flags;

        if ( HB_ISNUM( 2 ) ) {
            flags = hb_parni( 2 );
        } else {
            flags = MONGOC_INSERT_NONE;
        }

        const mongoc_write_concern_t * write_concern = NULL;
        bson_error_t error;

        result = mongoc_collection_insert( pMongoc_collection->p, flags, pBson_document->p, write_concern, &error );

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

        MONGOC_ERR_ARGS();

    }
}

/*
 mongoc_database_destroy
 */
HB_FUNC( MONGOC_DATABASE_DESTROY )
{
    PHB_MONGOC pMongoc_database = hbmongoc_param( 1, _hb_database_t_ );
    
    if( pMongoc_database ) {
        mongoc_database_destroy( pMongoc_database->p );
        pMongoc_database->p = NULL;
    } else {
        MONGOC_ERR_ARGS();
    }
}
