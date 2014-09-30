//
//  ExampleProxy2.h
//  TiValue
//
//  Created by Matt Langston on 9/8/14.
//  Copyright (c) 2014 Pedro Enrique. All rights reserved.
//

#ifndef __TiValue__ExampleProxy2__
#define __TiValue__ExampleProxy2__

#include "TiProxy2.h"

namespace Ti
{
	class ExampleProxy2 : public Proxy2
	{
//		TI_CREATE_PROXY(ExampleProxy)
//		
//		// Sample JavaScript method
//		TI_CREATE_METHOD(ExampleProxy, saySomething);
		
	public:
		ExampleProxy2() : Proxy2() {
        };
	};
}

#endif /* defined(__TiValue__ExampleProxy2__) */
