//
//  hb_mongoc.h
//  hbmongoc
//
//  Created by Teo Fonrouge on 8/25/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#ifndef hb_mongoc_h
#define hb_mongoc_h

#include <stdio.h>
#include <bson.h>

typedef enum { _hb_client_t_, _hb_database_t_, _hb_collection_t_ } hbmongoc_t_;

typedef struct _HB_MONGOC_
{
    hbmongoc_t_ type_t;
    void * p;

} HB_MONGOC, * PHB_MONGOC;

static PHB_MONGOC hbmongoc_new_dataContainer( hbmongoc_t_ type, void * p );
void hbmongoc_return_byref_bson( int iParam, bson_t * bson );

#endif /* hb_mongoc_h */
