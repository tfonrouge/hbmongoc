//
//  hb_mongoc_collection.c
//  hbmongoc
//
//  Created by Teo Fonrouge on 9/1/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#include "hb_mongoc_collection.h"
#include "hb_mongoc.h"


HB_FUNC( MONGOC_COLLECTION_AGGREGATE )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * pipeline = bson_hbparam( 3, HB_IT_ANY );

    if (collection && pipeline) {
        mongoc_query_flags_t flags = hb_parnidef( 2, MONGOC_QUERY_NONE );
        bson_t * opts = bson_hbparam( 4, HB_IT_ANY );
        const mongoc_read_prefs_t *read_prefs = mongoc_hbparam( 5, _hbmongoc_read_prefs_t_ );

        mongoc_cursor_t * cursor = mongoc_collection_aggregate(collection, flags, pipeline, opts, read_prefs);

        PHB_MONGOC phCursor = hbmongoc_new_dataContainer( _hbmongoc_cursor_t_, cursor );

        hb_retptrGC( phCursor );

        if (opts && !HB_ISPOINTER(4)) {
            bson_destroy(opts);
        }

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if (pipeline && !HB_ISPOINTER(3)) {
        bson_destroy(pipeline);
    }
}

HB_FUNC( MONGOC_COLLECTION_CREATE_BULK_OPERATION_WITH_OPTS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );

    if ( collection ) {
        bson_t * opts = bson_hbparam( 2, HB_IT_ANY );

        mongoc_bulk_operation_t * bulk = mongoc_collection_create_bulk_operation_with_opts(collection, opts);
        PHB_MONGOC phMongo = hbmongoc_new_dataContainer( _hbmongoc_bulk_operation_t_, bulk );
        hb_retptrGC( phMongo );

        if ( opts && ! HB_ISPOINTER( 2 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_COMMAND_SIMPLE )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * command = bson_hbparam( 2, HB_IT_ANY );

    if ( collection && command && HB_ISBYREF( 4 ) ) {

        const mongoc_read_prefs_t *read_prefs = mongoc_hbparam( 3, _hbmongoc_read_prefs_t_ );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_collection_command_simple( collection, command, read_prefs, &reply, &error);

        hbmongoc_return_byref_bson( 4, bson_copy( &reply ) );
        bson_destroy(&reply);
        bson_hbstor_byref_error( 5, &error, result );

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( command && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( command );
    }
}

HB_FUNC( MONGOC_COLLECTION_DESTROY )
{
    PHB_MONGOC collection = hbmongoc_param( 1, _hbmongoc_collection_t_ );

    if( collection ) {
        mongoc_collection_destroy( collection->p );
        collection->p = NULL;
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_DROP )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );

    if ( collection ) {
        bson_error_t error;

        bool result = mongoc_collection_drop( collection, &error );

        bson_hbstor_byref_error( 2, &error, result );

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_DROP_WITH_OPTS )
#if MONGOC_CHECK_VERSION( 1, 5, 0 )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );

    if ( collection ) {
        bson_t * opts = bson_hbparam( 2, HB_IT_ANY );

        bson_error_t error;

        bool result = mongoc_collection_drop_with_opts( collection, opts, &error );

        bson_hbstor_byref_error( 3, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 2 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}
#else
{
    HBMONGOC_ERR_NOFUNC();
}
#endif

HB_FUNC( MONGOC_COLLECTION_FIND )
#if ! MONGOC_CHECK_VERSION( 1, 5, 0 )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * query = bson_hbparam( 6, HB_IT_ANY );

    if ( collection && query ) {
        mongoc_query_flags_t flags = hb_parnidef( 2, MONGOC_QUERY_NONE );
        u_int32_t skip = hb_parnidef( 3, 0 );
        u_int32_t limit = hb_parnidef( 4, 0 );
        u_int32_t batch_size = hb_parnidef( 5, 0 );
        bson_t * fields = bson_hbparam( 7, HB_IT_ANY );
        const mongoc_read_prefs_t *read_prefs = mongoc_hbparam( 8, _hbmongoc_read_prefs_t_ );
        mongoc_cursor_t * cursor = mongoc_collection_find( collection, flags, skip, limit, batch_size, query, fields, read_prefs );

        PHB_MONGOC phCursor = hbmongoc_new_dataContainer( _hbmongoc_cursor_t_, cursor );

        hb_retptrGC( phCursor );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( query && ! HB_ISPOINTER( 6 ) ) {
        bson_destroy( query );
    }
}
#else
{
    HBMONGOC_ERR_DEPRECATEDFUNC();
}
#endif

HB_FUNC( MONGOC_COLLECTION_FIND_INDEXES_WITH_OPTS )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );

    if (collection) {
        bson_t * opts = bson_hbparam( 2, HB_IT_ANY );

        mongoc_cursor_t * cursor = mongoc_collection_find_indexes_with_opts(collection, opts);

        PHB_MONGOC phCursor = hbmongoc_new_dataContainer( _hbmongoc_cursor_t_, cursor );

        hb_retptrGC( phCursor );

        if ( opts && ! HB_ISPOINTER( 2 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_FIND_WITH_OPTS )
#if MONGOC_CHECK_VERSION( 1, 5, 0 )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * filter = bson_hbparam( 2, HB_IT_ANY );

    if ( collection && filter ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
        const mongoc_read_prefs_t *read_prefs = mongoc_hbparam( 4, _hbmongoc_read_prefs_t_ );

        mongoc_cursor_t * cursor = mongoc_collection_find_with_opts( collection, filter, opts, read_prefs );

        PHB_MONGOC phCursor = hbmongoc_new_dataContainer( _hbmongoc_cursor_t_, cursor );

        hb_retptrGC( phCursor );

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( filter && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( filter );
    }
}
#else
{
    HBMONGOC_ERR_NOFUNC();
}
#endif

HB_FUNC( MONGOC_COLLECTION_INSERT )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * document = bson_hbparam( 3, HB_IT_ANY );

    if ( collection && document ) {
        mongoc_insert_flags_t flags = hb_parnidef( 2, MONGOC_INSERT_NONE );
        const mongoc_write_concern_t * write_concern = mongoc_hbparam( 4, _hbmongoc_write_concern_t_ );
        bson_error_t error;

        bool result = mongoc_collection_insert( collection, flags, document, write_concern, &error );

        bson_hbstor_byref_error( 5, &error, result );

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( document && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( document );
    }
}

/**
 * param 3 (array size) not used
 */
HB_FUNC( MONGOC_COLLECTION_INSERT_MANY )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    PHB_ITEM pArray = hb_param( 2, HB_IT_ARRAY );

    if (collection && pArray) {
        HB_SIZE arrayLen = hb_arrayLen(pArray);
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;
        bson_t * docs[arrayLen];
        bson_t * noBsonDocs[arrayLen];

        for (HB_SIZE i = 0; i < arrayLen; ++i) {
            PHB_ITEM pItem = hb_itemArrayGet(pArray, i + 1);
            docs[i] = get_bson_item(pItem);
            if (docs[i] && ! HB_IS_POINTER(pItem)) {
                noBsonDocs[i] = docs[i];
            } else {
                noBsonDocs[i] = NULL;
            }
            hb_itemRelease(pItem);
        }

        bool result = mongoc_collection_insert_many(collection, (const bson_t **) docs, arrayLen, opts, &reply, &error);

        for (HB_SIZE i = 0; i < arrayLen; ++i) {
            if (noBsonDocs[i] != NULL) {
                bson_free(noBsonDocs[i]);
            }
        }

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }

        if (HB_ISBYREF(4)) {
            hbmongoc_return_byref_bson(4, bson_copy(&reply));
        }
        bson_destroy(&reply);

        bson_hbstor_byref_error( 5, &error, result );

        hb_retl(result);

    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_INSERT_ONE )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * document = bson_hbparam( 2, HB_IT_ANY );

    if (collection && document) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );
        bson_t reply;
        bson_error_t error;

        bool result = mongoc_collection_insert_one(collection, document, opts, &reply, &error);

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }

        if (HB_ISBYREF(4)) {
            hbmongoc_return_byref_bson(4, bson_copy(&reply));
        }
        bson_destroy(&reply);

        bson_hbstor_byref_error( 5, &error, result );

        hb_retl(result);

    } else {
        HBMONGOC_ERR_ARGS();
    }
}

HB_FUNC( MONGOC_COLLECTION_KEYS_TO_INDEX_STRING )
{
    bson_t * document = bson_hbparam( 1, HB_IT_ANY );

    if ( document ) {
        char * indexName = mongoc_collection_keys_to_index_string( document );
        hb_retc( indexName );
        bson_free( indexName );
    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( document && ! HB_ISPOINTER( 1 ) ) {
        bson_destroy( document );
    }
}

HB_FUNC( MONGOC_COLLECTION_REMOVE )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * selector = bson_hbparam( 3, HB_IT_ANY );

    if ( collection && selector ) {
        int flags = hb_parnidef( 2, MONGOC_REMOVE_NONE );
        const mongoc_write_concern_t * write_concern = mongoc_hbparam( 4, _hbmongoc_write_concern_t_ );
        bson_error_t error;

        bool result = mongoc_collection_remove( collection, flags, selector, write_concern, &error );

        bson_hbstor_byref_error( 5, &error, result );

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( selector );
    }
}

HB_FUNC( MONGOC_COLLECTION_UPDATE )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * selector = bson_hbparam( 3, HB_IT_ANY );
    bson_t * update = bson_hbparam( 4, HB_IT_ANY );

    if ( collection && HB_ISNUM( 2 ) && selector && update ) {
        int flags = hb_parni( 2 );
        const mongoc_write_concern_t * write_concern = mongoc_hbparam( 5, _hbmongoc_write_concern_t_ );
        bson_error_t error;

        bool result = mongoc_collection_update( collection, flags, selector, update, write_concern, &error );

        bson_hbstor_byref_error( 6, &error, result );

        hb_retl( result );

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( selector && ! HB_ISPOINTER( 3 ) ) {
        bson_destroy( selector );
    }

    if ( update && ! HB_ISPOINTER( 4 ) ) {
        bson_destroy( update );
    }
}

HB_FUNC( MONGOC_COLLECTION_WRITE_COMMAND_WITH_OPTS )
#if MONGOC_CHECK_VERSION( 1, 5, 0 )
{
    mongoc_collection_t * collection = mongoc_hbparam( 1, _hbmongoc_collection_t_ );
    bson_t * command = bson_hbparam( 2, HB_IT_ANY );

    if ( collection && command ) {
        bson_t * opts = bson_hbparam( 3, HB_IT_ANY );

        bson_t reply;
        bson_error_t error;

        bool result = mongoc_collection_write_command_with_opts( collection, command, opts, &reply, &error );

        hbmongoc_return_byref_bson( 4, bson_copy( &reply ) );
        bson_destroy(&reply);
        bson_hbstor_byref_error( 5, &error, result );

        hb_retl( result );

        if ( opts && ! HB_ISPOINTER( 3 ) ) {
            bson_destroy( opts );
        }

    } else {
        HBMONGOC_ERR_ARGS();
    }

    if ( command && ! HB_ISPOINTER( 2 ) ) {
        bson_destroy( command );
    }
}
#else
{
    HBMONGOC_ERR_NOFUNC();
}
#endif
