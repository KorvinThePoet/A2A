#pragma once

#include "../requests/requests.hpp"
#include "../tools/strtools.hpp"
#include "../requests/response.hpp"
#include "controllers.hpp"

using namespace Requests;

namespace Users {
Response route(const Request& req, Storage& storage);
}