#pragma once

#include "../requests/requests.hpp"
#include "../tools/strtools.hpp"
#include "../requests/response.hpp"
#include "controllers.hpp"

namespace Chats {
Requests::Response route(const Requests::Request& req, Storage& storage);
}
