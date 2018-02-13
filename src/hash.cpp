#include "hash.h"

#include <crypto++/filters.h>
#include <crypto++/sha.h>
#include <crypto++/hex.h>

std::string Hash::hash( std::string data )
{
    return hashDoubleSHA512( data );
}

std::string Hash::hashSHA512( std::string data )
{
    CryptoPP::SHA512 hash;
    byte digest[ CryptoPP::SHA512::DIGESTSIZE ];
    hash.CalculateDigest( digest, (byte*) data.c_str(), data.length() );

    CryptoPP::HexEncoder encoder;
    std::string output;
    encoder.Attach( new CryptoPP::StringSink( output ) );
    encoder.Put( digest, sizeof(digest) );
    encoder.MessageEnd();

    return output;
}

std::string Hash::hashDoubleSHA512( std::string data )
{
    return hashSHA512( hashSHA512( data ) );
}

std::string Hash::hashSHA256( std::string data )
{
    CryptoPP::SHA256 hash;
    byte digest[ CryptoPP::SHA256::DIGESTSIZE ];
    hash.CalculateDigest( digest, (byte*) data.c_str(), data.length() );

    CryptoPP::HexEncoder encoder;
    std::string output;
    encoder.Attach( new CryptoPP::StringSink( output ) );
    encoder.Put( digest, sizeof(digest) );
    encoder.MessageEnd();

    return output;
}


std::string Hash::hashDoubleSHA256( std::string data )
{
    return hashSHA256( hashSHA256( data ) );
}
