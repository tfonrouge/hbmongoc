//
//  hb_mongoc.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 8/26/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc.h"

enum hb_return_value_type { _HBRETVAL_BSON_, _HBRETVAL_JSON_ };

static bool s_mongoc_inited = false;
static enum hb_return_value_type s_hbmongoc_returnValueType = _HBRETVAL_BSON_;

static const char * _STR_BSON_ = "BSON";
static const char * _STR_JSON_ = "JSON";

static HB_GARBAGE_FUNC( hbmongoc_funcs_destroy )
{
    PHB_MONGOC phMongoc = Cargo;

    if ( phMongoc && phMongoc->p ) {
        switch (phMongoc->type) {
            case _hb_client_t_:
                mongoc_client_destroy( ( mongoc_client_t * ) phMongoc->p );
                break;
            case _hb_database_t_:
                mongoc_database_destroy( ( mongoc_database_t * ) phMongoc->p );
                break;
            case _hb_collection_t_:
                mongoc_collection_destroy( ( mongoc_collection_t * ) phMongoc->p );
                break;
            case _hb_uri_t_:
                mongoc_uri_destroy( ( mongoc_uri_t * ) phMongoc->p );
                break;
        }
        phMongoc->p = NULL;
    }
}

static const HB_GC_FUNCS s_gc_mongoc_funcs = {
    hbmongoc_funcs_destroy,
    hb_gcDummyMark
};

PHB_MONGOC hbmongoc_new_dataContainer( void * p, hbmongoc_t_ type )
{
    PHB_MONGOC phMongo = hb_gcAllocate( sizeof( HB_MONGOC ), &s_gc_mongoc_funcs );

    phMongo->type = type;
    phMongo->p = p;

    return phMongo;
}

static void hbmongoc_check_inited()
{
    if( ! s_mongoc_inited )
        {
        mongoc_init();
        s_mongoc_inited = true;
        }
}

PHB_MONGOC hbmongoc_param( int iParam, hbmongoc_t_ type )
{
    PHB_MONGOC phMongo = hb_parptrGC( &s_gc_mongoc_funcs, iParam );

    return phMongo && phMongo->type == type ? phMongo : NULL;
}

void hbmongoc_return_byref_bson( int iParam, bson_t * bson )
{
    PHB_BSON pBson;
    char * szJson;

    switch ( s_hbmongoc_returnValueType ) {
        case _HBRETVAL_JSON_:
            szJson = bson_as_json( bson, NULL );
            hb_storc( szJson, iParam );
            bson_free( szJson );
            bson_destroy( bson );
            break;
        case _HBRETVAL_BSON_:
            pBson = hbbson_new_dataContainer( _hbbson_t_, bson );
            hb_storptrGC( pBson, iParam );
            break;
    }
}

void * mongoc_hbparam( int iParam, hbmongoc_t_ type )
{
    PHB_MONGOC phMongoc = hbmongoc_param( iParam, type );
    if ( phMongoc && phMongoc->p ) {
        return phMongoc->p;
    }
    return NULL;
}


/* Harbour API */

HB_FUNC( HBMONGOC_SETRETURNVALUETYPE )
{
    if ( hb_pcount() > 0 ) {
        if ( hb_stricmp( hb_parc( 1 ), _STR_JSON_ ) == 0 ) {
            s_hbmongoc_returnValueType = _HBRETVAL_JSON_;
        } else if ( hb_stricmp( hb_parc( 1 ), _STR_BSON_ ) == 0 ) {
            s_hbmongoc_returnValueType = _HBRETVAL_BSON_;
        } else {
            HBMONGOC_ERR_ARGS();
        }
    }
    switch ( s_hbmongoc_returnValueType ) {
        case _HBRETVAL_JSON_:
            hb_retc( _STR_JSON_ );
            break;
        case _HBRETVAL_BSON_:
            hb_retc( _STR_BSON_ );
            break;
    }
}

HB_FUNC( MONGOC_CLEANUP )
{
    if( s_mongoc_inited ) {
        mongoc_cleanup();
    }
}

HB_FUNC( MONGOC_INIT )
{
    hbmongoc_check_inited();
}
