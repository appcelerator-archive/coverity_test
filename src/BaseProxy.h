/**
* Appcelerator Titanium Mobile
* Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
* Licensed under the terms of the Apache Public License
* Please see the LICENSE included with this distribution for details.
*/


#ifndef __TI_BASE_PROXY__
#define __TI_BASE_PROXY__

#include <iostream>
#include <JavaScriptCore/JavaScript.h>
#include "TiRuntime.h"
#include "TiValue.h"

#ifndef TI_CREATE_METHOD
#define TI_CREATE_METHOD(PROXY_NAME, METHOD_NAME) \
virtual Ti::Value METHOD_NAME(const Ti::Value& args); \
static JSValueRef _##METHOD_NAME(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) \
{ \
	PROXY_NAME* proxy = static_cast<PROXY_NAME*>(JSObjectGetPrivate(thisObject)); \
	return proxy->METHOD_NAME(Ti::Value(arguments, argumentCount)).JSValueRef(); \
};
#endif

#ifndef TI_CREATE_PROXY
#define TI_CREATE_PROXY(PROXY_NAME) \
public:	\
static JSClassRef ClassDef() \
{ \
	static JSClassRef classDef = nullptr; \
	if(classDef == nullptr) { \
		JSClassDefinition def = kJSClassDefinitionEmpty; \
		def.className = #PROXY_NAME; \
		def.staticFunctions = PROXY_NAME::ClassMethods; \
		def.callAsConstructor = PROXY_NAME::ConstructorCallback; \
		def.parentClass = PROXY_NAME::Parent(); \
		def.finalize = PROXY_NAME::Finalizer; \
		classDef = JSClassCreate(&def); \
	} \
	return classDef; \
} \
static Ti::Value CreateConstructor() \
{ \
	return Ti::Value(JSObjectMakeConstructor(Ti::Runtime::instance().globalContext(), PROXY_NAME::ClassDef(), PROXY_NAME::ConstructorCallback)); \
} \
private: \
static const JSStaticFunction ClassMethods[]; \
static JSObjectRef ConstructorCallback (JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) \
{ \
	PROXY_NAME* proxy = new PROXY_NAME(#PROXY_NAME); \
	Ti::Value object = Ti::Value(JSObjectMake(Ti::Runtime::instance().globalContext(), PROXY_NAME::ClassDef(), proxy)); \
	proxy->setJSObject(object.JSObjectRef());	\
	proxy->initializer(Ti::Value(arguments, argumentCount)); \
	return object.JSObjectRef(); \
} \
static void Finalizer(JSObjectRef obj) \
{ \
	PROXY_NAME* proxy = static_cast<PROXY_NAME*>(JSObjectGetPrivate(obj)); \
	if(proxy == nullptr) return; \
	proxy->finalizer(); \
	delete proxy; \
	proxy = nullptr; \
	JSObjectSetPrivate(obj, nullptr); \
} 
#endif

class BaseProxy {
	TI_CREATE_PROXY(BaseProxy)
protected:
	BaseProxy(const char* name) : proxy_name_(name){
	}
	static JSClassRef Parent() { return nullptr; }
	virtual ~BaseProxy() {}
	virtual void initializer(const Ti::Value& args);
	virtual void finalizer();
	virtual void protect();
	virtual void unprotect();
	void setJSObject(JSObjectRef obj); 
	JSObjectRef getJSObject() const;
	TI_CREATE_METHOD(BaseProxy, testMethod);
private:
	JSObjectRef jsObject;
	bool protected_{ false };
	std::string proxy_name_;
};
#endif /* defined(__TI_BASE_PROXY__) */
