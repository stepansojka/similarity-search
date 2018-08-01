#include "core/app-template.hh"
#include "core/reactor.hh"

#include "http/httpd.hh"
#include "http/handlers.hh"
#include "http/function_handlers.hh"

#include <iostream>

using namespace seastar;
using namespace httpd;
using namespace std;

future<> set_routes(http_server_control& server)
{
  return server.set_routes([](routes& r) {
                             r.put(operation_type::GET, "/",
                                   new function_handler([] (const_req req) {
                                                          return "hello, world"; }));
                           });
}

int main(int argc, char** argv)
{
  app_template app;
  http_server_control server;

  return app.run_deprecated(argc, argv, [&server] {
      const uint16_t port = 10000;

      cout << "starting" << endl;

      return server.start("string-matching-server")
      .then([&server] {
              cout << "setting routes" << endl;
              return set_routes(server); })
      .then([&server, port] { return server.listen(port); })
      .then([&server, port] {
              cout << "Seastar HTTP server listening on port " << port << endl;
              engine().at_exit([&server] {
                                 return server.stop();
                               });
            }
        );
    });
}
