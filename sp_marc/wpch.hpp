/*
Copyright 1992-2009 Systems Planning (www.systemsplanning.com)

Licensed under the Apache License, Version 2.0 (the "License"); 
you may not use this file except in compliance with the License. 
You may obtain a copy of the License at 

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software 
distributed under the License is distributed on an "AS IS" BASIS, 
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
See the License for the specific language governing permissions and 
limitations under the License.
*/

/*  wpch.hpp
    This is the source for the precompiled header file.
*/

#ifndef WPCH_HPP_INCLUDED
#define WPCH_HPP_INCLUDED

// Un-comment the following if you wish to include windows.h
// in your projects.

// #include <windows.h>

// Also un-comment the following if you wish to include windows.h
// in projects that also use OLE/ActiveX Controls.

// #include <ole2.h>
// #include <olectl.h>
// #define _stdole_HPP_INCLUDED
// #define _StdType_HPP_INCLUDED

#include "wclass.hpp"

#ifdef WJAVA_NATIVE_DLL
    #include "wjnative.hpp"
#endif

//  Include target-wide header files below.


#endif
