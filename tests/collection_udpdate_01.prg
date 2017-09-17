/*
 *
 */

PROCEDURE main()
    LOCAL client
    LOCAL collection
    LOCAL oid

    /* REQUIRED to initialize mongoc internals */
    mongoc_init()

    client := mongoc_client_new( "mongodb://localhost:27027//?appname=update-example" )
    collection := mongoc_client_get_collection( client, "mydb", "mycoll" )

    bson_oid_init( @oid, nil )

    /* REQUIRED to cleanup mongoc internals */
    mongoc_cleanup()

    WAIT

RETURN
