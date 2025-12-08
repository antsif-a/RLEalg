#include <print>
#include <fstream>
#include <string_view>

using namespace std;

constexpr char ESCAPE = '#';

void encode(ifstream & input, ofstream & output) {
    char current, next;
    uint8_t counter = 1;

    if (!input.read(&current, 1))
        return;

    while (input.read(&next, 1)) {
        if (next != current) {
            if (counter >= 4 || current == ESCAPE) {
                output << ESCAPE << static_cast<char>(counter) << current;
            } else {
                output << string(counter, current);
            }

            counter = 1;
            current = next;
        } else {
            counter++;
        }

        if (counter == 255) {
            output << ESCAPE << static_cast<char>(counter) << current;
            counter = 0;
        }
    }

    if (counter >= 4) {
        output << ESCAPE << static_cast<char>(counter) << current;
    } else {
        output << string(counter, current);
    }
}

void decode(ifstream & input, ofstream & output) {
    char current, symbol;
    uint8_t count;
    
    while (input.read(&current, 1)) {
        if (current == ESCAPE) {
            input.read(reinterpret_cast<char *>(&count), 1);
            input.read(&symbol, 1);
            output << string(count, symbol);
        } else {
            output << current;
        }
    }
}

int main(int argc, char * argv[]) {
    if (argc < 4) {
        println(stderr, "Usage: {} code|decode <input> <output>", argv[0]);
        return 1;
    }
    ifstream input(argv[2], ios::binary | ios::ate);
    if (!input.is_open()) {
        println(stderr, "Could not open input file {}", argv[2]);
        return 1;
    }
    int input_size = input.tellg();
    input.seekg(0);
    ofstream output(argv[3], ios::binary);
    if (!output.is_open()) {
        println(stderr, "Could not open output file {}", argv[2]);
        return 1;
    }

    string_view command_name(argv[1]);
    if (command_name == "code") {
        encode(input, output);
        output.flush();
        output.seekp(0, ios::end);
        int output_size = output.tellp();
        println("Коэффициент сжатия: {}", input_size / output_size);
    } else if (command_name == "decode") {
        decode(input, output);
    } else {
        println(stderr, "Usage: {} code|decode <input> <output>", argv[0]);
        return 1;
    }

    return 0;
}
