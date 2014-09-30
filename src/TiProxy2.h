//
//  Proxy2.h
//  TiValue
//
//  Created by Matt Langston on 9/8/14.
//  Copyright (c) 2014 Pedro Enrique. All rights reserved.
//

#ifndef __TiValue__Proxy2__
#define __TiValue__Proxy2__

#include "BaseProxy2.h"

namespace Ti
{
	class Proxy2 : public BaseProxy2<Proxy2> {
	public:
        static constexpr const char*      Name           { "Proxy2" };
        static constexpr JSClassRef       Parent         { nullptr  };
        static constexpr JSStaticFunction ClassMethods[] { {0, 0, 0} };

        Proxy2() : BaseProxy2<Proxy2>() {
        };
	};
}

#endif /* defined(__TiValue__Proxy2__) */
