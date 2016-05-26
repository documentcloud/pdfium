// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef XFA_FXJSE_CONTEXT_H_
#define XFA_FXJSE_CONTEXT_H_

#include <memory>
#include <vector>

#include "core/fxcrt/include/fx_basic.h"
#include "v8/include/v8.h"
#include "xfa/fxjse/include/fxjse.h"

class CFXJSE_Class;
class CFXJSE_Value;
struct FXJSE_CLASS;

class CFXJSE_Context {
 public:
  static CFXJSE_Context* Create(v8::Isolate* pIsolate,
                                const FXJSE_CLASS* lpGlobalClass = nullptr,
                                void* lpGlobalObject = nullptr);
  ~CFXJSE_Context();

  V8_INLINE v8::Isolate* GetRuntime(void) { return m_pIsolate; }
  void GetGlobalObject(CFXJSE_Value* pValue);
  FX_BOOL ExecuteScript(const FX_CHAR* szScript,
                        CFXJSE_Value* lpRetValue,
                        CFXJSE_Value* lpNewThisObject = nullptr);

 protected:
  CFXJSE_Context();
  CFXJSE_Context(const CFXJSE_Context&);
  explicit CFXJSE_Context(v8::Isolate* pIsolate);
  CFXJSE_Context& operator=(const CFXJSE_Context&);

  v8::Global<v8::Context> m_hContext;
  v8::Isolate* m_pIsolate;
  std::vector<std::unique_ptr<CFXJSE_Class>> m_rgClasses;

  friend class CFXJSE_Class;
  friend class CFXJSE_ScopeUtil_IsolateHandleContext;
  friend class CFXJSE_ScopeUtil_IsolateHandleRootOrNormalContext;
};

v8::Local<v8::Object> FXJSE_CreateReturnValue(v8::Isolate* pIsolate,
                                              v8::TryCatch& trycatch);

#endif  // XFA_FXJSE_CONTEXT_H_
