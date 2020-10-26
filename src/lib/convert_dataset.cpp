#include "convert_dataset.hpp"
#include "serialize.hpp"
#include "utils.hpp"
#include <filesystem>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <vector>

namespace fs = std::filesystem;
namespace bovw::serialization::sifts {

const std::vector<std::string>
ConvertDataset(const std::filesystem::path &img_path,
               const std::vector<std::string> &img_sequence,
               int reserve_num = 100) {
  std::vector<std::string> bin_sequence;
  bin_sequence.reserve(reserve_num);
  // The path to the binary files
  const std::string bin_path =
      img_path.parent_path().parent_path().string() + "/bin/";
  fs::create_directories(bin_path);
  // Iterate through each file in the given path, compute the sequence of
  // processed image file.
  if (img_sequence.empty()) {
    std::vector<std::string> img_sequence;
    img_sequence.reserve(reserve_num);
    for (const auto &entry : fs::directory_iterator(img_path)) {
      img_sequence.push_back(entry.path());
    }
  }

  // Compute SIFT descripter and store them as bin files
  for (const std::string &filename : img_sequence) {
    // Make sure the extension is png
    const std::string extension = fs::path(filename).extension();
    if (extension != ".png") {
      std::cerr << "The image file " << filename << " is not .png file.\n";
      continue;
    }

    // Compute the SIFT descripter
    cv::Mat descriptors = compute_sifts(filename);

    // Serialize descripter
    const std::string bin_name = fs::path(filename).stem().string() + ".bin";
    const std::string des_bin_path = bin_path + bin_name;
    bin_sequence.emplace_back(des_bin_path);
    std::cout << "Writing binary file: " << des_bin_path << std::endl;
    bovw::serialization::Serialize(descriptors, des_bin_path);
    descriptors.release();
  }
  return bin_sequence;
} // namespace bovw::serialization::sifts

std::vector<cv::Mat> LoadDataset(const std::filesystem::path &bin_path,
                                 const std::vector<std::string> &bin_sequence,
                                 int reserve_num = 100) {

  std::vector<cv::Mat> vec_des;
  // Iterate through the give directory
  if (bin_sequence.empty()) {
    std::vector<std::string> bin_sequence;
    bin_sequence.reserve(reserve_num);
    for (const auto &entry : fs::directory_iterator(bin_path)) {
      bin_sequence.push_back(entry.path());
    }
  }

  for (const std::string &filename : bin_sequence) {
    const std::string extension = fs::path(filename).extension();
    if (extension != ".bin") {
      std::cout << "The file " << filename << " is not .bin file.\n";
      continue;
    }
    std::cout << "Reading binary file: " << filename << std::endl;
    vec_des.emplace_back(bovw::serialization::Deserialize(filename));
  }

  return vec_des;
}

} // namespace bovw::serialization::sifts