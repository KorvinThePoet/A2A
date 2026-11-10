#include "router.hpp"

// GETTING ROUTE LIKE
// /users/.../.../

// ROUTES might be
// /users/create_user/
// /users/get_user/{user_id: int}/

Response Users::route(const Request& req, Storage &storage) {
    std::string local_route = req.get_route();
    std::vector<std::string> prf_n_psf = divide_in_two(local_route, 1, '/');

    std::string prefix = prf_n_psf[0];
    std::string postfix = prf_n_psf[01];

    if (prefix != "users") {
        std::cout << "DEBUG\n";
        std::cout << prefix << "!=users\n";
        return MessageResponse(404, "Wrong route method");
    }
    UserController uctrl(storage);
    if(starts_with(postfix, "create_user") && req.get_method() == "POST") {
        return uctrl.create_user(req.get_body());
    }
    if(starts_with(postfix, "get_user") && req.get_method() == "GET") {
        return uctrl.get_user(postfix);
    }
    if(starts_with(postfix, "get_personal_info") && req.get_method() == "GET") {
        return uctrl.get_personal_info(req.get_body());
    }
    if(starts_with(postfix, "get_all") && req.get_method() == "GET") {
        return uctrl.get_all_users();
    }
    if(starts_with(postfix, "login") && req.get_method() == "POST") {
        return uctrl.login(req.get_body());
    }
    return MessageResponse(404, "unknown route");
}
