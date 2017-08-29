//
//  hb_bson.h
//  hbmongoc
//
//  Created by Teo Fonrouge on 8/26/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#ifndef hb_bson_h
#define hb_bson_h

#include <stdio.h>
#include <bson.h>

typedef enum { _hb_bson_t_ } hbbson_t_;
typedef enum { _void_, _real_bson_, _json_hbstr_ } hbbson_origin_t_;

typedef struct _HB_BSON_
{
    hbbson_t_ type_t;
    void * p;
    hbbson_origin_t_ origin;

} HB_BSON, * PHB_BSON;

bson_t *    bson_param( int iParam );
PHB_BSON    hbbson_new_dataContainer( hbbson_t_ type, void * p );

#endif /* hb_bson_h */
