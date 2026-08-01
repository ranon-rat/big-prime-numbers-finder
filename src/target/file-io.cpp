#include <fstream>
#include <iostream>
int main() {
  // i should be saving 1
  // the batch size
  // 1. Open with both read and write flags in binary mode
  std::fstream file("./pi.txt",
                    std::ios::in | std::ios::out | std::ios::binary);
  // first of all
  if (!file) {
    std::cerr << "Error opening file!" << std::endl;
    return 1;
  }

  // 2. Move the put (write) pointer to the 10th byte from the beginning
  std::streampos position = 10;
  file.seekp(position, std::ios::beg);

  // 3. Overwrite the data at that position
  // i have to say the size of the data?
  // what if i want to include more data?
  file.write("NEW_DATA", 8);

  file.close();
  std::cout << "hello world\n";
}