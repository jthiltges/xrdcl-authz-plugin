//------------------------------------------------------------------------------
// Copyright 2020, Morgridge Institute for Research
// Author:  Brian P Bockelman <bbockelman@morgridge.org>
//------------------------------------------------------------------------------
// Copyright (c) 2011-2017 by European Organization for Nuclear Research (CERN)
// Author: Elvin Sindrilaru <esindril@cern.ch>
//------------------------------------------------------------------------------

#pragma once
#include "XrdCl/XrdClPlugInInterface.hh"

namespace xrdcl_xcache
{
//------------------------------------------------------------------------------
//! XrdCl Xcache plugin factory
//------------------------------------------------------------------------------
class XcacheFactory: public XrdCl::PlugInFactory
{
public:
  //----------------------------------------------------------------------------
  //! Construtor
  //!
  //! @param config map containing configuration parameters
  //----------------------------------------------------------------------------
  XcacheFactory(const std::map<std::string, std::string>* config);

  //----------------------------------------------------------------------------
  //! Destructor
  //----------------------------------------------------------------------------
  virtual ~XcacheFactory();

  //----------------------------------------------------------------------------
  //! Create a file plug-in for the given URL
  //----------------------------------------------------------------------------
  virtual XrdCl::FilePlugIn* CreateFile(const std::string& url);

  //----------------------------------------------------------------------------
  //! Create a file system plug-in for the given URL
  //----------------------------------------------------------------------------
  virtual XrdCl::FileSystemPlugIn* CreateFileSystem(const std::string& url);
};

} // namespace xrdcl_xcache
