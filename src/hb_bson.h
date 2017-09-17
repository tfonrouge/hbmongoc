//
//  hb_bson.h
//  hbmongoc
//
//  Created by Teo Fonrouge on 8/26/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#ifndef hb_bson_h
#define hb_bson_h

#include "hbapi.h"
#include "hbapiitm.h"
#include "hbapierr.h"
#include "hbapifs.h"
#include "hbapistr.h"
#include "hbstack.h"
#include "hbvm.h"

#include <bson.h>

#define HBBSON_ERR_ARGS()  ( hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS ) )
#define HBBSON_ERR_NOFUNC()  ( hb_errRT_BASE_SubstR( EG_NOFUNC, 1001, "Undefined function", HB_ERR_FUNCNAME, 0 ) )

typedef enum
{
    _hbbson_t_,
    _hbbson_oid_t_,
    _hbbson_iter_t_,
    _hbbson_context_t_,
#if BSON_CHECK_VERSION( 1, 5, 0 )
    _hbbson_decimal128_t_
#endif
} hbbson_t_;

typedef struct _HB_BSON_
{
    hbbson_t_ hbbson_type;
    void * p;
} HB_BSON, * PHB_BSON;

bson_context_t *    bson_context_hbparam( int iParam );
#if BSON_CHECK_VERSION( 1, 5, 0 )
bson_decimal128_t * bson_decimal128_hbparam( int iParam );
#endif
bson_t *            bson_hbparam( int iParam, long lMask );
void                bson_hbstor_byref_error( int iParam, bson_error_t * error, HB_BOOL valid );
bson_oid_t *        bson_oid_hbparam( int iParam );
char *              hbbson_as_json( const bson_t * bson );
uint64_t            hbbson_dateTimeToUnix( PHB_ITEM pItem, HB_BOOL utc );
PHB_BSON            hbbson_new_dataContainer( hbbson_t_ hbbson_type, void * p );
PHB_BSON            hbbson_param( int iParam, hbbson_t_ hbbson_type );

#endif /* hb_bson_h */
