/**
 * jsonquery.cpp
 * Erick Veil
 * 2016-08-15
 * Copyright 2016 ComTech Communications
 *
 * Changelog:
 *
 * 2021-03-01
 * EV
 * Removed any logging output.
 * Removed validation methods and replaced with return default values.
 * Removed stnctl namespace.
 **/

#ifndef JSONQUERY_H
#define JSONQUERY_H

#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonValueRef>
#include <QVariant>

#include <QDebug>
#include <QJsonDocument>

namespace stnctl {

/**
 * JsonQuery safely loads, saves, and edits jquery data, throwing
 * exceptions when there are problems.
 *
 * I am establishing these as static functions, though it violates the concept
 * of avoiding global components in loosely coupled design. The qureies do not
 * affect any class values. The query collection is specific in funtion and
 * not likely to be replaced. Test versions of the queries do not make sense.
 * It would contribute to parameter bloat to inclue a query object in the
 * constructor of every single json parsing class. And finally, as of this
 * writing, I have found no reasonable argumen online as to why I would not do
 * this.
 *
 * The structure of this file follows a pattern outlined in this SO post:
 * http://stackoverflow.com/a/112451
 * This pattern, and the provided reasons for using it make the most sense to
 * me.*
 */
namespace jsonQuery {

/**
 * @brief loadJsonDoc
 *
 * Safely loads a document from byte data and validates that it's not null.
 *
 * @param source
 * @return
 */
QJsonDocument loadJsonDoc(QByteArray source);

/**
 * @brief validateDocIsObject
 *
 * @param doc
 * @throws if the root of the document is not a JSON object.
 */
void validateDocIsObject(QJsonDocument doc);

void validateJValueIsObject(QJsonValue val);

/**
 * @brief getJsonObjectByKey
 *
 * Provide a string key, and get back a QJsonObject with built in validation.
 * Throws if the key does not point to an object, or does not exist.
 *
 * @param root_object
 * @param key
 * @return
 */
QJsonObject getJsonObjectByKey(QJsonObject root_object, QString key);

/**
 * @brief getJsonObjectFromListIndex
 *
 * Given a QJsonArray of objects, returns the object found at index.
 * Validates that the QJsonValue at the index can be cast to object and then
 * does so.
 *
 * @param list
 * @param index
 * @return The object at index
 * @throws If item at index cannot be cast to object, or if index is not on the
 * list.
 */
QJsonObject getJsonObjectFromListIndex(QJsonArray list, int index);

/**
 * @brief getJsonArrayByKey
 *
 * Provide a string key, and get back a QJsonArray, with built in validation.
 * Throws if the key does not exist in root_object, or if it does not hold
 * an array.
 *
 * @param root_object
 * @param key
 * @return
 */
QJsonArray getJsonArrayByKey(QJsonObject root_object, QString key);

/**
 * @brief getJsonValueByKey
 *
 * Provide a string key, and get back a QJsonValue, with validation.
 * Superior to calling root_object[key], as it automatically provides
 * validation in that it throws if the key does not exist.
 *
 * @param root_object
 * @param key
 * @return
 */
QJsonValue getJsonValueByKey(QJsonObject root_object, QString key);

/**
 * @brief getJsonObjectByMemberValue
 *
 * Iterates over the members of root_object and returns a QJsonObject only if:
 * 1. A member contains an object,
 * 2. AND that object contains the key provided,
 * 3. AND the value stored at that key is a string with the provided value.
 *
 * Example:
 * {
 *      "item_a" : {
 *              "id" : "001",
 *              "item_c" : "value_b"
 *      },
 *      "item_b" : {
 *              "id" : "002",
 *              "item_c" : "value_c"
 *      }
 * }
 *
 * getJsonObjectByMemberValue(root, "id", "001");
 * will return the "item_a" object.
 *
 * Will throw if no criteria are found as a match for that key/value pair.
 *
 * @param root_object : A QJsonObject containing several object members (may
 * also contain non object members).
 * @param key : The key that must be in at leas one of the objects contained
 * within the root_object.
 * @param value : The value that must be set on at least one of the objects at
 * their key member.
 * @return The object that matches all of the criteria. If there are multiple
 * objects, the first will be returned. If there are no objects, an exception
 * will be thrown.
 */
QJsonObject getJsonObjectByMemberValue(QJsonObject root_object, QString key,
                                       QString value);

/**
 * @brief getJsonObjectByMemberValue
 *
 * Overloaded method that searches an array for a matching key/value pair,
 * instead of an object.
 *
 * @param rootList
 * @param key
 * @param value
 * @return
 */
QList<QJsonObject> getJsonObjListByMemberValue(QJsonArray root, QString key,
                                               QString value);
/**
 * @brief getJsonObjectByMemberValue
 *
 * Iterates over the members of the root_list and returns a QJsonObject only if:
 * 1. The list item contains an object.
 * 2. AND that object contains the key proviced.
 * 3. AND the value stored at that key is a string with the provided value.
 *
 * Overloaded method.
 *
 * @param root_list : A QJsonArray containing object members.
 * @param key : The key that must be in at leas one of the objects contained
 * within the root_object.
 * @param value : The value that must be set on at least one of the objects at
 * their key member.
 * @return The object that matches all of the criteria. If there are multiple
 * objects, the first will be returned. If there are no objects, an exception
 * will be thrown.
 */
QJsonObject getJsonObjectByMemberValue(QJsonArray root_list, QString key,
                                       QString value);


/**
 * @brief getObjectAttributeValue
 *
 * Returns the "attribute value" of an object.
 * The argument for root_object must be a QJsonObject, and it must contain a
 * member with the key named "attributes".
 * The attributes member must be an object that contains one or more members
 * of any type.
 * At least one of the attributes object's members must be named att_key.
 *
 * Will throw if any of these criteria are not met.
 *
 * An example of the very specific structure required by this method:
 * {
 *      "item_a" : {
 *              "id" : "001",
 *              "name" : "Foo",
 *              "attributes" : {
 *                      "src" : "http://example.com",
 *                      "style" : "http://example.com/style.css"
 *              }
 *      }
 * }
 *
 * In this example, root_object must be the "item_a" object, and the value
 * passed for att_key must be either "src" or "style".
 * If those criteria are not met, a qxException will be thrown.
 *
 * @param root_object : A JSON object that contains an "attributes" object
 * member.
 * @param att_key : One of the members of the attributes object.
 * @return The value stored at the att_key location.
 */
QJsonValue getObjectAttributeValue(QJsonObject root_object,
                                   QString att_key);

QJsonArray mergeJsonArrays(QJsonArray list1, QJsonArray list2);
/**
 * @brief isJsonObjectHasMember
 *
 * Test to see if the provided obj has a member with the key name provided.
 *
 * @param obj
 * @param key
 * @return : False if the key is not a member of obj. True if it is.
 */
bool isJsonObjectHasMember(QJsonObject obj, QString key);

/**
 * @brief isJsonValueAnObject
 *
 * Test to see if the QValue is a Json object.
 *
 * @param object
 * @return
 */
bool isJsonValueAnObject(QJsonValue object);

/**
 * @brief validateIsKeyMemberOfJsonObject
 *
 * Throws qxException if obj does not contain the provided key.
 *
 * @param obj
 * @param key
 */
void validateIsKeyMemberOfJsonObject(QJsonObject obj, QString key);

/**
 * @brief validateJsonValueIsJsonObject
 *
 * Throws qxException if object is not a QJsonObject.
 *
 * @param object
 */
void validateJsonValueIsJsonObject(QJsonValue object, QString key);

/**
 * @brief validateIsJsonList
 *
 * Throws qxException if list is not a QJsonList
 *
 * @param list
 * @param key
 */
void validateIsJsonList(QJsonValue json_list, QString key);

/**
 * @brief validateJsonObject
 *
 * Throws qxException if obj is not actually a valid object.
 *
 * @param obj
 */
void validateJsonObject(QJsonObject obj);

/**
 * @brief validateFilePath
 *
 * Throws qxException if the provided path does not lead to an actual file.
 * Not exactly a JSON related method, but I have no other class for this sort
 * of method at this time.
 *
 * @param path
 */
void validateFilePath(QString path);

/**
 * @brief jsonObjectToString
 *
 * @param obj
 * @return obj as a string of indented json
 */
QString jsonObjectToString(QJsonObject obj);

/**
 * @brief jsonArrayToString
 *
 * @param list
 * @return list as a string of indented json
 */
QString jsonArrayToString(QJsonArray list);

}  // namespace jsonQuery
}  // namespace stnctl

#endif  // JSONQUERY_H
