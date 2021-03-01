#include "jsonquery.h"

namespace jsonQuery {

/* This namespace contains methods that are "private" implementation code and
 * are accessible to this file only.
 */
namespace {

bool _isRequestedObject(QJsonValue child, QString key, QString value)
{
    bool is_object;
    bool is_key_value_match;
    QJsonObject child_object;

    child_object = child.toObject();
    // 1. are we correctly identifying child as an object or not?
    is_object = !child_object.isEmpty();
    if (!is_object) { return false; }

    // 2. does chiled have member key?
    if (!child_object.contains(key)) { return false; }

    // 3. does key value match?
    is_key_value_match =
            (child_object[key].toString().toLower() == value.toLower());
    if (!is_key_value_match) { return false; }
    return true;
}

}  // namespace


QJsonObject getJsonObjectByKey(QJsonObject root_object, QString key)
{
    QJsonValue json_at_key;

    if (!root_object.contains(key)) {
        QJsonObject emptyObj;
        return emptyObj;
    }
    else {
        json_at_key = root_object[key];
    }

    return json_at_key.toObject();
}

QJsonArray getJsonArrayByKey(QJsonObject root_object, QString key)
{
    QJsonValue json_at_key;

    if (!root_object.contains(key)) {
        QJsonArray emptyList;
        json_at_key = emptyList;
    }
    else {
        json_at_key = root_object[key];
    }

    return json_at_key.toArray();
}

QJsonValue getJsonValueByKey(QJsonObject root_object, QString key)
{
    if (root_object.contains(key)) { return root_object[key]; }
    else { return ""; }
}

QJsonObject getJsonObjectByMemberValue(QJsonObject root_object, QString key,
                                       QString value)
{
    foreach (QJsonValue child, root_object) {
        QJsonObject child_object = child.toObject();
        if (!child.isObject()) { continue; }
        bool is_has_member = child_object.contains(key);
        if (!is_has_member) { continue; }
        bool is_key_value_match = (child_object[key].toString() == value);
        if (!is_key_value_match) { continue; }
        return child_object;
    }

    QJsonObject nullObj;
    return nullObj;
}

QJsonObject getJsonObjectByMemberValue(QJsonArray root_list, QString key,
                                       QString value)
{
    foreach (QJsonValue child, root_list) {
        if (!_isRequestedObject(child, key, value)) { continue; }
        return child.toObject();
    }

    QJsonObject nullObj;
    return nullObj;
}

QList<QJsonObject> getJsonObjListByMemberValue(QJsonArray root, QString key,
                                               QString value)
{
    QList<QJsonObject> objList;
    foreach (QJsonValue child, root) {
        if (!_isRequestedObject(child, key, value)) { continue; }
        objList.append(child.toObject());
    }
    return objList;
}


QJsonValue getObjectAttributeValue(QJsonObject root_object, QString att_key)
{
    QString attribute_key = "attributes";
    QJsonValueRef wav_attributes = root_object[attribute_key];
    QJsonObject att_obj = wav_attributes.toObject();
    return att_obj[att_key];
}

bool isJsonObjectHasMember(QJsonObject obj, QString key)
{
    return !obj.value(key).isUndefined();
}

bool isJsonValueAnObject(QJsonValue object)
{
    return (object.isObject() && !object.isNull());
}

QString jsonObjectToString(QJsonObject obj) {
    QJsonDocument doc(obj);
    return QString(doc.toJson(QJsonDocument::Indented));
}

QString jsonArrayToString(QJsonArray list) {
    QJsonDocument doc(list);
    return QString(doc.toJson(QJsonDocument::Indented));
}

QJsonDocument loadJsonDoc(QByteArray source)
{
    QJsonParseError error;

    auto jdoc = QJsonDocument::fromJson(source, &error);
    if (!jdoc.isNull()) { return jdoc; }
    QJsonDocument nullDoc;
    return nullDoc;
}

QJsonObject getJsonObjectFromListIndex(QJsonArray list, int index)
{
    int size = list.size();
    bool isValidIndex = (size > index) || (index < 0);
    QJsonObject nullObj;
    if (!isValidIndex) { return nullObj; }
    QJsonValue record = list.at(index);
    if (!record.isObject()) { return nullObj; }
    return record.toObject();
}

QJsonArray mergeJsonArrays(QJsonArray list1, QJsonArray list2)
{
    for (int i = 0; i < list2.size(); ++i) { list1.append(list2[i]); }
    return list1;
}

}  // namespace jsonQuery
