/**
* Appcelerator Titanium Mobile
* Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
* Licensed under the terms of the Apache Public License
* Please see the LICENSE included with this distribution for details.
*/

#ifndef __TI_VALUE__
#define __TI_VALUE__

#include <JavaScriptCore/JavaScript.h>
#include <cassert>
#include <iostream>
#include <vector>
#include <map>
#include <atomic>

#include "TiRuntime.h"

/**
 * TiValue
 *
 * Design constraints:
 *
 * Objects of this class can only live on the stack. Never the heap. This is enforced in the class' design.
 * This class should never be subclassed.
 *
 *
 * Description: 
 *		Wrapper class for creating JSValues and and calling methods on them.
 *		Instances of this class must be created on the stack.
 *		Some examples:
 *			To create a Undefined JS value simply:
 *				Ti::Value undefined();
 *			To create a Number JS value, pass the number to the constructor
 *				Ti::Value number(12.23);
 *		You can also create JS objects or array by passing a std::map or std::vector
 *			To create an Object value with a map:
 *				std::map<std::string, Ti::Value> myObject;
 *				myObject["number"] = Ti::Value(12.34);
 *				myObject["string"] = Ti::Value("hello world);
 *				Ti::Value jsObject(myObject);
 *			Similarly, create a Array value
 *				std::vector<Ti::Value> myArray;
 *				myArray.push_back(Ti::Value(12.34));
 *				myArray.push_back(Ti::Value("hello world");
 *				Ti::Value jsArray(myArray);
 */
namespace Ti {
	class Proxy;
	class Value final
	{
	public:

        // Create an empty object.
		Value() {
            std::clog << "TiValue: ctor called ()" << std::endl;
            postConstruct(JSObjectMake(globalContext_, nullptr, nullptr));
        }
        
		~Value() {
            std::clog << "TiValue: dtor called" << std::endl;
            JSValueUnprotect(globalContext_, value_);
            ++dtorCounter_;
        }
		
        // Copy constructor.
        Value(const Value& rhs) {
            std::clog << "TiValue: copy ctor called" << std::endl;
            postConstruct(rhs.value_);
        }
        
        // Create a copy of another Ti::Value by assignment.
        Value& operator=( const Value& rhs ) {
            std::clog << "TiValue: operator= called" << std::endl;

            if ( this == &rhs ) {
                return *this;
            }

            // Tell the JavaScriptCore garbage collector that we
            // have no more interest in the value being replaced.
            JSValueUnprotect(globalContext_, value_);
            
            value_ = rhs.value_;
            
            // However, we must tell the JavaScriptCore garbage collector
            // that we do have an interest in the value that replaced the
            // previous one.
            JSValueProtect(globalContext_, value_);
            
            
            return *this;
        }
		
		// Creates a Ti::Value with a predefined JSValueRef
		explicit Value(JSValueRef value) {
            std::clog << "TiValue: ctor called (JSValueRef)" << std::endl;
            postConstruct(value);
        }
		
		// Creates a Ti::Value from a c string, the internal value will be a String
		explicit Value(const char* string) {
            std::clog << "TiValue: ctor called (const char*)" << std::endl;
            auto jsString = JSStringCreateWithUTF8CString(string);
            postConstruct(JSValueMakeString(globalContext_, jsString));
            JSStringRelease(jsString);
        }
		
		explicit Value(const std::string& string) {
			std::clog << "TiValue: ctor called (std::string)" << std::endl;
			auto jsString = JSStringCreateWithUTF8CString(string.c_str());
			postConstruct(JSValueMakeString(globalContext_, jsString));
			JSStringRelease(jsString);
		}
		// Creates a Ti::Value from a std::vector, the internal value will be an Array
		explicit Value(const std::vector<Value>& list);
		
		// Creates a Ti::Value from a std::map, this internal value will be an Object
		explicit Value(const std::map<std::string, Value>& map);
		
		// Creates a Ti::Value from a number, it must be a double. The interanl value will be a Number
		explicit Value(double number) {
            std::clog << "TiValue: ctor called (double)" << std::endl;
            postConstruct(JSValueMakeNumber(globalContext_, number));
        }
		
		// Creates a Ti::Value from a bool, the interanal value will be a Boolean
		explicit Value(bool flag) {
            std::clog << "TiValue: ctor called (bool)" << std::endl;
            postConstruct(JSValueMakeBoolean(globalContext_, flag));
        }
		
		// Creates a Ti::Value from callback function arguments
		// If the length is two or more, the internal value will be an Array
		explicit Value(const JSValueRef arguments[], size_t argumentCount) {
			if(argumentCount == 0) {
				// If there are no arguments, set the value_ to Undefined.
				postConstruct(JSValueMakeUndefined(globalContext_));
			} else if(argumentCount == 1) {
				// If there is only ono argument, set the value_ to that.
				postConstruct(arguments[0]);
			} else {
				// If there are more than one argument, create a JSArray and set the value to it
				postConstruct(JSObjectMakeArray(globalContext_, argumentCount, arguments, nullptr));
			}
		}

		// Create a Ti::Value of each of these types
		static Value Undefined() {
            return Value(JSValueMakeUndefined(Ti::Runtime::instance().globalContext()));
        }

		static Value Null() {
            return Value(JSValueMakeNull(Ti::Runtime::instance().globalContext()));
        }
		
		// ======= Utility methods for objects =======
		// Gets the property `propertyName` from the internal JSObjectRef. A Ti::Value containing
		// this property will be returned. If the internal value is not an object, the Ti::Value's value
		// will be Undefined
		Value getProperty(const std::string& propertyName) const {
            assert(isObject());
            auto propertyNameStr = JSStringCreateWithUTF8CString(propertyName.c_str());
            auto value = JSObjectGetProperty(globalContext_, (::JSObjectRef)value_, propertyNameStr, nullptr);
            JSStringRelease(propertyNameStr);
            return Value(value);
        }
		
		// Sets a property on the internal JSObjectRef. It will fail if the internal value is not
		// a JSObjectRef
		void setProperty(const std::string& propertyName, const Value& value) {
            assert(isObject());
            auto propertyNameStr = JSStringCreateWithUTF8CString(propertyName.c_str());
            JSObjectSetProperty(globalContext_, (::JSObjectRef)value_, propertyNameStr, value.JSValueRef(), kJSPropertyAttributeNone, nullptr);
            JSStringRelease(propertyNameStr);
        }

		// Returns a std::map with all the Object properties
		const std::map<std::string, Value> toDictionary() const;
		
		// ======= Utility methods for arrays =======
		// Returns true if the internal value is of type Array
		bool isArray() const {
			if (!isObject()) return false;
            // Check if this is an object (from the `getProperty` call) and get the "length" value
            // If the internal value is undefined, chances are that this is not an array
            return !getProperty("length").isUndefined();
        }
		
		// Returns the length of the array in double format for consistency
		double getLength() const {
            // Same as above, but this time return the length number
            return getProperty("length").toDouble();
        }
		
		// Returns a Ti::Value at index `index` if the internal value is an array,
		// otherwise it will fail
		Value getValueAtIndex(int index) const {
            assert(isObject());
            return Value(JSObjectGetPropertyAtIndex(globalContext_, (::JSObjectRef)value_, index, nullptr));
        }

		// Returns an std::vector with all the values of the array
		std::vector<Value> toVector() const;
		
		//  ======= Casting, converting JSValueRef to C/C++ types =======
		// Returns a number representation of the internal value
		double toDouble() const {
            ::JSValueRef exception = 0;
            const double result = JSValueToNumber(globalContext_, value_, &exception);
            if (exception) {
                //[_context notifyException:exception];
                return std::numeric_limits<double>::quiet_NaN();
            }
            
            return result;
        }
		
		// Returns a bool representation of the internal value
		bool toBool() const {
			return JSValueToBoolean(globalContext_, value_);
		}

		// Returns a bool representation of the internal value
		Ti::Proxy* toProxy();

		// Returns an std::string representation of the internal value
		std::string toString() const;
		
		// Checking types
		// Returns true if the internal value is Undefined, otherwise it returns false
		bool isUndefined() const {
            return JSValueIsUndefined(globalContext_, value_);
        }
		
		// Returns true if the internal value is Null, otherwise it returns false
		bool isNull() const {
            return JSValueIsNull(globalContext_, value_);
        }
		
		// Returns true if the internal value is a Boolean, otherwise it returns false
		bool isBoolean() const {
            return JSValueIsBoolean(globalContext_, value_);
        }
		
		// Returns true if the internal value is a Number, otherwise it returns false
		bool isNumber() const {
            return JSValueIsNumber(globalContext_, value_);
        }
		
		// Returns true if the internal value is a String, otherwise it returns false
		bool isString() const {
            return JSValueIsString(globalContext_, value_);
        }
		
		// Returns true if the internal value is an Object, otherwise it returns false
		bool isObject() const {
			return JSValueIsObject(globalContext_, value_);
		}

		// Returns the interal value
		::JSValueRef JSValueRef() const {
            return value_;
        }
		
		// Returns the interal value as a JSObjectRef that's what it is, otherwise the behaviour
		// is undefined
		::JSObjectRef JSObjectRef() const {
            // Cast the value to a JSObjectRef
            // TODO: Handle exception
            return JSValueToObject(globalContext_, value_, nullptr);
        }
        
        static long ctorCounter() {
            return ctorCounter_;
        }
		
        static long dtorCounter() {
            return dtorCounter_;
        }

    protected:
        // Prevent heap based objects.
        static void * operator new(size_t);      // #1: To prevent allocation of scalar objects
        static void * operator new [] (size_t);  // #2: To prevent allocation of array of objects
        
	private:
        void postConstruct(::JSValueRef value) {
            value_ = value;
            JSValueProtect(globalContext_, value_);
            ++ctorCounter_;
        }
        
        ::JSGlobalContextRef globalContext_ {  Ti::Runtime::instance().globalContext() };
		::JSValueRef value_ { nullptr };
        
        static std::atomic<long> ctorCounter_;
        static std::atomic<long> dtorCounter_;
	};
	
} /* namespace Ti */

#endif /* defined(__JSCoreAPII__JSValue__) */
