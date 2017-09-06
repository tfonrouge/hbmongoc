//
//  hb_mongo_cursor.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/5/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongo_cursor.h"
#include "hb_mongoc.h"

HB_FUNC( MONGOC_CURSOR_NEXT )
{
    mongoc_cursor_t * cursor = mongoc_hbparam( 1, _hb_cursor_t_ );


    if ( cursor && HB_ISBYREF( 2 ) ) {

        const bson_t * doc;

        bool result = mongoc_cursor_next( cursor, &doc );

        /* TODO: solve cast warning  */
        /* TODO: solve return json/hash */
        //hbmongoc_return_byref_bson( 2, ( void * ) doc, false );

        if ( result ) {
            bson_t * bson = bson_copy( doc );
            hbmongoc_return_byref_bson( 2, bson );
        } else {
            hb_stor( 2 );
        }

        hb_retl( result );
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
