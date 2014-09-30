//
//  ExampleProxy.h
//  TiValue
//
//  Created by Pedro Enrique on 9/6/14.
//  Copyright (c) 2014 Pedro Enrique. All rights reserved.
//

#ifndef __TiValue__ExampleProxy__
#define __TiValue__ExampleProxy__

#include "TiProxy.h"

namespace Ti
{
	class ExampleProxy : public Proxy
	{
		TI_CREATE_PROXY(ExampleProxy)
    protected:
        // Required
        static JSClassRef Parent() { return Proxy::ClassDef(); }
        // Required
        ExampleProxy(const char* name);
        // Optional
        virtual ~ExampleProxy();
        // Optional
        virtual void initializer(const Ti::Value& args);
        // Optional
        virtual void finalizer();

    private:
        // Sample JavaScript method
        TI_CREATE_METHOD(ExampleProxy, saySomething);
        
		// something private here
	};
}

#endif /* defined(__TiValue__ExampleProxy__) */
