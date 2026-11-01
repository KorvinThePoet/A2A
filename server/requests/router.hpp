#pragma once

#include <string>
#include <map>
#include <functional>

#include "response.hpp"
#include "requests.hpp"
#include "../db/storage.hpp"
#include "../tools/strtools.hpp"

// Эхх, щас все стало чуточку проще. 17 00. 
// Имеем RouterManager, его основная обязанность рекистрировать роуты у приложений
// например у приложения users, 
// Легкость в том, что мы делигируем парсинг запроса на само приложение
// Все что делает RouterManager - просто сохраняет список функций route(const Request&, Storage&)
//   и вызывает нужну, в зависимости от prefix в самом req.get_route()
// а там уже server/users/router.cpp вызывает route и далее по кайфу выполняет задачку
// и тдтдтдтдтдтд и тд(и так далее(и тд))

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
            if(starts_with(route, controllers[i].prefix)) {
                return controllers[i].controller(req, storage);
            }
        }

        return Response(404, {}, "");
    }
};
}