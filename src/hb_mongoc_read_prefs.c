//
//  hb_mongoc_read_prefs.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/7/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc_read_prefs.h"
#include "hb_mongoc.h"

HB_FUNC( MONGOC_READ_PREFS_ADD_TAG )
{
    mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 1, _hbmongoc_read_prefs_t_ );
    bson_t * tag = bson_hbparam( 2, HB_IT_ANY );

    if ( read_prefs && tag ) {
        mongoc_read_prefs_add_tag( read_prefs, tag );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( tag && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( tag );
    }
}

HB_FUNC( MONGOC_READ_PREFS_COPY )
{
    const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 1, _hbmongoc_read_prefs_t_ );

    if ( read_prefs ) {
        mongoc_read_prefs_t * copy = mongoc_read_prefs_copy( read_prefs );
        PHB_MONGOC phRead_prefs = hbmongoc_new_dataContainer( _hbmongoc_read_prefs_t_, copy );
        hb_retptrGC( phRead_prefs );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_READ_PREFS_GET_MAX_STALENESS_SECONDS )
#if MONGOC_CHECK_VERSION( 1, 5, 0 )
{
    const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 1, _hbmongoc_read_prefs_t_ );

    if ( read_prefs ) {
        HB_LONGLONG staleness = mongoc_read_prefs_get_max_staleness_seconds( read_prefs );
        hb_retnll( staleness );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
#else
{
    HBMONGOC_ERR_NOFUNC();
}
#endif

HB_FUNC( MONGOC_READ_PREFS_GET_MODE )
{
    const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 1, _hbmongoc_read_prefs_t_ );

    if ( read_prefs ) {
        int read_mode = mongoc_read_prefs_get_mode( read_prefs );
        hb_retni( read_mode );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_READ_PREFS_GET_TAGS )
{
    const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 1, _hbmongoc_read_prefs_t_ );

    if ( read_prefs ) {
        const bson_t * read_mode = mongoc_read_prefs_get_tags( read_prefs );
        PHB_BSON phBson = hbbson_new_dataContainer( _hbbson_t_, bson_copy( read_mode ) );
        hb_retptrGC( phBson );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_READ_PREFS_IS_VALID )
{
    const mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 1, _hbmongoc_read_prefs_t_ );

    if ( read_prefs ) {
        bool is_valid = mongoc_read_prefs_is_valid( read_prefs );
        hb_retl( is_valid );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_READ_PREFS_NEW )
{
    if ( HB_ISNUM( 1 ) ) {
        int read_mode = hb_parni( 1 );
        mongoc_read_prefs_t * read_prefs = mongoc_read_prefs_new( read_mode );
        PHB_MONGOC phRead_prefs = hbmongoc_new_dataContainer( _hbmongoc_read_prefs_t_, read_prefs );
        hb_retptrGC( phRead_prefs );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_READ_PREFS_SET_MAX_STALENESS_SECONDS )
#if MONGOC_CHECK_VERSION( 1, 5, 0 )
{
    mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 1, _hbmongoc_read_prefs_t_ );

    if ( read_prefs && HB_ISNUM( 2 ) ) {
        HB_LONGLONG max_staleness_seconds = hb_parnll( 2 );
        mongoc_read_prefs_set_max_staleness_seconds( read_prefs, max_staleness_seconds );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
#else
{
    HBMONGOC_ERR_NOFUNC();
}
#endif

HB_FUNC( MONGOC_READ_PREFS_SET_MODE )
{
    mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 1, _hbmongoc_read_prefs_t_ );

    if ( read_prefs && HB_ISNUM( 2 ) ) {
        int max_staleness_seconds = hb_parni( 2 );
        mongoc_read_prefs_set_mode( read_prefs, max_staleness_seconds );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_READ_PREFS_SET_TAGS )
{
    mongoc_read_prefs_t * read_prefs = mongoc_hbparam( 1, _hbmongoc_read_prefs_t_ );
    bson_t * tags = bson_hbparam( 2, HB_IT_ANY );

    if ( read_prefs && HB_ISNUM( 2 ) ) {
        mongoc_read_prefs_set_tags( read_prefs, tags );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( tags && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( tags );
    }
}
