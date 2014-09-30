#include "TiRuntime.h"
#include "TiValue.h"

#include "TiProxy.h"
//#include "TiProxy2.h"
//#include "ExampleProxy.h"

#include <string>
#include <iostream>

#include <JavaScriptCore/JavaScript.h>
#include "gtest/gtest.h"

TEST(TiCoreTests, TestCrash) {
	auto& context = Ti::Runtime::instance();
    auto ctx = context.globalContext();
	
    auto globalObject = Ti::Value(context.globalObject());

	// This produces SEGFAULT
	//JSGlobalContextRelease(ctx);

	// Unfortunately, it looks like we can't use Death Tests on OS X
    //due to JavaScriptCore's multithreaded nature. Uncomment this
    //line to see the following output.
    //ASSERT_DEATH(JSGlobalContextRelease(ctx), ".*");

	// 1: Test command: /Users/matt/Documents/Appcelerator/src/Appcelerator/titanium_mobile_windows/test/TiCoreTests/build.debug/TiCoreTests
	// 1: Test timeout computed to be: 9.99988e+06
	// 1: Running main() from gtest_main.cc
	// 1: [==========] Running 1 test from 1 test case.
	// 1: [----------] Global test environment set-up.
	// 1: [----------] 1 test from TiCoreTests
	// 1: [ RUN      ] TiCoreTests.TestCrash
	// 1: TiValue: ctor called (JSValueRef)
	// 1: 
	// 1: [WARNING] /Users/matt/Documents/Appcelerator/src/download/gtest-1.7.0/src/gtest-death-test.cc:825:: Death tests use fork(), which is unsafe particularly in a threaded context. For this test, Google Test detected 11 threads.
	// 1: /Users/matt/Documents/Appcelerator/src/Appcelerator/titanium_mobile_windows/test/TiCoreTests/TiCoreTests.cpp:20: Failure
	// 1: Death test: JSGlobalContextRelease(ctx)
	// 1:     Result: failed to die.
	// 1:  Error msg:
	// 1: [  DEATH   ] The process has forked and you cannot use this CoreFoundation functionality safely. You MUST exec().
	// 1: [  DEATH   ] Break on __THE_PROCESS_HAS_FORKED_AND_YOU_CANNOT_USE_THIS_COREFOUNDATION_FUNCTIONALITY___YOU_MUST_EXEC__() to debug.
	// 1: [  DEATH   ] The process has forked and you cannot use this CoreFoundation functionality safely. You MUST exec().
	// 1: [  DEATH   ] Break on __THE_PROCESS_HAS_FORKED_AND_YOU_CANNOT_USE_THIS_COREFOUNDATION_FUNCTIONALITY___YOU_MUST_EXEC__() to debug.
	// 1: [  DEATH   ] The process has forked and you cannot use this CoreFoundation functionality safely. You MUST exec().
	// 1: [  DEATH   ] Break on __THE_PROCESS_HAS_FORKED_AND_YOU_CANNOT_USE_THIS_COREFOUNDATION_FUNCTIONALITY___YOU_MUST_EXEC__() to debug.
	// 1: [  DEATH   ] The process has forked and you cannot use this CoreFoundation functionality safely. You MUST exec().
	// 1: [  DEATH   ] Break on __THE_PROCESS_HAS_FORKED_AND_YOU_CANNOT_USE_THIS_COREFOUNDATION_FUNCTIONALITY___YOU_MUST_EXEC__() to debug.
	// 1: [  DEATH   ] The process has forked and you cannot use this CoreFoundation functionality safely. You MUST exec().
	// 1: [  DEATH   ] Break on __THE_PROCESS_HAS_FORKED_AND_YOU_CANNOT_USE_THIS_COREFOUNDATION_FUNCTIONALITY___YOU_MUST_EXEC__() to debug.
	// 1: [  DEATH   ] The process has forked and you cannot use this CoreFoundation functionality safely. You MUST exec().
	// 1: [  DEATH   ] Break on __THE_PROCESS_HAS_FORKED_AND_YOU_CANNOT_USE_THIS_COREFOUNDATION_FUNCTIONALITY___YOU_MUST_EXEC__() to debug.
	// 1: [  DEATH   ] The process has forked and you cannot use this CoreFoundation functionality safely. You MUST exec().
	// 1: [  DEATH   ] Break on __THE_PROCESS_HAS_FORKED_AND_YOU_CANNOT_USE_THIS_COREFOUNDATION_FUNCTIONALITY___YOU_MUST_EXEC__() to debug.
	// 1: [  DEATH   ] The process has forked and you cannot use this CoreFoundation functionality safely. You MUST exec().
	// 1: TiValue: dtor called
	// 1: [  DEATH   ] Break on __THE_PROCESS_HAS_FORKED_AND_YOU_CANNOT_USE_THIS_COREFOUNDATION_FUNCTIONALITY___YOU_MUST_EXEC__() to debug.
	// 1: [  DEATH   ] The process has forked and you cannot use this CoreFoundation functionality safely. You MUST exec().
	// 1: [  DEATH   ] Break on __THE_PROCESS_HAS_FORKED_AND_YOU_CANNOT_USE_THIS_COREFOUNDATION_FUNCTIONALITY___YOU_MUST_EXEC__() to debug.
	// 1: [  DEATH   ] The process has forked and you cannot use this CoreFoundation functionality safely. You MUST exec().
	// 1: [  DEATH   ] Break on __THE_PROCESS_HAS_FORKED_AND_YOU_CANNOT_USE_THIS_COREFOUNDATION_FUNCTIONALITY___YOU_MUST_EXEC__() to debug.
	// 1: [  DEATH   ] 
	// 1: [  FAILED  ] TiCoreTests.TestCrash (9 ms)
	// 1: [----------] 1 test from TiCoreTests (9 ms total)
	// 1: 
	// 1: [----------] Global test environment tear-down
	// 1: [==========] 1 test from 1 test case ran. (9 ms total)
	// 1: [  PASSED  ] 0 tests.
	// 1: [  FAILED  ] 1 test, listed below:
	// 1: [  FAILED  ] TiCoreTests.TestCrash
	// 1: 
	// 1:  1 FAILED TEST
	// 1/1 Test #1: TiCoreTests ......................***Failed    0.01 sec
	// 
}
