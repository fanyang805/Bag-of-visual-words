#include <iostream>
#include <string>
#include <vector>

#include "bowdictionary.hpp"
#include "convert_dataset.hpp"
#include "histogram.hpp"
#include "serialize.hpp"
#include "utils.hpp"

using namespace bovw;
using namespace cv;

int main() {

  // Compute histograms
  std::vector<bovw::Histogram> vec_hist_from_csv =
      bovw::gen_vechist_from_csv("Histogram.csv");
  std::cout << vec_hist_from_csv.size() << std::endl;
  for (auto &hist : vec_hist_from_csv) {
    // std::cout << "empty: " << std::boolalpha << hist.empty() << std::endl;
    std::cout << hist << std::endl;
  }

  // Build dictionary
  const std::string &bin_path{"data/bin/"};
  const std::string &bin_extension{".bin"};
  std::vector<std::string> bin_sequence =
      bovw::get_sequence(bin_path, bin_extension, 0, 9);
  std::cout << "Start to read binary files.\n";
  std::vector<cv::Mat> descriptors =
      bovw::serialization::sifts::LoadDataset("data/bin/", bin_sequence, 12);
  std::cout << "descriptors vector size: " << descriptors.size() << std::endl;
  std::cout << "Start to create dictionary.\n";
  auto &dictionary = bovw::BowDictionary::GetInstance();
  dictionary.build(100, 100, descriptors);
  dictionary.set_words_ocurrence(vec_hist_from_csv);
  for (const auto &word_num : dictionary.words_occurrence()) {
    std::cout << word_num << ", ";
  }
  std::cout << std::endl;

  const std::string filename_reweighted{"Reweighted_histogram.csv"};
  for (auto &hist : vec_hist_from_csv) {
    // auto &hist = vec_hist_from_csv[3];

    hist.set_img_num(10);
    hist.reweight_hist(dictionary);
    std::vector<float> data = hist.reweighted_data();
    std::cout << data << std::endl;
    hist.WriteToCSV(filename_reweighted, true);
  }

  // Compute consine distance between the first picture and the others
  const std::vector<float> &vec_img0 = vec_hist_from_csv[0].reweighted_data();
  Mat img0 = Mat(vec_img0);
  std::cout << "Start to compare images by cosine distance: \n";
  int img_idx = 0;
  for (auto it = vec_hist_from_csv.cbegin(); it < vec_hist_from_csv.cend();
       ++it) {
    const std::vector<float> &vec_img = it->reweighted_data();
    Mat img = Mat(vec_img);
    std::cout << "Mat type is " << img.type() << std::endl;
    std::cout << "Distance to image " << img_idx++ << ": "
              << cosine_dis(img0, img) << std::endl;
  }

  return 0;
} // namespace )
