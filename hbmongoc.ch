//
//  hbmongoc.ch
//  hbmongoc
//
//  Created by Teo Fonrouge on 8/26/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#ifndef hbmongoc_ch
#define hbmongoc_ch

#define     MONGOC_INSERT_NONE                  0
#define     MONGOC_INSERT_CONTINUE_ON_ERROR     hb_bitShift( 1, 0 )
#define     MONGOC_INSERT_NO_VALIDATE           hb_bitShift( 1, 31)

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

#endif /* hbmongoc_ch */
