#include "histogram.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <limits>
#include <sstream>
#include <vector>

#include "bowdictionary.hpp"
#include "utils.hpp"

using namespace cv;

namespace bovw {

Histogram::Histogram(cv::Mat descriptors, const BowDictionary &dictionary) {
  Mat vocabularies = dictionary.vocabulary();
  if (descriptors.empty() || dictionary.empty()) {
    std::cerr << "The inputs of histogram are empty!\n";
    return;
  }
  data_ = std::vector<int>(vocabularies.rows);
  for (int des_id = 0; des_id < descriptors.rows; ++des_id) {
    float min_distance = std::numeric_limits<float>::max();
    int min_voc_id = -1;
    for (int voc_id = 0; voc_id < vocabularies.rows; ++voc_id) {
      float distance =
          euclidean_dis_sq(descriptors.row(des_id), vocabularies.row(voc_id));
      if (min_distance > distance) {
        min_distance = distance;
        min_voc_id = voc_id;
      }
    }
    ++data_[min_voc_id];
  }
}

Histogram::Histogram(const cv::Mat_<int> &bestLabel, int dictionary_size) {
  data_ = std::vector<int>(dictionary_size);
  for (const auto &word_idx : bestLabel) {
    ++data_[word_idx];
  }
}

void Histogram::ReadFromCSV(const std::string &filename, bool reweighted) {

  std::ifstream in(filename, std::ios_base::in);
  if (!in) {
    std::cerr << "Cannot read .csv file: " << filename << std::endl;
    return;
  }
  std::string line;
  std::getline(in, line);
  std::stringstream ss{line};
  std::string str_counts;

  if (!reweighted) {
    std::vector<int> data;
    while (std::getline(ss, str_counts, ',')) {
      int counts = std::stoi(str_counts);
      data.emplace_back(counts);
    }
    data_ = data;
  } else {
    std::vector<float> reweighted_data;
    while (std::getline(ss, str_counts, ',')) {
      int counts = std::stof(str_counts);
      reweighted_data.emplace_back(counts);
    }
    reweighted_data_ = reweighted_data;
  }

  return;
}

void Histogram::WriteToCSV(const std::string &filename, bool reweighted) const {
  namespace fs = std::filesystem;
  fs::create_directories(fs::path(filename).remove_filename());
  std::ofstream out(filename, std::ios::app);
  size_t counts = 1;
  if (!reweighted) {
    for (const int &voc : data_) {
      out << voc;
      if (counts++ < data_.size()) {
        out << ",";
      }
    }
  } else {
    for (const float &voc : reweighted_data_) {
      out << voc;
      if (counts++ < reweighted_data_.size()) {
        out << ",";
      }
    }
  }

  out << std::endl;
}

std::ostream &operator<<(std::ostream &os, const Histogram &hist_obj) {
  size_t counts = 1;
  for (const auto &voc : hist_obj.data_) {
    os << voc;
    if (counts++ < hist_obj.data_.size()) {
      os << ",";
    }
  }
  return os;
}

// std::ostream &operator<<(std::ostream &os,
//                          const std::vector<float> &reweighted_data) {
//   for (auto voc_it = reweighted_data.cbegin(); voc_it <
//   reweighted_data.cend();
//        ++voc_it) {
//     os << *voc_it;
//     if (voc_it < reweighted_data.cend() - 1) {
//       os << ", ";
//     }
//   }
//   return os;
// }

void Histogram::reweight_hist(const BowDictionary &dictionary) {
  if (img_num_ == 0) {
    std::cerr << "The number of images in the dataset is 0!\n";
    return;
  }
  // Compute the number of word occurrences
  float n_d = 0;
  for (const auto &num : data_) {
    n_d += num;
  }
  // Compute reweighted bin
  std::vector<float> reweighted_data;
  reweighted_data.reserve(data_.size());
  // number of images that contain the word
  const std::vector<int> &words_occurrence = dictionary.words_occurrence();
  if (words_occurrence.size() != data_.size()) {
    std::cerr << "The number of words in the dictionary and the in the "
                 "histogram are not equal!\n";
    return;
  }
  auto word_it = words_occurrence.cbegin();
  for (const auto &n_id : data_) {
    // number of images in the data set
    float N = img_num_;
    float n_i = *word_it;
    float t_id = (float(n_id) / n_d) * std::log(N / n_i);
    reweighted_data.emplace_back(t_id);
    // std::cout << "N = " << N << " n_i = " << n_i << " t_id = " << t_id
    //           << " n_id = " << n_id << " n_d = " << n_d << std::endl;
    ++word_it;
  }
  reweighted_data_ = reweighted_data;
}

} // namespace bovw