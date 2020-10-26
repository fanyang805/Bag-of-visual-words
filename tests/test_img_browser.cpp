#include "image_browser.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

int rand_num(int min, int max) {
  std::srand(static_cast<unsigned int>(std::time(nullptr)));
  static constexpr double fraction{1.0 / (RAND_MAX + 1.0)};
  return min + (max - min + 1) * (std::rand() * fraction);
}
using namespace bovw;

int main() {
  std::string title = "Fan's Image Browser";
  std::string stylesheet = "../include/style.css";
  std::vector<image_browser::ImageRow> rows;
  std::vector<std::string> vec_img_names =
      get_sequence("data/images/", ".png", 0, 9);
  int idx = 0;
  for (int row_num = 0; row_num < 3; ++row_num) {
    std::vector<image_browser::ScoredImage> img_row_vec;
    for (int col_num = 0; col_num < 3; ++col_num) {
      std::string img_path = vec_img_names[idx++];
      double score = rand_num(0, 100) * 0.01;
      img_row_vec.push_back(image_browser::ScoredImage{img_path, score});
    }
    // image_browser::ImageRow img_row = {img_row_vec[0], img_row_vec[1],
    //                                    img_row_vec[2]};
    rows.push_back(img_row_vec);
  }
  std::vector<image_browser::ScoredImage> img_row_last;
  const std::string img_path_last = vec_img_names.back();
  double score_last = rand_num(0, 100) * 0.01;
  img_row_last.push_back(image_browser::ScoredImage{img_path_last, score_last});
  image_browser::ImageRow img_row(3);
  img_row[0] = {img_row_last[0]};
  rows.push_back(img_row);

  std::string html_file{"show_results.html"};
  std::ofstream ofs(html_file, std::ios_base::app);
  image_browser::CreateImageBrowser(title, stylesheet, rows, ofs);
  return 0;
}
