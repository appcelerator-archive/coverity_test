/**
* Appcelerator Titanium Mobile
* Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
* Licensed under the terms of the Apache Public License
* Please see the LICENSE included with this distribution for details.
*/

#include "TiValue.h"
//#include "TiProxy.h"

// For description on each function, see header
namespace Ti
{
    std::atomic<long> Value::ctorCounter_ { 0 };
    std::atomic<long> Value::dtorCounter_ { 0 };
    
	Value::Value(const std::vector<Value>& list) {
        std::clog << "TiValue: ctor called (const std::vector<Value>&)" << std::endl;
		const unsigned long length = list.size();
		// Create the Array values by iterating through the vector
		std::vector<::JSValueRef> array;
		for (unsigned long i = 0; i < length; i++) {
			// Insert each JSValue from the list into the JSArray values array
			array.push_back(list[i].JSValueRef());
		}
		// Create a JSArray and set the internal value to it
		postConstruct(JSObjectMakeArray(globalContext_, length, &array[0], nullptr));
	}
	
	Value::Value(const std::map<std::string, Value>& map) {
        std::clog << "TiValue: ctor called (const std::map<std::string, Value>&)" << std::endl;
		// Create an empty object
		auto object = JSObjectMake(globalContext_, nullptr, nullptr);
		// Iterate through the std::map to grab the keys and values
		for(const auto& key : map)
		{
			// Insert the keys and value in the JSObject
			const auto keyStr = JSStringCreateWithUTF8CString(key.first.c_str());
			JSObjectSetProperty(globalContext_, object, keyStr, key.second.JSValueRef(), kJSPropertyAttributeNone, nullptr);
			JSStringRelease(keyStr);
		}
		// Set the internal value to this new obeject
		postConstruct(object);
	}
	
	const std::map<std::string, Value> Value::toDictionary() const {
        assert(isObject());
		std::map<std::string, Value> props;
		// Get an array of all the properties of this object
		JSPropertyNameArrayRef propertyArray = JSObjectCopyPropertyNames(globalContext_, (::JSObjectRef)value_);
		// Get the length
		const size_t count = JSPropertyNameArrayGetCount(propertyArray);
		// Iterate through all the properties
		for (size_t i = 0; i < count; i++) {
			// Get the key of this property
			::JSStringRef key = JSPropertyNameArrayGetNameAtIndex(propertyArray, i);
			// Get the value
			::JSValueRef value = JSObjectGetProperty(globalContext_, (::JSObjectRef)value_, key, nullptr);
			// Get the std::string representation
			std::string stringKey = Value(JSValueMakeString(globalContext_, key)).toString();
			// Add it to the std::map
			props[stringKey] = Value(value);
		}
		// Release the array
		JSPropertyNameArrayRelease(propertyArray);
		// return the map
		return props;
	}

	std::vector<Value> Value::toVector() const {
		std::vector<Value> allValues;
        assert(isArray());
		// Iterate as many times as "length" on the array
		for(int i = 0, length = (int)getLength(); i < length; i++) {
			// Add a new Value to the vector
			allValues.push_back(Value(JSObjectGetPropertyAtIndex(globalContext_, (::JSObjectRef)value_, i, nullptr)));
		}
		// return all values
		return allValues;
	}
	
	std::string Value::toString() const {
		auto string = JSValueToStringCopy(globalContext_, value_, nullptr);
		auto size = JSStringGetMaximumUTF8CStringSize(string);
		char* cstring = (char*) std::malloc(size);
		JSStringGetUTF8CString(string, cstring, size);
		std::string str { cstring };
		std::free(cstring);
		JSStringRelease(string);
		return str;
	}
	Ti::Proxy* Value::toProxy()
	{
		if (!isObject()) return nullptr;
		void * raw = JSObjectGetPrivate((::JSObjectRef)value_);
		return static_cast<Ti::Proxy*>(raw);
	}
}
