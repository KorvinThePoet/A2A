#include <string>
#include <vector>

class Chat{
private:
    size_t id;
    std::string name;
    std::vector<size_t> users_ids;
    std::vector<size_t> admins_ids;
public:
    // Как реализовать автоинкремент
    Chat();
    void exclude(size_t user_id);
    void change_admin(size_t new_admin_id);

};