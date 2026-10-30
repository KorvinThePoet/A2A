#pragma once

#include <string>
#include <map>
#include <functional>

#include "response.hpp"
#include "requests.hpp"
#include "../db/storage.hpp"
#include "../tools/strtools.hpp"

// используя parser, чтобы получить payload и превраитить его в один из  user/models.hpp 
// чтобы потом использовать эту dto в users/controllers и тддтдддтдтд

using RouterFunk = std::function<Response(const Request&, Storage&)>;

namespace Requests {
class RouterManager{
private:
    struct Element{
        std::string prefix;
        RouterFunk controller;
    };

    std::vector<Element> controllers;
public:
    void register_router(std::string pref, RouterFunk cntrl) {
        controllers.push_back({pref, std::move(cntrl)});
    }

    Response route(const Request& req, Storage& storage) {
        if (req.is_bad_request()) {
            return Response(400, {}, "'msg': 'bad request'");
        }
        std::string route = req.get_route();

        for(int i = 0; i < controllers.size(); ++i) {
            std::cout << "IS IT " << controllers[i].prefix << "?\n";
            if(starts_with(route, controllers[i].prefix)) {
                return controllers[i].controller(req, storage);
            }
        }

        return Response(404, {}, "");
    }
};
}