#include "bowdictionary.hpp"
#include "image_browser.hpp"
#include "utils.hpp"

#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <string>

using namespace cv;
using namespace bovw;
int main() {
  //   namespace fs = std::filesystem;
  //   int img_idx, num_similar_img;
  //   bool initialize = true;
  //   while (initialize) {
  //     std::cout << "Please enter the index of the image (0-691): ";
  //     std::cin >> img_idx;
  //     std::string img_name = gen_img_name("data/images/", ".png", img_idx);
  //     if (!fs::exists(img_name)) {
  //       std::cerr << "The query image: " << img_name
  //                 << " does not exist! Please try again.\n";
  //       continue;
  //     }
  //     std::cout << "Please enter the number of similar images you want to
  //     find: "; std::cin >> num_similar_img; std::cout << "Your query image is
  //     " << img_name << std::endl; std::cout << "You want to find " <<
  //     num_similar_img
  //               << " images similar to the query image.\n";
  //     std::cout << "Please enter Y to continue otherwise to initialize again:
  //     "; std::string condition; std::cin >> condition; initialize = condition
  //     == "Y" ? false : true;
  //   }
  // Compute the histogram of the query image
  std::string img_name = gen_img_name("data/images/", ".png", 188);
  auto &dictionary = bovw::BowDictionary::GetInstance();
  dictionary.deserialize_dic("data/dictionary/dic_full_3_10_1000.bin");
  //   std::cout << dictionary.words_occurrence() << std::endl;
  Mat descriptors = bovw::compute_sifts(img_name);
  Histogram img_hist(descriptors, dictionary);
  img_hist.set_img_num(692);
  img_hist.reweight_hist(dictionary);
  // std::cout << "data:\n" << img_hist.data() << std::endl;
  // std::cout << "reweighted_data:\n" << img_hist.reweighted_data() <<
  // std::endl;
  // Get the vector of histograms of the data set from .csv
  std::vector<bovw::Histogram> vec_hist_from_csv = bovw::gen_vechist_from_csv(
      "data/histograms/Reweighted_histogram.csv", true);
  show_results(img_hist, vec_hist_from_csv, 9);
  return 0;
}