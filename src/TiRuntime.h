/**
* Appcelerator Titanium Mobile
* Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
* Licensed under the terms of the Apache Public License
* Please see the LICENSE included with this distribution for details.
*/

#ifndef __TI_RUNTIME__
#define __TI_RUNTIME__

#include <JavaScriptCore/JavaScript.h>
#include <string>
#include <cassert>

namespace Ti {
	class Value;
	class Proxy;
	class Runtime
	{
	public:
        // Return the one and only instance of the Runtime class.
        static Runtime& instance();
        // Returns the global JS context
		::JSGlobalContextRef globalContext() const;
		// Returns the global JS object
		::JSObjectRef globalObject() const;
		// Adds a property to the global object
		void addPropertyToGlobalObject(const std::string& propertyName, const Ti::Value& value) const;
		// Evals a string into the global object and returns the value from eval
		Ti::Value evaluateScript(const std::string& script) const;
		// Evals a string into the object that was passed in and returns the value from eval
		Ti::Value evaluateScriptInObject(const std::string& script, const Ti::Value& object) const;
		// Titanium version of "require" Currently works only with JS files
		Ti::Value requireJS(const std::string& file);

		// For dev and debug, forces GC
		void forceGarbageCollection();
    private:
        // Enforce the Singleton pattern.
        Runtime();
        Runtime(const Runtime&) = delete;
        Runtime& operator=(const Runtime&) = delete;

        ::JSGlobalContextRef globalContext_ { JSGlobalContextCreate(nullptr) };
        ::JSObjectRef        globalObject_  { JSContextGetGlobalObject(globalContext_) };
	};
}

#endif // __TI_RUNTIME__
