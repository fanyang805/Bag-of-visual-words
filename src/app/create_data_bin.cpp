#include "convert_dataset.hpp"
#include "utils.hpp"

#include <iostream>

int main() {
  const std::string &img_path{"data/images/"};
  const std::string &extension{".png"};
  std::vector<std::string> img_sequence =
      bovw::get_sequence(img_path, extension, 0, 691);
  std::cout << "Start to create binary files.\n";
  bovw::serialization::sifts::ConvertDataset(img_path, img_sequence, 700);

  return 0;
}