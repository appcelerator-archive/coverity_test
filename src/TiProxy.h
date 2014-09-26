/**
* Appcelerator Titanium Mobile
* Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
* Licensed under the terms of the Apache Public License
* Please see the LICENSE included with this distribution for details.
*/

#ifndef __TI_PROXY__
#define __TI_PROXY__

#include <iostream>
#include "BaseProxy.h"

// This is the main Proxy, every Proxy will inherit from this 
namespace Ti
{
	class Proxy : public BaseProxy
	{
		TI_CREATE_PROXY(Proxy)
	protected:
		Proxy(const char* name);
		virtual ~Proxy();
		virtual void initializer(const Ti::Value& args);
		virtual void finalizer();
		static JSClassRef Parent() { return BaseProxy::ClassDef(); }
	public:
		// Call this to fire an event to the JS
		// fireEvent("click", Ti::Value());
		void fireEvent(const std::string&, const Ti::Value&);
		// Call this to fire a callback, for example
		// JS:
		// myPrpxy.onLoad = function(e) {};
		// C++
		// fireCallback("onLoad", Ti::Value());
		void fireCallback(const std::string&, const Ti::Value&);
	protected:
		// Called when an event has been removed from the object
		TI_CREATE_METHOD(Proxy, eventRemoved);
		// Called when an event has been added to the object
		TI_CREATE_METHOD(Proxy, eventAdded);
        
	private:
	};
}
#endif /* defined(__TI_PROXY__) */
