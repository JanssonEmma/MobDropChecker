#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void processGroup(std::vector<std::string>& lines, size_t& index)
{
    int expectedValue = 1;

    while (index < lines.size()) {
        std::string& line = lines[index];
        if (line.find('}') != std::string::npos) {
            break;
        }

        std::stringstream ss(line);
        std::string leadingWhitespace;
        int runningValue;

        // Extract leading whitespace (tab characters)
        size_t pos = line.find_first_not_of("\t ");
        if (pos != std::string::npos) {
            leadingWhitespace = line.substr(0, pos);
        }

        ss >> runningValue;

        if (!ss.fail()) {
            std::string restOfLine;
            std::getline(ss, restOfLine); // Get the rest of the line after the first number

            // Write the corrected line with the preserved leading whitespace
            lines[index] = leadingWhitespace + std::to_string(expectedValue) + restOfLine;
            expectedValue++;
        }
        index++;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "No file provided. Drag and drop a file onto this executable to process it." << std::endl;
        return 1;
    }

    std::string inputFilePath = argv[1];
    std::ifstream infile(inputFilePath, std::ios::binary);
    if (!infile.is_open()) {
        std::cerr << "Error opening file: " << inputFilePath << std::endl;
        return 1;
    }

    std::string outputFilePath = inputFilePath.substr(0, inputFilePath.find_last_of(".")) + "_corrected.txt";
    std::ofstream outfile(outputFilePath, std::ios::binary);
    if (!outfile.is_open()) {
        std::cerr << "Error creating output file: " << outputFilePath << std::endl;
        return 1;
    }

    // Read the entire file content
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }

    // Process each group
    for (size_t i = 0; i < lines.size(); i++) {
        if (lines[i].find("Group") != std::string::npos) {
            i += 2; // Skip the 'Mob' and 'Type' lines
            processGroup(lines, i);
        }
    }

    // Write the processed lines to the output file
    for (const auto& l : lines) {
        outfile.write(l.c_str(), l.size());
        outfile.write("\r\n", 2); // Maintain the same line endings as in the input file
    }

    infile.close();
    outfile.close();

    std::cout << "File processed and saved as: " << outputFilePath << std::endl;
    return 0;
}
