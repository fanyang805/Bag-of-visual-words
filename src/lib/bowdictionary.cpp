#include "bowdictionary.hpp"
#include "kMeans.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <opencv2/core.hpp>
#include <string>

using namespace cv;

namespace bovw {

void BowDictionary::build(const int &max_iterations, const int &dict_size,
                          const std::vector<cv::Mat> &descriptors) {
  Mat mat_data = Vec_MatToMat(descriptors);
  int total_features = mat_data.rows;

  // Check if the new dictionary can be built
  if (descriptors.size() == 0) {
    std::cerr << "The descriptors are empty!\n";
    return;
  }

  if (max_iterations <= 0) {
    std::cerr << "The number of iteration should be a positive integer!\n";
    return;
  }

  if (dict_size > total_features) {
    std::cerr << "The size of dictionary" << dict_size
              << " cannot be larger than the "
                 "descriptors "
              << (int)descriptors.size() << "!\n";
    return;
  }

  auto [vocabulary, bestLabels] = kMeans(mat_data, dict_size, max_iterations);
  vocabulary_ = vocabulary.clone();
  bestLabels_ = bestLabels.clone();
}

void BowDictionary::set_words_ocurrence(
    const std::vector<Histogram> &vec_hist) {
  if (vec_hist.size() == 0) {
    std::cerr << "The input vector of histograms is empty!\n";
    return;
  }
  if (empty()) {
    std::cerr << "The dictionary is empty!\n";
    return;
  }
  std::vector<int> words_occurrence(size());
  for (const auto &hist : vec_hist) {
    int word_idx = 0;
    for (const auto &word_num : hist.data()) {
      if (word_num > 0) {
        ++words_occurrence[word_idx];
      }
      ++word_idx;
    }
  }

  words_occurrence_ = words_occurrence;
}

void BowDictionary::serialize_dic(const std::string &filename) const {
  if (empty()) {
    std::cerr << "The dictionary is empty!\n";
    return;
  }
  namespace fs = std::filesystem;
  fs::create_directories(fs::path(filename).remove_filename());
  std::ofstream write_bin(filename, std::ios_base::out | std::ios_base::binary);
  // write vocabulary_ into binary
  uchar *ptmat = vocabulary_.data;
  int row_num = vocabulary_.rows;
  int col_num = vocabulary_.cols;
  int type_int = vocabulary_.type();
  write_bin.write(reinterpret_cast<char *>(&row_num), sizeof(int));
  write_bin.write(reinterpret_cast<char *>(&col_num), sizeof(int));
  write_bin.write(reinterpret_cast<char *>(&type_int), sizeof(int));
  write_bin.write(reinterpret_cast<char *>(ptmat),
                  vocabulary_.total() * vocabulary_.elemSize());
  // write words_occurrence_ into binary
  int vec_size = words_occurrence_.size();
  auto *ptvec = &words_occurrence_.front();
  write_bin.write(reinterpret_cast<char *>(&vec_size), sizeof(int));
  write_bin.write(reinterpret_cast<const char *>(ptvec),
                  words_occurrence_.size() * sizeof(words_occurrence_.front()));
  // write bestLabels_ into binary
  uchar *ptmat_b = bestLabels_.data;
  int row_num_b = bestLabels_.rows;
  int col_num_b = bestLabels_.cols;
  int type_int_b = bestLabels_.type();
  write_bin.write(reinterpret_cast<char *>(&row_num_b), sizeof(int));
  write_bin.write(reinterpret_cast<char *>(&col_num_b), sizeof(int));
  write_bin.write(reinterpret_cast<char *>(&type_int_b), sizeof(int));
  write_bin.write(reinterpret_cast<char *>(ptmat_b),
                  bestLabels_.total() * bestLabels_.elemSize());
}

void BowDictionary::deserialize_dic(const std::string &filename) {
  std::ifstream read_bin(filename, std::ios_base::in | std::ios_base::binary);
  if (!read_bin) {
    std::cerr << "The file " << filename << " does not exist!\n";
    return;
  }
  // read vocabulary_ from binary
  int row_num, col_num, type_int;
  read_bin.read(reinterpret_cast<char *>(&row_num), sizeof(int));
  read_bin.read(reinterpret_cast<char *>(&col_num), sizeof(int));
  read_bin.read(reinterpret_cast<char *>(&type_int), sizeof(int));
  cv::Mat vocabulary = cv::Mat::zeros(row_num, col_num, type_int);
  read_bin.read(reinterpret_cast<char *>(vocabulary.data),
                vocabulary.total() * vocabulary.elemSize());
  vocabulary_ = vocabulary.clone();
  // read words_occurrence_ from binary
  int vec_size;
  read_bin.read(reinterpret_cast<char *>(&vec_size), sizeof(int));
  std::vector<int> words_occurrence(vec_size, 0);
  read_bin.read(reinterpret_cast<char *>(&words_occurrence.front()),
                words_occurrence.size() * sizeof(words_occurrence.front()));
  words_occurrence_ = words_occurrence;
  // read bestLabels_ from binary
  int row_num_b, col_num_b, type_int_b;
  read_bin.read(reinterpret_cast<char *>(&row_num_b), sizeof(int));
  read_bin.read(reinterpret_cast<char *>(&col_num_b), sizeof(int));
  read_bin.read(reinterpret_cast<char *>(&type_int_b), sizeof(int));
  cv::Mat bestLabels = cv::Mat::zeros(row_num_b, col_num_b, type_int_b);
  read_bin.read(reinterpret_cast<char *>(bestLabels.data),
                bestLabels.total() * bestLabels.elemSize());
  bestLabels_ = bestLabels.clone();
}

} // namespace bovw