#include <string>
#include <vector>

class File{
private:
    size_t id;
    size_t owner_id;
    std::string file_name;
    std::string file_path;
    size_t file_size;
    std::vector<uint8_t> data;

public:
    File(const std::vector<uint8_t>&);
    std::vector<uint8_t> get_data() const;
};