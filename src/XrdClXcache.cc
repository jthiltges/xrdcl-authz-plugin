//------------------------------------------------------------------------------
// Copyright 2020, Morgridge Institute for Research
// Author:  Brian P Bockelman <bbockelman@morgridge.org>
//------------------------------------------------------------------------------
// Copyright (c) 2011-2017 by European Organization for Nuclear Research (CERN)
// Author: Elvin Sindrilaru <esindril@cern.ch>
//------------------------------------------------------------------------------

#include "XrdClXcache.hh"
#include <cstdlib>
#include "XrdCl/XrdClLog.hh"

namespace xrdcl_xcache
{
//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
XcacheFile::XcacheFile():
  mIsOpen(false),
  pFile(0)
{}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
XcacheFile::~XcacheFile()
{
  if (pFile) {
    delete pFile;
  }
}

//------------------------------------------------------------------------------
// Open
//------------------------------------------------------------------------------
XRootDStatus
XcacheFile::Open(const std::string& url,
                 OpenFlags::Flags flags,
                 Access::Mode mode,
                 ResponseHandler* handler,
                 uint16_t timeout)
{
  XRootDStatus st;

  if (mIsOpen) {
    st = XRootDStatus(stError, errInvalidOp);
    return st;
  }

  pFile = new XrdCl::File(false);
  std::string open_url = ConstructFinalUrl(url);
  st = pFile->Open(open_url, flags, mode, handler, timeout);

  if (st.IsOK()) {
    mIsOpen = true;
  }

  return st;
}

//==============================================================================

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
XcacheFileSystem::XcacheFileSystem( const std::string &url )
    {
      XrdCl::Log *log = DefaultEnv::GetLog();
      log->Debug( 1, "Calling XcacheFileSystem::XcacheFileSystem" );
      std::string open_url = ConstructFinalUrl(url);
      pFileSystem = new XrdCl::FileSystem( URL(open_url), false );
    }

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
XcacheFileSystem::~XcacheFileSystem()
{
  delete pFileSystem;
}

//==============================================================================

//------------------------------------------------------------------------------
// Get XCache hostname
//------------------------------------------------------------------------------
std::string
GetXcacheHost()
{
  const char *host = getenv("XCACHE_HOST");

  if (host) { return host; }
  else { return ""; }
}

//------------------------------------------------------------------------------
// Get XCache port
//------------------------------------------------------------------------------
int
GetXcachePort()
{
  int port = 0;
  try {
    port = std::stoi(getenv("XCACHE_PORT"));
  } catch (...) {}

  return port;
}

//------------------------------------------------------------------------------
// Construct final Url
//------------------------------------------------------------------------------
std::string
ConstructFinalUrl(const std::string& orig_surl)
{
  XrdCl::URL final_url(orig_surl);
  std::string xcache_host = GetXcacheHost();
  int xcache_port = GetXcachePort();
  XrdCl::Log* log = DefaultEnv::GetLog();
  log->Debug(1, "url=%s, xcache_host=%s, xcache_port=%i", orig_surl.c_str(), xcache_host.c_str(), xcache_port);

  if (final_url.GetHostName() == "xcache" && !xcache_host.empty()) {
    final_url.SetHostName(xcache_host);
    if (xcache_port) {
      final_url.SetPort(xcache_port);
    }
  }

  std::string final_surl = final_url.GetURL();

  log->Debug(1, "final_url=%s", final_surl.c_str());
  return final_surl;
}

} // namespace xrdcl_xcache
