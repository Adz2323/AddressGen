#include <bitcoin/system.hpp>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: ./gen -n <number_of_keys>\n";
        return 1;
    }

    int num_keys = std::stoi(argv[2]);
    bc::data_chunk secret(32, 0);
    secret[0] = 0x20; // Starting with 130-bit value 0x200000000000000000000000000000000

    std::ofstream txtFile("keys.txt");
    std::ofstream binFile("publickeys.bin", std::ios::binary);

 for (int i = 0; i < num_keys; ++i) {
    bc::ec_secret ec_secret;
    std::copy_n(secret.begin(), secret.size(), ec_secret.begin());

    bc::wallet::ec_private privateKey(ec_secret, bc::wallet::ec_private::mainnet_p2kh);
    bc::wallet::ec_public publicKey(privateKey.to_public());

    // Convert private key to hexadecimal and write to file
    std::string hex_private_key = bc::encode_base16(privateKey.secret());
    txtFile << hex_private_key << " " << publicKey.encoded() << "\n";

    bc::data_chunk compressed_pubkey;
    if (publicKey.to_data(compressed_pubkey)) {
        binFile.write(reinterpret_cast<const char*>(compressed_pubkey.data()), compressed_pubkey.size());
    } else {
        std::cerr << "Error converting public key to data.\n";
    }

    // Increment secret
    for (int j = 31; j >= 0; --j) {
        if (++secret[j]) break;
    }
}

    txtFile.close();
    binFile.close();

    return 0;
}
