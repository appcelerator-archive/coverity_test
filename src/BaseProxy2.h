//
//  BaseProxy2.h
//  TiValue
//
//  Created by Matt Langston on 9/8/14.
//  Copyright (c) 2014 Pedro Enrique. All rights reserved.
//

#ifndef __TiValue__BaseProxy2__
#define __TiValue__BaseProxy2__

#include "TiValue.h"
#include <JavaScriptCore/JavaScript.h>

template<typename Derived> class BaseProxy2 {
    
public:
    
	static Ti::Value CreateConstructor() 	{
		return Ti::Value(JSObjectMakeConstructor(Ti::Runtime::instance().globalContext(), ClassDef(), ConstructorCallback));
	}
    
	static JSClassRef ClassDef() {
		static JSClassRef classDef = nullptr;
		if (classDef == nullptr) {
			JSClassDefinition def = kJSClassDefinitionEmpty;
			def.className         = Derived::Name;
			def.parentClass       = Derived::Parent;
			def.staticFunctions   = Derived::ClassMethods;
			def.callAsConstructor = ConstructorCallback;
			def.finalize          = Finalizer;
			classDef              = JSClassCreate(&def);
		}
        
		return classDef;
	}
    
protected:
    
	BaseProxy2() {
	}
    
	virtual ~BaseProxy2() {
	}
    
	virtual void onFinalizer() {
    }
    
	virtual void onInitializer() {
    }
    
private:
	
	static JSObjectRef ConstructorCallback (JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
		auto proxy = new Derived();
		Ti::Value jsObject(JSObjectMake(Ti::Runtime::instance().globalContext(), ClassDef(), proxy));
		proxy->setJSObject(jsObject.JSObjectRef());
		proxy->onInitializer();
		return jsObject.JSObjectRef();
	}
    
	static void Finalizer(JSObjectRef obj) {
		auto proxy = static_cast<Derived*>(JSObjectGetPrivate(obj));
		if(proxy == nullptr) {
            return;
        }
		proxy->onFinalizer();
		delete proxy;
		proxy = nullptr;
		JSObjectSetPrivate(obj, nullptr);
	}
    
	void setJSObject(JSObjectRef obj) {
		object_ = obj;
	}
    
	JSObjectRef object_;
};


#endif /* defined(__TiValue__BaseProxy2__) */
