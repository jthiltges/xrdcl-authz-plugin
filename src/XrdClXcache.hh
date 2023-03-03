//------------------------------------------------------------------------------
// Copyright 2020, Morgridge Institute for Research
// Author:  Brian P Bockelman <bbockelman@morgridge.org>
//------------------------------------------------------------------------------
// Copyright (c) 2011-2017 by European Organization for Nuclear Research (CERN)
// Author: Elvin Sindrilaru <esindril@cern.ch>
//------------------------------------------------------------------------------

#pragma once
#include "XrdCl/XrdClDefaultEnv.hh"
#include "XrdCl/XrdClPlugInInterface.hh"

using namespace XrdCl;

namespace xrdcl_xcache
{
//------------------------------------------------------------------------------
//! XrdClFile plugin that redirects requests to host "xcache" to an xcache
//! server defined by envvars XCACHE_HOST and XCACHE_PORT.
//------------------------------------------------------------------------------
class XcacheFile: public XrdCl::FilePlugIn
{
public:
  //----------------------------------------------------------------------------
  //! Constructor
  //----------------------------------------------------------------------------
  XcacheFile();

  //----------------------------------------------------------------------------
  //! Destructor
  //----------------------------------------------------------------------------
  virtual ~XcacheFile();

  //----------------------------------------------------------------------------
  //! Open
  //----------------------------------------------------------------------------
  XRootDStatus Open(const std::string& url,
                    OpenFlags::Flags flags,
                    Access::Mode mode,
                    ResponseHandler* handler,
                    uint16_t timeout) override;

  //----------------------------------------------------------------------------
  //! Close
  //----------------------------------------------------------------------------
  XRootDStatus Close(ResponseHandler* handler,
                     uint16_t         timeout) override
  {
    return pFile->Close(handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! Stat
  //----------------------------------------------------------------------------
  XRootDStatus Stat(bool             force,
                    ResponseHandler* handler,
                    uint16_t         timeout) override
  {
    return pFile->Stat(force, handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! Read
  //----------------------------------------------------------------------------
  XRootDStatus Read(uint64_t         offset,
                    uint32_t         size,
                    void*            buffer,
                    ResponseHandler* handler,
                    uint16_t         timeout) override
  {
    return pFile->Read(offset, size, buffer, handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! Read
  //----------------------------------------------------------------------------
  XRootDStatus Read(uint64_t         offset,
                    uint32_t         size,
                    void*            buffer,
                    uint32_t        &bytesRead,
                    uint16_t         timeout) // FIXME: override
  {
    return pFile->Read(offset, size, buffer, bytesRead, timeout);
  }

  //------------------------------------------------------------------------
  //! PgRead
  //------------------------------------------------------------------------
  XRootDStatus PgRead(uint64_t         offset,
                      uint32_t         size,
                      void            *buffer,
                      ResponseHandler *handler,
                      uint16_t         timeout) override
  {
    return pFile->PgRead(offset, size, buffer, handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! Write
  //----------------------------------------------------------------------------
  XRootDStatus Write(uint64_t         offset,
                     uint32_t         size,
                     const void*      buffer,
                     ResponseHandler* handler,
                     uint16_t         timeout) override
  {
    return pFile->Write(offset, size, buffer, handler, timeout);
  }

  XRootDStatus Write(uint64_t          offset,
                     Buffer          &&buffer,
                     ResponseHandler  *handler,
                     uint16_t          timeout) override
  {
    return pFile->Write(offset, std::move(buffer), handler, timeout);
  }

  XRootDStatus Write(uint64_t            offset,
                     uint32_t            size,
                     Optional<uint64_t>  fdoff,
                     int                 fd,
                     ResponseHandler    *handler,
                     uint16_t            timeout) override
  {
    return pFile->Write(offset, size, fdoff, fd, handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! PgWrite
  //----------------------------------------------------------------------------
  XRootDStatus PgWrite(uint64_t               offset,
                       uint32_t               nbpgs,
                       const void            *buffer,
                       std::vector<uint32_t> &cksums,
                       ResponseHandler       *handler,
                       uint16_t               timeout) override
  {
    return pFile->PgWrite(offset, nbpgs, buffer, cksums, handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! Sync
  //----------------------------------------------------------------------------
  XRootDStatus Sync(ResponseHandler* handler,
                    uint16_t         timeout) override
  {
    return pFile->Sync(handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! Truncate
  //----------------------------------------------------------------------------
  XRootDStatus Truncate(uint64_t         size,
                        ResponseHandler* handler,
                        uint16_t         timeout) override
  {
    return pFile->Truncate(size, handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! VectorRead
  //----------------------------------------------------------------------------
  XRootDStatus VectorRead(const ChunkList& chunks,
                          void*            buffer,
                          ResponseHandler* handler,
                          uint16_t         timeout) override
  {
    return pFile->VectorRead(chunks, buffer, handler, timeout);
  }


  //----------------------------------------------------------------------------
  //! VectorWrite
  //----------------------------------------------------------------------------
  XRootDStatus VectorWrite(const ChunkList &chunks,
                           ResponseHandler *handler,
                           uint16_t         timeout) override
  {
    return pFile->VectorWrite(chunks, handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! WriteV
  //----------------------------------------------------------------------------
  XRootDStatus WriteV(uint64_t            offset,
                      const struct iovec *iov,
                      int                 iovcnt,
                      ResponseHandler    *handler,
                      uint16_t            timeout) override
  {
    return pFile->WriteV(offset, iov, iovcnt, handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! Fcntl
  //----------------------------------------------------------------------------
  XRootDStatus Fcntl(const Buffer&    arg,
                     ResponseHandler* handler,
                     uint16_t         timeout) override
  {
    return pFile->Fcntl(arg, handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! Visa
  //----------------------------------------------------------------------------
  XRootDStatus Visa(ResponseHandler* handler,
                    uint16_t         timeout) override
  {
    return pFile->Visa(handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! IsOpen
  //----------------------------------------------------------------------------
  bool IsOpen() const override
  {
    return pFile->IsOpen();
  }

  //----------------------------------------------------------------------------
  //! SetProperty
  //----------------------------------------------------------------------------
  bool SetProperty(const std::string& name,
                   const std::string& value) override
  {
    return pFile->SetProperty(name, value);
  }

  //----------------------------------------------------------------------------
  //! GetProperty
  //----------------------------------------------------------------------------
  bool GetProperty(const std::string& name,
                   std::string& value) const override
  {
    return pFile->GetProperty(name, value);
  }

private:
  bool mIsOpen;
  XrdCl::File* pFile;
};


//----------------------------------------------------------------------------
// A plug-in that forwards all the calls to a XrdCl::FileSystem object
//----------------------------------------------------------------------------
class XcacheFileSystem: public FileSystemPlugIn
{
public:
  //------------------------------------------------------------------------
  // Constructor
  //------------------------------------------------------------------------
  XcacheFileSystem( const std::string &url );

  //------------------------------------------------------------------------
  // Destructor
  //------------------------------------------------------------------------
  virtual ~XcacheFileSystem();

  //------------------------------------------------------------------------
  // Locate
  //------------------------------------------------------------------------
  XRootDStatus Locate(const std::string &path,
                      OpenFlags::Flags   flags,
                      ResponseHandler   *handler,
                      uint16_t           timeout) override
  {
    return pFileSystem->Locate( path, flags, handler, timeout );
  }

  //------------------------------------------------------------------------
  // DeepLocate
  //------------------------------------------------------------------------
  XRootDStatus DeepLocate(const std::string &path,
                          OpenFlags::Flags   flags,
                          ResponseHandler   *handler,
                          uint16_t           timeout) override
  {
    return pFileSystem->DeepLocate( path, flags, handler, timeout );
  }

  //------------------------------------------------------------------------
  // Mv
  //------------------------------------------------------------------------
  XRootDStatus Mv(const std::string &source,
                  const std::string &dest,
                  ResponseHandler   *handler,
                  uint16_t           timeout) override
  {
    return pFileSystem->Mv( source, dest, handler, timeout );
  }

  //------------------------------------------------------------------------
  // Query
  //------------------------------------------------------------------------
  XRootDStatus Query(QueryCode::Code  queryCode,
                     const Buffer    &arg,
                     ResponseHandler *handler,
                     uint16_t         timeout) override
  {
    return pFileSystem->Query( queryCode, arg, handler, timeout );
  }

  //------------------------------------------------------------------------
  // Truncate
  //------------------------------------------------------------------------
  XRootDStatus Truncate(const std::string &path,
                        uint64_t           size,
                        ResponseHandler   *handler,
                        uint16_t           timeout) override
  {
    return pFileSystem->Truncate( path, size, handler, timeout );
  }

  //------------------------------------------------------------------------
  // Rm
  //------------------------------------------------------------------------
  XRootDStatus Rm(const std::string &path,
                  ResponseHandler   *handler,
                  uint16_t           timeout) override
  {
    return pFileSystem->Rm( path, handler, timeout );
  }

  //------------------------------------------------------------------------
  // MkDir
  //------------------------------------------------------------------------
  XRootDStatus MkDir(const std::string &path,
                     MkDirFlags::Flags  flags,
                     Access::Mode       mode,
                     ResponseHandler   *handler,
                     uint16_t           timeout) override
  {
    return pFileSystem->MkDir( path, flags, mode, handler, timeout );
  }

  //------------------------------------------------------------------------
  // RmDir
  //------------------------------------------------------------------------
  XRootDStatus RmDir(const std::string &path,
                     ResponseHandler   *handler,
                     uint16_t           timeout) override
  {
    return pFileSystem->RmDir( path, handler, timeout );
  }

  //------------------------------------------------------------------------
  // ChMod
  //------------------------------------------------------------------------
  XRootDStatus ChMod(const std::string &path,
                     Access::Mode       mode,
                     ResponseHandler   *handler,
                     uint16_t           timeout) override
  {
    return pFileSystem->ChMod( path, mode, handler, timeout );
  }

  //------------------------------------------------------------------------
  // Ping
  //------------------------------------------------------------------------
  XRootDStatus Ping(ResponseHandler *handler,
                    uint16_t         timeout) override
  {
    return pFileSystem->Ping( handler, timeout );
  }

  //------------------------------------------------------------------------
  // Stat
  //------------------------------------------------------------------------
  XRootDStatus Stat(const std::string &path,
                    ResponseHandler   *handler,
                    uint16_t           timeout) override
  {
    return pFileSystem->Stat( path, handler, timeout );
  }

  //------------------------------------------------------------------------
  // StatVFS
  //------------------------------------------------------------------------
  XRootDStatus StatVFS(const std::string &path,
                       ResponseHandler   *handler,
                       uint16_t           timeout) override
  {
    return pFileSystem->StatVFS( path, handler, timeout );
  }

  //------------------------------------------------------------------------
  // Protocol
  //------------------------------------------------------------------------
  XRootDStatus Protocol(ResponseHandler *handler,
                        uint16_t         timeout) override
  {
    return pFileSystem->Protocol( handler, timeout );
  }

  //------------------------------------------------------------------------
  // DirlList
  //------------------------------------------------------------------------
  XRootDStatus DirList(const std::string   &path,
                       DirListFlags::Flags  flags,
                       ResponseHandler     *handler,
                       uint16_t             timeout) override
  {
    return pFileSystem->DirList( path, flags, handler, timeout );
  }

  //------------------------------------------------------------------------
  // SendInfo
  //------------------------------------------------------------------------
  XRootDStatus SendInfo(const std::string &info,
                        ResponseHandler   *handler,
                        uint16_t           timeout) override
  {
    return pFileSystem->SendInfo( info, handler, timeout );
  }

  //------------------------------------------------------------------------
  // Prepare
  //------------------------------------------------------------------------
  XRootDStatus Prepare(const std::vector<std::string> &fileList,
                       PrepareFlags::Flags             flags,
                       uint8_t                         priority,
                       ResponseHandler                *handler,
                       uint16_t                        timeout) override
  {
    return pFileSystem->Prepare( fileList, flags, priority, handler,
                                  timeout );
  }

  //------------------------------------------------------------------------
  // SetXAttr
  //------------------------------------------------------------------------
  XRootDStatus SetXAttr(const std::string           &path,
                        const std::vector<xattr_t>  &attrs,
                        ResponseHandler             *handler,
                        uint16_t                     timeout) override
  {
    return pFileSystem->SetXAttr(path, attrs, handler, timeout);
  }

  //------------------------------------------------------------------------
  // GetXAttr
  //------------------------------------------------------------------------
  XRootDStatus GetXAttr(const std::string               &path,
                        const std::vector<std::string>  &attrs,
                        ResponseHandler                 *handler,
                        uint16_t                         timeout ) override
  {
    return pFileSystem->GetXAttr(path, attrs, handler, timeout);
  }

  //------------------------------------------------------------------------
  // DelXAttr
  //------------------------------------------------------------------------
  XRootDStatus DelXAttr(const std::string               &path,
                        const std::vector<std::string>  &attrs,
                        ResponseHandler                 *handler,
                        uint16_t                         timeout ) override
  {
    return pFileSystem->DelXAttr(path, attrs, handler, timeout);
  }

  //------------------------------------------------------------------------
  // ListXAttr
  //------------------------------------------------------------------------
  XRootDStatus ListXAttr(const std::string         &path,
                         ResponseHandler           *handler,
                         uint16_t                   timeout ) override
  {
    return pFileSystem->ListXAttr(path, handler, timeout);
  }

  //------------------------------------------------------------------------
  // SetProperty
  //------------------------------------------------------------------------
  bool SetProperty(const std::string &name,
                   const std::string &value) override
  {
    return pFileSystem->SetProperty( name, value );
  }

  //------------------------------------------------------------------------
  // GetProperty
  //------------------------------------------------------------------------
  bool GetProperty(const std::string &name,
                   std::string &value ) const override
  {
    return pFileSystem->GetProperty( name, value );
  }

private:
  XrdCl::FileSystem *pFileSystem;
};

//----------------------------------------------------------------------------
//! Get XCache hostname from the environment
//!
//! @return hostname
//----------------------------------------------------------------------------
inline std::string GetXcacheHost();

//----------------------------------------------------------------------------
//! Get XCache port from the environment
//!
//! @return port
//----------------------------------------------------------------------------
inline int GetXcachePort();

//----------------------------------------------------------------------------
//! Construct final URL if there is a proxy prefix URL specified and if the
//! exclusion list is satisfied
//!
//! @param orig_url original url
//!
//! @return final URL
//----------------------------------------------------------------------------
std::string ConstructFinalUrl(const std::string& orig_url);

} // namespace xrdcl_xcache
