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

typedef enum { _hbbson_t_, _hbbson_decimal128_t_ } hbbson_t_;

typedef struct _HB_BSON_
{
    hbbson_t_ hbbson_type;
    bson_t * bson;
    bson_decimal128_t * bson_128;
    int documentLevel;
} HB_BSON, * PHB_BSON;

bson_decimal128_t * bson_decimal128_hbparam( int iParam );
bson_t *            bson_hbparam( int iParam, int hb_type );
PHB_BSON            hbbson_new_dataContainer( hbbson_t_ hbbson_type, void * p );
PHB_BSON            hbbson_param( int iParam, hbbson_t_ hbbson_type );

#endif /* hb_bson_h */
