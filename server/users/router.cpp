#include "router.hpp"

// GETTING ROUTE LIKE
// /users/.../.../

// ROUTES might be
// /users/create_user/
// /users/get_user/{user_id: int}/

Response Users::route(const Request& req, Storage &storage) {
    std::string local_route = req.get_route();
    std::vector<std::string> prf_n_psf = devide_in_two(local_route, 1, '/');

    std::string prefix = prf_n_psf[0];
    std::string postfix = prf_n_psf[01];

    if (prefix != "users") {
        std::cout << "DEBUG\n";
        std::cout << prefix << "!=users\n";
        return Response(404, {}, "");
    }
    UserController uctrl(storage);
    if(starts_with(postfix, "create_user") && req.get_method() == "POST") {
        return uctrl.create_user(req.get_body());
    }
    if(starts_with(postfix, "get_user") && req.get_method() == "GET") {
        return uctrl.get_user(postfix);
    }
    return Response(404, {}, "");
}
