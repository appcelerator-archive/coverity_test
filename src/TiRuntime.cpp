/**
* Appcelerator Titanium Mobile
* Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
* Licensed under the terms of the Apache Public License
* Please see the LICENSE included with this distribution for details.
*/

#include "TiRuntime.h"
#include "TiValue.h"
//#include "TiProxy.h"
#include <unordered_map>
#include <fstream>
#include <ostream>
#include <sstream>
#include <memory>

// For debug!!
extern "C" JS_EXPORT void JSSynchronousGarbageCollectForDebugging(JSContextRef);

static std::unordered_map <std::string, JSValueRef> js_module_cache;

namespace Ti {
    
    Runtime::Runtime() {
    }
    
    Runtime& Runtime::instance() {
        static Runtime instance;
        return instance;
    }
    
	::JSGlobalContextRef Runtime::globalContext() const {
		return globalContext_;
	}
	
    ::JSObjectRef Runtime::globalObject() const {
		return globalObject_;
	}
    
	void Runtime::addPropertyToGlobalObject(const std::string &propertyName, const Ti::Value& value) const {
		const auto propertyStr = JSStringCreateWithUTF8CString(propertyName.c_str());
		JSObjectSetProperty(globalContext_, (::JSObjectRef)globalObject_, propertyStr, value.JSValueRef(), kJSPropertyAttributeNone, nullptr);
		JSStringRelease(propertyStr);
	}
	
	Value Runtime::evaluateScript(const std::string& script) const {
		// Call eval on the global object
		return evaluateScriptInObject(script, Ti::Value(globalObject_));
	}
	Value Runtime::evaluateScriptInObject(const std::string& script, const Value& object) const
	{
		::JSValueRef exceptionValue{ nullptr };
		auto scriptStr = JSStringCreateWithUTF8CString(script.c_str());
		auto result = JSEvaluateScript(globalContext_, scriptStr, object.JSObjectRef(), nullptr, 0, &exceptionValue);
		JSStringRelease(scriptStr);
		if (exceptionValue) {
			// TODO: Show the red screen of death
			return Ti::Value(exceptionValue);
		}
		return Ti::Value(result);
	}

	Ti::Value Runtime::requireJS(const std::string& file)
	{
		if (js_module_cache.find(file) != js_module_cache.end()) {
			return Ti::Value(js_module_cache[file]);
		}
#if defined(WIN32X) || defined(_WIN32X)
		// Lookup directly under app installed location. eventually we'll store assets under "Assets"
		auto winpath = Windows::ApplicationModel::Package::Current->InstalledLocation->Path;
		auto path = std::string(winpath->Begin(), winpath->End()) + "/" + file + ".js";

		std::string scriptContent;
		std::ifstream ifs(path);
		if (!ifs.is_open()) {
			return Ti::Value::Undefined();
		}
		while (!ifs.eof()) {
			std::string line;
			std::getline(ifs, line);
			scriptContent += line + '\n';
		}
		std::string code = "(function(exports){"
			"var __OXP=exports;var module={'exports':exports};"
			"var __filename=\"" + file + "\";\n"
			+ scriptContent + ";\n"
			"if(module.exports !== __OXP){return module.exports;}"
			"return exports;})({})";

		Ti::Value result = evaluateScript(code);
		JSValueRef jsResult = result.JSValueRef();
		JSValueProtect(globalContext_, jsResult);
		js_module_cache[file] = jsResult;
		return result;
#else
        return Ti::Value();
#endif
	}
	void Runtime::forceGarbageCollection()
	{
		// This is for dev and debug only!!
		JSSynchronousGarbageCollectForDebugging(globalContext_);
	}
}
