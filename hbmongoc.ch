//
//  hbmongoc.ch
//  hbmongoc
//
//  Created by Teo Fonrouge on 8/26/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#ifndef hbmongoc_ch
#define hbmongoc_ch

#define MONGOC_INSERT_NONE                  0
#define MONGOC_INSERT_CONTINUE_ON_ERROR     hb_bitShift( 1, 0 )
#define MONGOC_INSERT_NO_VALIDATE           hb_bitShift( 1, 31)

#define MONGOC_REMOVE_NONE                  0
#define MONGOC_REMOVE_SINGLE_REMOVE         hb_bitShift( 1, 0 )

/* mongoc_read_mode_t */
#define MONGOC_READ_PRIMARY                 hb_bitShift( 1, 0 )
#define MONGOC_READ_SECONDARY               hb_bitShift( 1, 1 )
#define MONGOC_READ_PRIMARY_PREFERRED       hb_bitShift( 1, 2 ) .OR. MONGOC_READ_PRIMARY
#define MONGOC_READ_SECONDARY_PREFERRED     hb_bitShift( 1, 2 ) .OR. MONGOC_READ_SECONDARY
#define MONGOC_READ_NEAREST                 hb_bitShift( 1, 3 ) .OR. MONGOC_READ_SECONDARY

/* bson macros */
#xtranslate BSON_APPEND_ARRAY( <document>, <key>, <array> ) => bson_append_array( <document>, <key>, -1, <array> )
#xtranslate BSON_APPEND_ARRAY_BEGIN( <document>, <key>, <child> ) => bson_append_array_begin( <document>, <key>, -1, <child> )
#xtranslate BSON_APPEND_BINARY( <document>, <key>, <binData> ) => bson_append_binary ( <document>, <key>, -1, nil, <binData>, nil )
#xtranslate BSON_APPEND_BOOL( <document>, <key>, <value> ) => bson_append_bool( <document>, <key>, -1, <value> )
#xtranslate BSON_APPEND_CODE( <document>, <key>, <code> ) => bson_append_code( <document>, <key>, -1, <code> )
#xtranslate BSON_APPEND_CODE_WITH_SCOPE( <document>, <key>, <code>, <scope> ) => bson_append_code_with_scope( <document>, <key>, -1, <code>, <scope> )
#xtranslate BSON_APPEND_DATE_TIME( <document>, <key>, <dt> ) => bson_append_date_time( <document>, <key>, -1, <dt> )
#xtranslate BSON_APPEND_DECIMAL128( <document>, <key>, <dec> ) => bson_append_decimal128( <document>, <key>, -1, <dec> )
#xtranslate BSON_APPEND_DOCUMENT_BEGIN( <document>, <key>, <child> ) => bson_append_document_begin( <document>, <key>, -1, <child> )
#xtranslate BSON_APPEND_DOCUMENT( <document>, <key>, <child> ) => bson_append_document( <document>, <key>, -1, <child> )
#xtranslate BSON_APPEND_DOUBLE( <document>, <key>, <value> ) => bson_append_double( <document>, <key>, -1, <value> )
#xtranslate BSON_APPEND_INT32( <document>, <key>, <value> ) => bson_append_int32( <document>, <key>, -1, <value> )
#xtranslate BSON_APPEND_INT64( <document>, <key>, <value> ) => bson_append_int64( <document>, <key>, -1, <value> )
#xtranslate BSON_APPEND_REGEX( <document>, <key>, <regex>, <options> ) => bson_append_regex( <document>, <key>, -1, <regex>, <options> )
#xtranslate BSON_APPEND_UTF8( <document>, <key>, <value> ) => bson_append_utf8( <document>, <key>, -1, <value>, -1 )

/* bson_error_t */
#xtranslate HB_BSON_ERROR_DOMAIN( <error> ) => iif( <error> = nil, nil, <error>\["domain"\] )
#xtranslate HB_BSON_ERROR_CODE( <error> ) => iif( <error> = nil, nil, <error>\["code"\] )
#xtranslate HB_BSON_ERROR_MESSAGE( <error> ) => iif( <error> = nil, nil, <error>\["message"\] )

/* mongoc_query_flags_t ( DEPRECATED ) */
#define MONGOC_QUERY_NONE               0
#define MONGOC_QUERY_TAILABLE_CURSOR    hb_bitShift( 1, 1 )
#define MONGOC_QUERY_SLAVE_OK           hb_bitShift( 1, 2 )
#define MONGOC_QUERY_OPLOG_REPLAY       hb_bitShift( 1, 3 )
#define MONGOC_QUERY_NO_CURSOR_TIMEOUT  hb_bitShift( 1, 4 )
#define MONGOC_QUERY_AWAIT_DATA         hb_bitShift( 1, 5 )
#define MONGOC_QUERY_EXHAUST            hb_bitShift( 1, 6 )
#define MONGOC_QUERY_PARTIAL            hb_bitShift( 1, 7 )

/* mongoc_write_concern_t */
#define MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED   0
#define MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED   -1 /* deprecated */
#define MONGOC_WRITE_CONCERN_W_DEFAULT          -2
#define MONGOC_WRITE_CONCERN_W_MAJORITY         -3
#define MONGOC_WRITE_CONCERN_W_TAG              -4

/* mongoc_update_flags_t */
#define MONGOC_UPDATE_NONE          0
#define MONGOC_UPDATE_UPSERT        hb_bitShift( 1, 0 )
#define MONGOC_UPDATE_MULTI_UPDATE  hb_bitShift( 1, 1 )
#define MONGOC_UPDATE_NO_VALIDATE   hb_bitShift( 1, 31 )

/* bson_validate_flags_t */
#define BSON_VALIDATE_NONE              0
#define BSON_VALIDATE_UTF8              hb_bitShift( 1, 0 )
#define BSON_VALIDATE_DOLLAR_KEYS       hb_bitShift( 1, 1 )
#define BSON_VALIDATE_DOT_KEYS          hb_bitShift( 1, 2 )
#define BSON_VALIDATE_UTF8_ALLOW_NULL   hb_bitShift( 1, 3 )
#define BSON_VALIDATE_EMPTY_KEYS        hb_bitShift( 1, 4 )

/* bson_context_t */
#define BSON_CONTEXT_NONE               0
#define BSON_CONTEXT_THREAD_SAFE        hb_bitShift( 1, 0 )
#define BSON_CONTEXT_DISABLE_HOST_CACHE hb_bitShift( 1, 1 )
#define BSON_CONTEXT_DISABLE_PID_CACHE  hb_bitShift( 1, 2 )
#define BSON_CONTEXT_USE_TASK_ID        hb_bitShift( 1, 3 )

/* bson_type_t */
#define BSON_TYPE_EOD           0x00
#define BSON_TYPE_DOUBLE        0x01
#define BSON_TYPE_UTF8          0x02
#define BSON_TYPE_DOCUMENT      0x03
#define BSON_TYPE_ARRAY         0x04
#define BSON_TYPE_BINARY        0x05
#define BSON_TYPE_UNDEFINED     0x06
#define BSON_TYPE_OID           0x07
#define BSON_TYPE_BOOL          0x08
#define BSON_TYPE_DATE_TIME     0x09
#define BSON_TYPE_NULL          0x0A
#define BSON_TYPE_REGEX         0x0B
#define BSON_TYPE_DBPOINTER     0x0C
#define BSON_TYPE_CODE          0x0D
#define BSON_TYPE_SYMBOL        0x0E
#define BSON_TYPE_CODEWSCOPE    0x0F
#define BSON_TYPE_INT32         0x10
#define BSON_TYPE_TIMESTAMP     0x11
#define BSON_TYPE_INT64         0x12
#define BSON_TYPE_DECIMAL128    0x13
#define BSON_TYPE_MAXKEY        0x7F
#define BSON_TYPE_MINKEY        0xFF

/* bson_subtype_t */
#define BSON_SUBTYPE_BINARY             0x00
#define BSON_SUBTYPE_FUNCTION           0x01
#define BSON_SUBTYPE_BINARY_DEPRECATED  0x02
#define BSON_SUBTYPE_UUID_DEPRECATED    0x03
#define BSON_SUBTYPE_UUID               0x04
#define BSON_SUBTYPE_MD5                0x05
#define BSON_SUBTYPE_USER               0x80

#endif /* hbmongoc_ch */
