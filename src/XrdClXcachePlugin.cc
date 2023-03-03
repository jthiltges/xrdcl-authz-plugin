//------------------------------------------------------------------------------
// Copyright 2020, Morgridge Institute for Research
// Author:  Brian P Bockelman <bbockelman@morgridge.org>
//------------------------------------------------------------------------------
// Copyright (c) 2011-2017 by European Organization for Nuclear Research (CERN)
// Author: Elvin Sindrilaru <esindril@cern.ch>

#include "XrdClXcachePlugin.hh"
#include "XrdClXcache.hh"
#include "XrdVersion.hh"
#include "XrdCl/XrdClDefaultEnv.hh"
#include "XrdCl/XrdClLog.hh"
#include <cstdlib>

XrdVERSIONINFO(XrdClGetPlugIn, XrdClGetPlugIn)

extern "C"
{
  void* XrdClGetPlugIn(const void* arg)
  {
    const std::map<std::string, std::string>* config =
      static_cast< const std::map<std::string, std::string>* >(arg);
    return static_cast<void*>(new xrdcl_xcache::XcacheFactory(config));
  }
}

namespace xrdcl_xcache
{
//------------------------------------------------------------------------------
// Construtor
//------------------------------------------------------------------------------
XcacheFactory::XcacheFactory(const std::map<std::string, std::string>* config)
{
  XrdCl::Log* log = XrdCl::DefaultEnv::GetLog();
  // If any of the parameters specific to this plugin are present then export
  // them as env variables to be used later on if not already set.
  if (config) {
    std::list<std::string> lst_envs {"XCACHE_HOST", "XCACHE_PORT"};

    for (std::list<std::string>::iterator it_env = lst_envs.begin();
	 it_env != lst_envs.end(); ++it_env) {
      std::map<std::string, std::string>::const_iterator it_map =
	config->find(*it_env);

      if (it_map != config->end() && !it_map->second.empty()) {
	if (setenv(it_map->first.c_str(), it_map->second.c_str(), 0)) {
	  log->Error(1, "Failed to set env variable %s from the configuration"
		     " file", it_map->first.c_str());
	}
      }
    }
  }
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
XcacheFactory::~XcacheFactory() {}

//------------------------------------------------------------------------------
// Create a file plug-in for the given URL
//------------------------------------------------------------------------------
XrdCl::FilePlugIn*
XcacheFactory::CreateFile(const std::string& url)
{
  return static_cast<XrdCl::FilePlugIn*>(new XcacheFile());
}

//------------------------------------------------------------------------------
// Create a file system plug-in for the given URL
//------------------------------------------------------------------------------
XrdCl::FileSystemPlugIn*
XcacheFactory::CreateFileSystem(const std::string& url)
{
  //XrdCl::Log* log = XrdCl::DefaultEnv::GetLog();
  //log->Error(1, "FileSystem plugin implementation not supported");
  //return static_cast<XrdCl::FileSystemPlugIn*>(0);
  return static_cast<XrdCl::FileSystemPlugIn*>(new XcacheFileSystem(url));
}

} // namespace xrdcl_xcache
