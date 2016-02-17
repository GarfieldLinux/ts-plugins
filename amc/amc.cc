#include <iostream>
#include <atscppapi/GlobalPlugin.h>
#include <atscppapi/PluginInit.h>

using namespace atscppapi;

class AMCPlugin : public GlobalPlugin
{
  typedef AMCPlugin self;
  typedef GlobalPlugin super;
  
public:
  AMCPlugin(char const* name) {
    _name.assign(name);
    registerHook(HOOK_READ_REQUEST_HEADERS_PRE_REMAP);
    registerHook(HOOK_SEND_RESPONSE_HEADERS);
    registerHook(HOOK_PLUGINS_LOADED, static_cast<LifecycleCallback>(&self::pluginsLoadedCallback));
  }

  virtual void
  handleReadRequestHeadersPreRemap(Transaction &transaction)
  {
    std::cout << "CB: Read Request Header - " << _name << std::endl;
    transaction.setHookPriorityThreshold(HOOK_SEND_RESPONSE_HEADERS, 1250);
    transaction.resume();
  }
  virtual void
  handleSendResponseHeaders(Transaction &transaction)
  {
    std::cout << "CB: Send Response Header - " << _name << std::endl;
    transaction.setHookPriorityThreshold(HOOK_SEND_RESPONSE_HEADERS, TS_HOOK_PRIORITY_UNSET);
    transaction.resume();
  }
private:
  void pluginsLoadedCallback(void* data) {
    std::cout << "CB: " << _name << " Plugins Loaded" << std::endl;
  }
  
  std::string _name;
};

void
TSPluginInit(int argc, const char *argv[])
{
  char name[256];

  if (argc > 1) sprintf(name, "amc-%s", argv[1]);
  else sprintf(name, "amc");

  RegisterGlobalPlugin(name, "apache", "solidwallofcode@yahoo-inc.com");
  new AMCPlugin(name);
}
