if (!file1.is_open() || !file2.is_open() || !outputFile.is_open()) {
        std::cout << "Failed to open the files." << std::endl;
        return;
    }