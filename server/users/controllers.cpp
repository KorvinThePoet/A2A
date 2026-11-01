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
    std::optional<CreateUserRequest> cur_opt;
    try {
        cur_opt.emplace(parseCreateUserRequest(body));
    } catch(const std::exception& e) {
        std::cout << "parse failed: " << e.what() << "\n";
        return Response(400, {}, "'error_message': 'bad body'");
    } catch(...) {
        std::cerr << "parse failed: unknown exception\n";
        return Response(400, {}, "'error_message': 'bad body'");
    }
    CreateUserRequest cur = *cur_opt;
    if(cur.password.size() < 5) {
        return Response(400, {}, "'error_message': 'Password is too short'");
    }
    User new_user;
    new_user.username = tolower(cur.username);
    new_user.full_name = cur.full_name;
    new_user.hashed_password = hash_pwd(cur.password);
    new_user.last_online = std::time(nullptr);
    if(cur.pfp_path.has_value()) {
        new_user.pfp_path = cur.pfp_path.value();
    }
    
    try {
        storage.insert(new_user);
    } catch(const std::system_error& e) {
        // when storage coudnt insert due to the unique constraint failure, it throws 
        // std::system_error with message somm like "UNIQUE CONTSTRAINT"
        // TODO add some checks to make sure it unique constraint
        return Response(404, {}, "'error_message': 'User already exists'");
    }
    return Response(200, {}, "'message': 'User created'");
}

Response Users::UserController::get_user(const std::string &route) {

    std::string username = "";
    for(int i = route.find("get_user/") + std::string("get_user/").length(); i < route.length(); ++i) {
        if(route[i] == '/') break;
        username += route[i];
    }
    username = tolower(username);
    auto user = storage.get_all<User>(sqlite_orm::where(sqlite_orm::c(&User::username) == username));
    if(user.empty()) {
        return Response(404, {}, "{'message': 'user not found'}");
    }
    return Response(200, {}, UserResponse(user[0]).to_json());

}


Response Users::UserController::get_all_users() {
    auto users = storage.get_all<User>();
    if(users.empty()) {
        return Response(200, {}, "{}");
    }
    std::string response = "{'0':" + UserResponse(users[0]).to_json()+"";
    for(int i = 1; i < users.size(); ++i) {
        response += ",{'" + std::to_string(i) + "':" + UserResponse(users[i]).to_json();
    }
    response += "}";
    return Response(200, {}, response);
}
