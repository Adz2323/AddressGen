#include <bitcoin/system.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace mp = boost::multiprecision;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: ./gen -n <number_of_keys>\n";
        return 1;
    }

    int num_keys = std::stoi(argv[2]);
    std::ofstream txtFile("keys.txt");
    std::ofstream pubkeyFile("publickeys.txt");

    mp::uint256_t min_hex("0x0000000000000000000000000000000200000000000000000000000000000000");
    mp::uint256_t max_hex("0x00000000000000000000000000000003FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    mp::uint256_t range = max_hex - min_hex + 1;
    mp::uint256_t step_size = range / num_keys;

for (int i = 0; i < num_keys; ++i) {
    mp::uint256_t hex_private_key_val = min_hex + step_size * i;

    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(64) << hex_private_key_val;
    std::string hex_private_key = ss.str();

    bc::data_chunk private_key_data;
    if (!bc::decode_base16(private_key_data, hex_private_key)) {
        std::cerr << "Error: Invalid hexadecimal private key.\n";
        continue;
    }

    if (private_key_data.size() != 32) {
        std::cerr << "Error: Invalid private key size.\n";
        continue;
    }

    bc::ec_secret ec_secret;
    std::copy(private_key_data.begin(), private_key_data.end(), ec_secret.begin());

    bc::wallet::ec_private privateKey(ec_secret, bc::wallet::ec_private::mainnet_p2kh);
    bc::wallet::ec_public publicKey(privateKey.to_public());

    txtFile << hex_private_key << " " << publicKey.encoded() << "\n";

    bc::data_chunk compressed_pubkey;
    if (publicKey.to_data(compressed_pubkey)) {
        std::string hex_pubkey = bc::encode_base16(compressed_pubkey);
        pubkeyFile << hex_pubkey << "\n";
    } else {
        std::cerr << "Error converting public key to data.\n";
    }
}


    txtFile.close();
    pubkeyFile.close();

    return 0;
}
