#include <print>
#include <fstream>
#include <string_view>

using namespace std;

constexpr char ESCAPE = '#';

void encode(ifstream & input, ofstream & output) {
    char current, next;
    uint8_t counter = 1;

    if (!input.read(&current, 1)) {
        return;
    }

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
    char curr;
    uint8_t count;
    char symbol;
    while (input.read(&curr, 1)) {
        if (curr == ESCAPE) {
            input.read(reinterpret_cast<char *>(&count), 1);
            input.read(&symbol, 1);
            output << string(count, symbol);
        } else {
            output << curr;
        }
    }
}

int main(int argc, char * argv[]) {
    if (argc < 4) {
        println(stderr, "Usage: {} code|decode <input> <output>", argv[0]);
        return 1;
    }
    ifstream input(argv[2], ios::binary);
    if (!input.is_open()) {
        println(stderr, "Could not open input file {}", argv[2]);
        return 1;
    }
    ofstream output(argv[3], ios::binary);
    if (!output.is_open()) {
        println(stderr, "Could not open output file {}", argv[2]);
        return 1;
    }

    string_view command_name(argv[1]);
    if (command_name == "code") {
        encode(input, output);
    } else if (command_name == "decode") {
        decode(input, output);
    } else {
        println(stderr, "Usage: {} code|decode <input> <output>", argv[0]);
        return 1;
    }

    return 0;
}
