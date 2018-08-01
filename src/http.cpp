#include "core/app-template.hh"
#include "core/reactor.hh"

#include "http/httpd.hh"
#include "http/handlers.hh"
#include "http/function_handlers.hh"

#include <iostream>

using namespace seastar;
using namespace httpd;
using namespace std;

auto set_routes(http_server_control& server)
{
  return server.set_routes([](auto& routes) {
                             routes.put(operation_type::GET, "/",
                                   new function_handler([] (auto req) {
                                                          return "hello, world"; }));
                           });
}

int main(int argc, char** argv)
{
  app_template app;
  http_server_control server;

  return app.run_deprecated(argc, argv, [&server] {
      const uint16_t port = 10000;

      return server.start("string-matching-server")
      .then([&server] {
              return set_routes(server); })
      .then([&server, port] { return server.listen(port); })
      .then([&server, port] {
              cout << "HTTP server listening on port " << port << endl;
              engine().at_exit([&server] {
                                 return server.stop();
                               });
            }
        );
    });
}
