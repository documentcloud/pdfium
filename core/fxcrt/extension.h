// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef CORE_FXCRT_EXTENSION_H_
#define CORE_FXCRT_EXTENSION_H_

#include <algorithm>
#include <memory>

#include "core/fxcrt/fx_basic.h"
#include "core/fxcrt/fx_safe_types.h"

class IFXCRT_FileAccess {
 public:
  static IFXCRT_FileAccess* Create();
  virtual ~IFXCRT_FileAccess() {}

  virtual bool Open(const CFX_ByteStringC& fileName, uint32_t dwMode) = 0;
  virtual bool Open(const CFX_WideStringC& fileName, uint32_t dwMode) = 0;
  virtual void Close() = 0;
  virtual FX_FILESIZE GetSize() const = 0;
  virtual FX_FILESIZE GetPosition() const = 0;
  virtual FX_FILESIZE SetPosition(FX_FILESIZE pos) = 0;
  virtual size_t Read(void* pBuffer, size_t szBuffer) = 0;
  virtual size_t Write(const void* pBuffer, size_t szBuffer) = 0;
  virtual size_t ReadPos(void* pBuffer, size_t szBuffer, FX_FILESIZE pos) = 0;
  virtual size_t WritePos(const void* pBuffer,
                          size_t szBuffer,
                          FX_FILESIZE pos) = 0;
  virtual bool Flush() = 0;
  virtual bool Truncate(FX_FILESIZE szFile) = 0;
};

#ifdef PDF_ENABLE_XFA
class CFX_CRTFileAccess : public IFX_FileAccess {
 public:
  CFX_CRTFileAccess();
  ~CFX_CRTFileAccess() override;

  // IFX_FileAccess
  void Release() override;
  IFX_FileAccess* Retain() override;
  void GetPath(CFX_WideString& wsPath) override;
  IFX_SeekableStream* CreateFileStream(uint32_t dwModes) override;

  bool Init(const CFX_WideStringC& wsPath);

 protected:
  CFX_WideString m_path;
  uint32_t m_RefCount;
};
#endif  // PDF_ENABLE_XFA

class CFX_CRTFileStream final : public IFX_SeekableStream {
 public:
  explicit CFX_CRTFileStream(std::unique_ptr<IFXCRT_FileAccess> pFA);
  ~CFX_CRTFileStream() override;

  // IFX_SeekableStream:
  IFX_SeekableStream* Retain() override;
  void Release() override;
  FX_FILESIZE GetSize() override;
  bool IsEOF() override;
  FX_FILESIZE GetPosition() override;
  bool ReadBlock(void* buffer, FX_FILESIZE offset, size_t size) override;
  size_t ReadBlock(void* buffer, size_t size) override;
  bool WriteBlock(const void* buffer, FX_FILESIZE offset, size_t size) override;
  bool Flush() override;

 protected:
  std::unique_ptr<IFXCRT_FileAccess> m_pFile;
  uint32_t m_dwCount;
};

#define FX_MEMSTREAM_BlockSize (64 * 1024)
#define FX_MEMSTREAM_Consecutive 0x01
#define FX_MEMSTREAM_TakeOver 0x02

class CFX_MemoryStream final : public IFX_MemoryStream {
 public:
  explicit CFX_MemoryStream(bool bConsecutive);
  CFX_MemoryStream(uint8_t* pBuffer, size_t nSize, bool bTakeOver);
  ~CFX_MemoryStream() override;

  // IFX_MemoryStream
  IFX_SeekableStream* Retain() override;
  void Release() override;
  FX_FILESIZE GetSize() override;
  bool IsEOF() override;
  FX_FILESIZE GetPosition() override;
  bool ReadBlock(void* buffer, FX_FILESIZE offset, size_t size) override;
  size_t ReadBlock(void* buffer, size_t size) override;
  bool WriteBlock(const void* buffer, FX_FILESIZE offset, size_t size) override;
  bool Flush() override;
  bool IsConsecutive() const override;
  void EstimateSize(size_t nInitSize, size_t nGrowSize) override;
  uint8_t* GetBuffer() const override;
  void AttachBuffer(uint8_t* pBuffer,
                    size_t nSize,
                    bool bTakeOver = false) override;
  void DetachBuffer() override;

 protected:
  CFX_ArrayTemplate<uint8_t*> m_Blocks;
  uint32_t m_dwCount;
  size_t m_nTotalSize;
  size_t m_nCurSize;
  size_t m_nCurPos;
  size_t m_nGrowSize;
  uint32_t m_dwFlags;
  bool ExpandBlocks(size_t size);
};

#ifdef __cplusplus
extern "C" {
#endif
#define MT_N 848
#define MT_M 456
#define MT_Matrix_A 0x9908b0df
#define MT_Upper_Mask 0x80000000
#define MT_Lower_Mask 0x7fffffff
struct FX_MTRANDOMCONTEXT {
  FX_MTRANDOMCONTEXT() {
    mti = MT_N + 1;
    bHaveSeed = false;
  }
  uint32_t mti;
  bool bHaveSeed;
  uint32_t mt[MT_N];
};
#if _FXM_PLATFORM_ == _FXM_PLATFORM_WINDOWS_
bool FX_GenerateCryptoRandom(uint32_t* pBuffer, int32_t iCount);
#endif
#ifdef __cplusplus
}
#endif

#endif  // CORE_FXCRT_EXTENSION_H_
