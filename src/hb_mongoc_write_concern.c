//
//  hb_mongoc_write_concern.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/7/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc_write_concern.h"
#include "hb_mongoc.h"

HB_FUNC( MONGOC_WRITE_CONCERN_APPEND )
#if MONGOC_CHECK_VERSION( 1, 5, 0 )
{
    mongoc_write_concern_t * write_concern = mongoc_hbparam( 1, _hbmongoc_write_concern_t_ );
    bson_t * doc = bson_hbparam( 2, HB_IT_POINTER );

    if ( write_concern && doc ) {
        bool result = mongoc_write_concern_append( write_concern, doc );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
#else
{
    HBMONGOC_ERR_NOFUNC();
}
#endif

HB_FUNC( MONGOC_WRITE_CONCERN_COPY )
{
    mongoc_write_concern_t * write_concern = mongoc_hbparam( 1, _hbmongoc_write_concern_t_ );

    if ( write_concern ) {
        mongoc_write_concern_t * write_concern_copy = mongoc_write_concern_copy( write_concern );
        PHB_MONGOC phWrite_concern = hbmongoc_new_dataContainer( _hbmongoc_write_concern_t_, write_concern_copy );
        hb_retptrGC( phWrite_concern );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_WRITE_CONCERN_GET_JOURNAL )
{
    mongoc_write_concern_t * write_concern = mongoc_hbparam( 1, _hbmongoc_write_concern_t_ );

    if ( write_concern ) {
        bool result = mongoc_write_concern_get_journal( write_concern );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_WRITE_CONCERN_GET_W )
{
    mongoc_write_concern_t * write_concern = mongoc_hbparam( 1, _hbmongoc_write_concern_t_ );

    if ( write_concern ) {
        uint32_t w = mongoc_write_concern_get_w( write_concern );
        hb_retni( w );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_WRITE_CONCERN_GET_WMAJORITY )
{
    mongoc_write_concern_t * write_concern = mongoc_hbparam( 1, _hbmongoc_write_concern_t_ );

    if ( write_concern ) {
        bool result = mongoc_write_concern_get_wmajority( write_concern );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_WRITE_CONCERN_GET_WTAG )
{
    mongoc_write_concern_t * write_concern = mongoc_hbparam( 1, _hbmongoc_write_concern_t_ );

    if ( write_concern ) {
        const char * wtag = mongoc_write_concern_get_wtag( write_concern );
        hb_retc( wtag );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_WRITE_CONCERN_GET_WTIMEOUT )
{
    mongoc_write_concern_t * write_concern = mongoc_hbparam( 1, _hbmongoc_write_concern_t_ );

    if ( write_concern ) {
        uint32_t w = mongoc_write_concern_get_wtimeout( write_concern );
        hb_retni( w );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_WRITE_CONCERN_IS_ACKNOWLEDGED )
#if MONGOC_CHECK_VERSION( 1, 5, 0 )
{
    mongoc_write_concern_t * write_concern = mongoc_hbparam( 1, _hbmongoc_write_concern_t_ );

    if ( write_concern ) {
        bool result = mongoc_write_concern_is_acknowledged( write_concern );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
#else
{
    HBMONGOC_ERR_NOFUNC();
}
#endif

HB_FUNC( MONGOC_WRITE_CONCERN_IS_DEFAULT )
#if MONGOC_CHECK_VERSION( 1, 7, 0 )
{
    mongoc_write_concern_t * write_concern = mongoc_hbparam( 1, _hbmongoc_write_concern_t_ );

    if ( write_concern ) {
        bool result = mongoc_write_concern_is_default( write_concern );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
#else
{
    HBMONGOC_ERR_NOFUNC();
}
#endif

HB_FUNC( MONGOC_WRITE_CONCERN_IS_VALID )
#if MONGOC_CHECK_VERSION( 1, 5, 0 )
{
    mongoc_write_concern_t * write_concern = mongoc_hbparam( 1, _hbmongoc_write_concern_t_ );

    if ( write_concern ) {
        bool result = mongoc_write_concern_is_valid( write_concern );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
#else
{
    HBMONGOC_ERR_NOFUNC();
}
#endif

HB_FUNC( MONGOC_WRITE_CONCERN_JOURNAL_IS_SET )
#if MONGOC_CHECK_VERSION( 1, 5, 0 )
{
    mongoc_write_concern_t * write_concern = mongoc_hbparam( 1, _hbmongoc_write_concern_t_ );

    if ( write_concern ) {
        bool result = mongoc_write_concern_journal_is_set( write_concern );
        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
#else
{
    HBMONGOC_ERR_NOFUNC();
}
#endif

HB_FUNC( MONGOC_WRITE_CONCERN_SET_JOURNAL )
{
    mongoc_write_concern_t * write_concern = mongoc_hbparam( 1, _hbmongoc_write_concern_t_ );

    if ( write_concern && HB_ISLOG( 2 ) ) {
        HB_BOOL journal = hb_parl( 2 );
        mongoc_write_concern_set_journal( write_concern, journal );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_WRITE_CONCERN_SET_W )
{
    mongoc_write_concern_t * write_concern = mongoc_hbparam( 1, _hbmongoc_write_concern_t_ );

    if ( write_concern && HB_ISNUM( 2 ) ) {
        uint32_t w = hb_parni( 2 );
        mongoc_write_concern_set_w( write_concern, w );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_WRITE_CONCERN_SET_WMAJORITY )
{
    mongoc_write_concern_t * write_concern = mongoc_hbparam( 1, _hbmongoc_write_concern_t_ );

    if ( write_concern && HB_ISNUM( 2 ) ) {
        uint32_t wtimeout_msec = hb_parni( 2 );
        mongoc_write_concern_set_wmajority( write_concern, wtimeout_msec );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_WRITE_CONCERN_SET_WTAG )
{
    mongoc_write_concern_t * write_concern = mongoc_hbparam( 1, _hbmongoc_write_concern_t_ );
    const char * tag = hb_parc( 2 );

    if ( write_concern && tag ) {
        mongoc_write_concern_set_wtag( write_concern, tag );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_WRITE_CONCERN_SET_WTIMEOUT )
{
    mongoc_write_concern_t * write_concern = mongoc_hbparam( 1, _hbmongoc_write_concern_t_ );

    if ( write_concern && HB_ISNUM( 2 ) ) {
        uint32_t wtimeout_msec = hb_parni( 2 );
        mongoc_write_concern_set_wtimeout( write_concern, wtimeout_msec );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_WRITE_CONCERN_NEW )
{
    mongoc_write_concern_t * write_concern = mongoc_write_concern_new();
    PHB_MONGOC phWrite_concern = hbmongoc_new_dataContainer( _hbmongoc_write_concern_t_, write_concern );
    hb_retptrGC( phWrite_concern );
}
