/*
 * test: bson_test01
 *
 * creating bson_t objects
 *
 */

#include "hbclass.ch"
#include "hbmongoc.ch"

PROCEDURE main()
    LOCAL bson
    LOCAL error
    LOCAL o

    CLS

    bson := bson_new_from_json( { "ping" => 1 }, nil, @error )
    ? " Error:", error
    ? "Result:", iif( bson != nil, bson_as_json( bson ), nil )

    bson := bson_new_from_json( { "Name" => "Jane", "age" => 25, "childs" => { "Lucy", "Nat", "Mich" } }, nil, @error )
    ? " Error:", error
    ? "Result:", iif( bson != nil, bson_as_json( bson ), nil )

    ?
    bson := bson_new_from_json( { 1, "a", 2, "b", 3, "c" }, nil, @error )
    ? " Error:", error
    ? "Result:", iif( bson != nil, bson_as_json( bson ), nil )

    ?
    bson := bson_new_from_json( { "John", "Juan", "Bill" }, nil, @error )
    ? " Error:", error
    ? "Result:", iif( bson != nil, bson_as_json( bson ), nil )

    ?
    bson := bson_new_from_json( "1,2,3,4", nil, @error ) /* not valid */
    ? " Error:", error
    ? "Result:", iif( bson != nil, bson_as_json( bson ), nil )

    o := MyClass():new()
    ?
    bson := bson_new_from_json( o, nil, @error ) /* not valid */
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
