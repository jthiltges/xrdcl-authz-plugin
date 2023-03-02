
#include <fcntl.h>

#include "XrdCl/XrdClPlugInInterface.hh"
#include "XrdCl/XrdClFile.hh"
#include "XrdCl/XrdClURL.hh"
#include "XrdVersion.hh"

using namespace XrdCl;

XrdVERSIONINFO(XrdClGetPlugIn, XrdClXcachePlugIn)

namespace XcachePlugIn
{

std::string
CustomizeURL(const std::string &input_url)
{
    URL parsed_url(input_url);
    const char *xcache_host = getenv("XCACHE_HOST");
    const char *xcache_port = getenv("XCACHE_PORT");
    if (parsed_url.GetHostName() == "xcache" && xcache_host)
    {
        parsed_url.SetHostName(xcache_host);
        if (xcache_port) {
            int port = 0;
            try {
                port = std::stol(xcache_port);
            } catch (...) {}
            if (port) parsed_url.SetPort(port);
        }
    }
    return parsed_url.GetURL();
}

class File : public XrdCl::FilePlugIn
{
public:
    File() : m_file(false) {}

    XRootDStatus Open(
        const std::string &url,
        OpenFlags::Flags   flags,
        Access::Mode       mode,
        ResponseHandler   *handler,
        uint16_t           timeout)
    override
    {
        return m_file.Open(CustomizeURL(url), flags, mode, handler, timeout);
    }

private:
    XrdCl::File m_file;
};


class Factory
{
public:

    virtual ~Factory() {}

    virtual FilePlugIn *CreateFile( const std::string &url ) {return new File();}
    virtual FileSystemPlugIn *CreateFileSystem( const std::string &url ) {
        // FileSystem plugin not supported
        return static_cast<XrdCl::FileSystemPlugIn*>(0);
    }
};

}


extern "C"
void *XrdClGetPlugIn( const void *arg )
{
    (void)arg;
    return new XcachePlugIn::Factory();
}
