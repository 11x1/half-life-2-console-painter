#ifndef LOADER_HH
#define LOADER_HH
#include <string>


class loader {
private:
    std::string m_last_err { };
public:
    loader( ) = default;
    ~loader( ) = default;

    bool inject( char* dll_path, const char* target_process_name );

    std::string& get_last_err( ) {
        return m_last_err;
    }
};

inline loader g_loader;


#endif //LOADER_HH
