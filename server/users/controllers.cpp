#include "controllers.hpp"

std::string hash_pwd(const std::string& pwd) {
    return "damn_that_password's hashed:" + pwd;
}

Users::CreateUserRequest Users::UserController::parseCreateUserRequest(const std::string& body) {
    std::cout << "BODY\n";
    auto body_items = to_map(body);
    std::cout << "DONE TO MAP\n";
    Users::CreateUserRequest cur{
        body_items["full_name"], body_items["username"],
        body_items["password"], body_items["pfp_path"]
    };
    std::cout << "DONE CUR\n";
    if(cur.username == "") {
        std::cout << "HOW DO U WANT TO CREATE USER WITHOUT USERNAME\n";
        throw std::runtime_error("username is not specified");
    }
    if(cur.password == "") {
        std::cout << "HOW DO U WANT TO CREATE USER WITHOUT PASSWORD\n";
        throw std::runtime_error("password is not specified");
    }
    return cur;
}

Response Users::UserController::create_user(const std::string& body) {
    std::cout << "+\n";
    std::optional<CreateUserRequest> cur_opt;
    std::cout << "-\n";
    try {
        cur_opt.emplace(parseCreateUserRequest(body));
    } catch(const std::exception& e) {
        std::cout << "parse failed: " << e.what() << "\n";
        return Response(400, {}, "'error_message': 'bad body'");
    } catch(...) {
        std::cerr << "parse failed: unknown exception\n";
        return Response(400, {}, "'error_message': 'bad body'");
    }
    std::cout << "*\n";
    CreateUserRequest cur = *cur_opt;
    if(cur.password.size() < 5) {
        return Response(400, {}, "'error_message': 'Password is too short'");
    }
    std::cout << "1\n";
    User new_user;
    new_user.username = cur.username;
    new_user.full_name = cur.full_name;
    new_user.hashed_password = hash_pwd(cur.password);
    new_user.last_online = std::time(nullptr);
    if(cur.pfp_path.has_value()) {
        new_user.pfp_path = cur.pfp_path.value();
    }
    std::cout << "2\n";
    
    try {
        storage.insert(new_user);
    } catch(const std::system_error& e) {
        // when storage coudnt insert due to the unique constraint failure, it throws 
        // std::system_error with message somm like "UNIQUE CONTSTRAINT"
        // TODO add some checks to make sure it unique constraint
        return Response(404, {}, "'error_message': 'User already exists'");
    }
    std::cout << "3\n";
    return Response(200, {}, "'msg': 'success'");
}

Response Users::UserController::get_user(const std::string &route) {

    // TODO make methods to return responses in json
    // prbly for responseuser, e.g. Response(200, {}, UserResponse(user).to_json())
    // type shi
    // fff123r
    std::string id_str = "";
    for(int i = route.find("get_user/") + std::string("get_user/").length(); i < route.length(); ++i) {
        if(route[i] == '/') break;
        id_str += route[i];
    }
    size_t id = string_to_size_t(id_str);
    try {
        auto user = storage.get<User>(id);
        return Response(200, {}, "");
    } catch(const std::system_error& e) {
        // Same shi like in create_user
        return Response(404, {}, "");
    }
    return Response(404, {}, "");
}
