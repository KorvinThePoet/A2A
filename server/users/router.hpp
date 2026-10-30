#pragma once

#include "../requests/requests.hpp"
#include "../tools/strtools.hpp"
#include "../requests/response.hpp"
#include "controllers.hpp"

namespace Users {
Response route(const Request& req, Storage& storage);
}