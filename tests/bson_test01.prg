/*
 * test: bson_test01
 *
 * creating bson_t objects
 *
 */

#include "hbclass.ch"
#include "hbmongoc.ch"

#if BSON_CHECK_VERSION( 1, 5, 0 )
    #xtranslate bson_as_json( <json> ) => bson_as_canonical_extended_json( <json> )
#endif

PROCEDURE main()
    LOCAL bson
    LOCAL error
    LOCAL o

    CLS

    /* bson from simple Harbour hash */
    bson := bson_new_from_json( { "ping" => 1 }, nil, @error )
    ? " Error:", error
    ? "Result:", iif( bson != nil, bson_as_json( bson ), nil )

    ?
    /* bson from not so simple Harbour hash */
    bson := bson_new_from_json( { "Name" => "Jane", "age" => 25, "childs" => { {"Lucy"=>4}, {"Nat"=>3}, {"Mich"=>1} }, "married" => .T. }, nil, @error )
    ? " Error:", error
    ? "Result:", iif( bson != nil, bson_as_json( bson ), nil )

    ?
    /* bson from simple Harbour array */
    bson := bson_new_from_json( { 1, "a", 2, "b", 3, "c" }, nil, @error )
    ? " Error:", error
    ? "Result:", iif( bson != nil, bson_as_json( bson ), nil )

    ?
    /* bson from simple Harbour array */
    bson := bson_new_from_json( { "John", "Juan", "Bill" }, nil, @error )
    ? " Error:", error
    ? "Result:", iif( bson != nil, bson_as_json( bson ), nil )

    o := MyClass():new()
    ?
    /* bson from Harbour instanced class ( Harbour Object ) */
    bson := bson_new_from_json( o, nil, @error ) /* ? */
    ? " Error:", error
    ? "Result:", iif( bson != nil, bson_as_json( bson ), nil )

    ?
    /* bson from valid JSON string */
    bson := bson_new_from_json( e"{\"name\":{\"first\":\"Billy\",\"last\":\"Joel\"},\"2\":\"two\",\"3\":\"three\",\"4\":\"four\"}", nil, @error ) /* valid JSON */
    ? " Error:", error
    ? "Result:", iif( bson != nil, bson_as_json( bson ), nil )

    ?
    /* bson from wrong JSON */
    bson := bson_new_from_json( "1,2,3,4", nil, @error ) /* not valid */
    ? " Error:", error
    ? "Result:", iif( bson != nil, bson_as_json( bson ), nil )

    WAIT

RETURN

CLASS MyClass
PROTECTED:
    DATA parentName
    DATA parentAge
EXPORTED:
    DATA name
    DATA age

    CONSTRUCTOR new()

ENDCLASS

METHOD new() CLASS MyClass
    ::parentName := "Father"
    ::parentAge := 70
    ::name := "Me"
    ::age := 50
RETURN self
