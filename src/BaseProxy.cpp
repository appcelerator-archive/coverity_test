/**
* Appcelerator Titanium Mobile
* Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
* Licensed under the terms of the Apache Public License
* Please see the LICENSE included with this distribution for details.
*/


#include "BaseProxy.h"


JSStaticFunction const BaseProxy::ClassMethods[] = {
	{"testMethod", _testMethod, kJSPropertyAttributeNone},
	{0,0,0}
};


void BaseProxy::protect()
{
	if (!protected_) {
		JSValueProtect(Ti::Runtime::instance().globalContext(), getJSObject());
	}
	protected_ = true;
}
void BaseProxy::unprotect()
{
	if (protected_) {
		JSValueUnprotect(Ti::Runtime::instance().globalContext(), getJSObject());
	}
	protected_ = false;
}

void BaseProxy::setJSObject(JSObjectRef obj)
{
	jsObject = obj;
}
JSObjectRef BaseProxy::getJSObject() const
{
	return jsObject;
}
void BaseProxy::finalizer()
{
	// for subcass
}
void BaseProxy::initializer(const Ti::Value& args)
{
	// for subclass
}
Ti::Value BaseProxy::testMethod(const Ti::Value& args)
{
	return Ti::Value(args.toString());
}
