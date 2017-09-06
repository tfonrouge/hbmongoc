//
//  hb_mongoc.h
//  hbmongoc
//
//  Created by Teo Fonrouge on 8/25/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#ifndef hb_mongoc_h
#define hb_mongoc_h

#include "hb_bson.h"

#include "hbapi.h"
#include "hbapiitm.h"
#include "hbapierr.h"
#include "hbapifs.h"
#include "hbapistr.h"
#include "hbstack.h"
#include "hbvm.h"

#include <mongoc.h>
#include <bson.h>

typedef enum { _hb_client_t_, _hb_database_t_, _hb_collection_t_, _hb_uri_t_, _hb_cursor_t_ } hbmongoc_t_;

typedef struct _HB_MONGOC_
{
    hbmongoc_t_ type;
    void * p;

} HB_MONGOC, * PHB_MONGOC;

PHB_MONGOC  hbmongoc_param( int iParam, hbmongoc_t_ type );
PHB_MONGOC  hbmongoc_new_dataContainer( void * p, hbmongoc_t_ type );
PHB_BSON    hbmongoc_return_byref_bson( int iParam, bson_t * bson );
void *      mongoc_hbparam( int iParam, hbmongoc_t_ type );

#define HBMONGOC_ERR_ARGS()  ( hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS ) )

#endif /* hb_mongoc_h */
