#include <cstdlib>
#include <string>

class Hash
{
    Hash();
    ~Hash();
    public:
        enum Method { SHA256, DoubleSHA256, SHA512, DoubleSHA512 };
        static std::string hash(std::string data );

    private:
        static std::string hashSHA256( std::string data );
        static std::string hashDoubleSHA256( std::string data );
        static std::string hashSHA512( std::string data );
        static std::string hashDoubleSHA512( std::string data );
};
