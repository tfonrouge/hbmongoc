//
//  hbmongoc.ch
//  hbmongoc
//
//  Automatically Created by Teo Fonrouge on 8/26/17.
//  Copyright © 2017 Teo Fonrouge. All rights reserved.
//

#ifndef hbbson_ch
#define hbbson_ch

#define BSON_MAJOR_VERSION (1)
#define BSON_MINOR_VERSION (7)
#define BSON_MICRO_VERSION (0)

#define BSON_CHECK_VERSION(major,minor,micro)   (BSON_MAJOR_VERSION > (major) .OR. (BSON_MAJOR_VERSION == (major) .AND. BSON_MINOR_VERSION > (minor)) .OR. (BSON_MAJOR_VERSION == (major) .AND. BSON_MINOR_VERSION == (minor) .AND. BSON_MICRO_VERSION >= (micro)))

#endif /* hbbson_ch */

