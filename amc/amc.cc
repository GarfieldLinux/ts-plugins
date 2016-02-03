#include <iostream>
#include <atscppapi/GlobalPlugin.h>
#include <atscppapi/PluginInit.h>

using namespace atscppapi;

class AMCPlugin : public GlobalPlugin
{
public:
  AMCPlugin() { registerHook(HOOK_READ_REQUEST_HEADERS_PRE_REMAP); }

  virtual void
  handleReadRequestHeadersPreRemap(Transaction &transaction)
  {
    std::cout << "Hello from handleReadRequesHeadersPreRemap!" << std::endl;
    transaction.resume();
  }
};

void
TSPluginInit(int argc, const char *argv[])
{
  char name[256];

  if (argc > 1) sprintf(name, "amc-%s", argv[1]);
  else sprintf(name, "amc");

  RegisterGlobalPlugin(name, "apache", "solidwallofcode@yahoo-inc.com");
  new AMCPlugin;
}
