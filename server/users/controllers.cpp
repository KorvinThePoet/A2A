#include "controllers.hpp"

Users::CreateUserRequest Users::UserController::parseCreateUserRequest(const std::string& body) {
    auto body_items = to_map(body);
    Users::CreateUserRequest cur{
        body_items["full_name"], body_items["username"], body_items["email"],
        body_items["password"], body_items["pfp_path"]
    };

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

Users::LoginUserRequest Users::UserController::parseLoginUserRequest(const std::string &body) {
    auto body_items = to_map(body);
    Users::LoginUserRequest lur{body_items["username"], body_items["password"]};
    if(lur.username.empty()) {
        std::cout << "HOW DO U WANT TO LOGIN USER WITHOUT USERNAME\n";
        throw std::runtime_error("username is not specified");
    }
    if(lur.password.empty()) {
        std::cout << "HOW DO U WANT TO LOGIN USER WITHOUT PASSWORD\n";
        throw std::runtime_error("password is not specified");
    }

    return lur;
}


Response Users::UserController::create_user(const std::string& body) {
    std::optional<CreateUserRequest> cur_opt;
    try {
        cur_opt.emplace(parseCreateUserRequest(body));
    } catch(const std::exception& e) {
        std::cout << "parse failed: " << e.what() << "\n";
        return MessageResponse(400, "bad body");
    } catch(...) {
        std::cerr << "parse failed: unknown exception\n";
        return MessageResponse(400, "bad body");
    }
    CreateUserRequest cur = *cur_opt;
    if(cur.password.size() < 5) {
        return MessageResponse(400, "Short password");
    }
    User new_user;
    new_user.username = tolower(cur.username);
    new_user.full_name = cur.full_name;
    new_user.email = cur.email;
    new_user.hashed_password = hash_password(cur.password);
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
        return MessageResponse(401, "User already exists");
    }
    return MessageResponse(200, "User created");
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
        return MessageResponse(404, "User not found");
    }
    return Response(200, {}, UserResponse(user[0]).to_json());
}


Response Users::UserController::get_personal_info(const std::string &body) {
    auto payload = to_map(body);
    std::string str_user_id = payload["user_id"];
    std::string str_token = payload["token"];
    int user_id = string_to_int(str_user_id);
    if(str_user_id.empty() || str_token.empty() || user_id == 0) {
        return MessageResponse(400, "user id or token are not specified");
    }

    auto tokens = storage.get_all<Token>(sqlite_orm::where(sqlite_orm::c(&Token::user_id) == user_id));
    if(tokens.empty()) {
        return MessageResponse(400, "You have no access");
    }
    auto token = *tokens.begin();
    
    if(token.expires < std::time(nullptr)) {
        return MessageResponse(400, "your session is over, relogin");
    }
    if(hash_token(str_token) != token.hashed_token) {
        return MessageResponse(400, "Access denied");
    }

    try {
        User user = storage.get<User>(token.user_id);
        return Response(200, {}, DetailedUserResponse(user).to_json());
    } catch(const std::runtime_error& e) {
        return MessageResponse(404, "cant find dis vanila");
    } catch(...) {
        return MessageResponse(400, "Unknow error wtf");
    }
}


Response Users::UserController::get_all_users() {
    auto users = storage.get_all<User>();
    if(users.empty()) {
        return Response(200, {}, "{}");
    }
    std::string response = "{'0':" + UserResponse(users[0]).to_json()+"";
    for(int i = 1; i < users.size(); ++i) {
        response += ",'" + std::to_string(i) + "':" + UserResponse(users[i]).to_json();
    }
    response += "}";
    return Response(200, {}, response);
}

Response Users::UserController::login(const std::string& body) {
    std::optional<LoginUserRequest> lur_opt;
    try{
        lur_opt = parseLoginUserRequest(body);
    }catch(const std::runtime_error& e) {
        return MessageResponse(400, "username or password are not specified");
    }

    LoginUserRequest lur = *lur_opt;
    auto users = storage.get_all<User>(sqlite_orm::where(sqlite_orm::c(&User::username)==tolower(lur.username)));
    if(users.empty()) {
        return MessageResponse(400, "No such user");
    }
    User user = *users.begin();
    if(user.hashed_password == hash_password(lur.password)) {
        storage.remove_all<Token>(sqlite_orm::where(sqlite_orm::c(&Token::user_id) == user.id));

        // Добавить немного времени хранения, а то 20 секунд маловато для меня жиесь
        std::string str_token = user.username + std::to_string(std::time(nullptr));
        Token token {-1, hash_token(str_token), user.id, std::time(nullptr) + 20};
        storage.insert<Token>(token);
        return Response(200, {}, "{'token':'" + str_token + "'}");
    }
    return MessageResponse(400, "password is invalid");
}
