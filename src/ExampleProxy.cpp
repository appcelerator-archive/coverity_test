//
//  ExampleProxy.cpp
//  TiValue
//
//  Created by Pedro Enrique on 9/6/14.
//  Copyright (c) 2014 Pedro Enrique. All rights reserved.
//

#include "ExampleProxy.h"

namespace Ti
{
	// These are JavaScript methods they will call class methods
	const JSStaticFunction ExampleProxy::ClassMethods[] = {
		{"saySomething", _saySomething, kJSPropertyAttributeNone},
		{0,0,0}
	};
	
	ExampleProxy::~ExampleProxy()
	{
		
	}
    ExampleProxy::ExampleProxy(const char* name) : Ti::Proxy(name){
    }
    
    // Called when this js proxy is created
    void ExampleProxy::initializer(const Ti::Value& val)
	{
		// Remember to call super
		Proxy::initializer(val);
	}
    // Called when this js proxy is about to get garbage collected
	void ExampleProxy::finalizer()
	{
		// Remember to call super
		Proxy::finalizer();
	}
	
	// JavaScriptCore calls an auto-generated _saySomething static function
	// Which then calls this function
	Ti::Value ExampleProxy::saySomething(const Ti::Value& args)
	{
		std::cout << args.toString() << std::endl;
		return Ti::Value(true);
	}
}

