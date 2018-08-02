#include "bk_tree.hpp"

#include "core/sstring.hh"
#include "core/app-template.hh"
#include "core/reactor.hh"

#include "http/httpd.hh"
#include "http/handlers.hh"
#include "http/function_handlers.hh"

#include <iostream>
#include <tuple>

using namespace bk;
using namespace seastar;
using namespace httpd;
using namespace std;

template <typename T>
auto set_routes(http_server_control& server, bk_tree<T>& tree)
{
  return server.set_routes([&tree](auto& routes) {
                             routes.put(operation_type::POST, "/db",
                                        new function_handler([&tree] (const_req req) {
                                                               auto l = req.get_header("content-lenght");
                                                               tree.insert(req.content);
                                                               cout << l << endl;
                                                               return ""; }));


                             routes.put(operation_type::GET, "/similarity",
                                        new function_handler([&tree] (const_req req) {
                                                               auto s = req.get_query_param("s");
                                                               auto result = tree.search(s);
                                                               return get<0>(result); }));

                             routes.put(operation_type::GET, "/",
                                        new function_handler([] (auto req) {
                                                               return "hello, world"; }));
                           });
}

int main(int argc, char** argv)
{
  app_template app;
  http_server_control server;

  // TODO: this will obviously match the empty string
  bk_tree tree(""s);

  return app.run_deprecated(argc, argv, [&server, &tree] {
      const uint16_t port = 10000;


      return server.start("string matching server")
        .then([&server, &tree] { return set_routes(server, tree); })
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
