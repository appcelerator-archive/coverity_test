/**
* Appcelerator Titanium Mobile
* Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
* Licensed under the terms of the Apache Public License
* Please see the LICENSE included with this distribution for details.
*/

#include "TiProxy.h"
#include "TiValue.h"

namespace Ti
{
	Proxy::Proxy(const char* name) : BaseProxy(name)
	{

	}
	Proxy::~Proxy()
	{

	}
	// Call super
	void Proxy::initializer(const Ti::Value& args)
	{
		BaseProxy::initializer(args);
	}
	void Proxy::finalizer()
	{
		BaseProxy::finalizer();
	}
	// Every proxy will have these two methods
	// TODO: make them private by prepending "_" to the string 
	JSStaticFunction const Proxy::ClassMethods[] = {
		{ "eventRemoved", _eventRemoved, kJSPropertyAttributeDontDelete },
        { "eventAdded", _eventAdded, kJSPropertyAttributeDontDelete },
		{0,0,0}
	};
	// Fires a callback on this object
	void Proxy::fireCallback(const std::string& name, const Ti::Value& val)
	{
		// Grab a hold of this object
		Ti::Value thisObject(getJSObject());

		Ti::Value prop = thisObject.getProperty(name);
		if (prop.isUndefined()) return;

		const JSValueRef args[] = {
			val.JSValueRef()
		};

		JSValueRef exception = nullptr;
		JSObjectCallAsFunction(Runtime::instance().globalContext(), prop.JSObjectRef(), getJSObject(), 1, args, &exception);
		if (exception != nullptr) {
			// Place a break point to see the exception
			std::string str = Ti::Value(exception).toString();

        }
	}

	void Proxy::fireEvent(const std::string& name, const Ti::Value& val)
	{
		Ti::Value eventName(name);
		const JSValueRef args[] = {
			eventName.JSValueRef(),
			val.JSValueRef()
		};
		Ti::Value thisObject(getJSObject());
		Ti::Value fireEvent = thisObject.getProperty("fireEvent");
		JSObjectCallAsFunction(Runtime::instance().globalContext(), fireEvent.JSObjectRef(), getJSObject(), 2, args, nullptr);
	}
	// Public API
	
	Ti::Value Proxy::eventRemoved(const Ti::Value& args)
	{
		// for subclass
		return Ti::Value::Undefined();
	}
	Ti::Value Proxy::eventAdded(const Ti::Value& args)
	{
		// for subclass
		return Ti::Value::Undefined();
	}

}
