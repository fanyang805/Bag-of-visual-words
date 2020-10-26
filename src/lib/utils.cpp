#include "utils.hpp"
#include "histogram.hpp"
#include "html_writer.hpp"
#include "image_browser.hpp"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <vector>

using namespace cv;

namespace bovw {

const cv::Mat compute_sifts(const std::string &filename) {
  namespace fs = std::filesystem;
  if (!fs::exists(filename)) {
    std::cerr << "The file " << filename << " does not exist!\n";
    return cv::Mat();
  }
  const cv::Mat img_mat = cv::imread(filename, cv::IMREAD_GRAYSCALE);
  auto detector = cv::SiftFeatureDetector::create();
  std::vector<cv::KeyPoint> vec_keypoint;
  detector->detect(img_mat, vec_keypoint);

  cv::Mat descriptors;
  auto extractor = cv::SiftDescriptorExtractor::create();
  extractor->compute(img_mat, vec_keypoint, descriptors);

  return descriptors;
}

const std::string gen_img_name(const std::string &img_path,
                               const std::string &extension, int img_id) {
  const std::string &prefix = "imageCompressedCam0_";
  auto to_format = [](const int number) {
    std::stringstream ss;
    ss << std::setw(7) << std::setfill('0') << number;
    return ss.str();
  };
  const std::string &img_id_formatted = to_format(img_id * 10);
  const std::string &filename =
      img_path + prefix + img_id_formatted + extension;
  return filename;
}

const std::vector<std::string> gen_img_name(const std::string &img_path,
                                            const std::string &extension,
                                            const std::vector<int> &img_ids) {
  const std::string &prefix = "imageCompressedCam0_";
  auto to_format = [](const int number) {
    std::stringstream ss;
    ss << std::setw(7) << std::setfill('0') << number;
    return ss.str();
  };
  std::vector<std::string> img_names;
  img_names.reserve(img_ids.size());
  for (const auto &img_id : img_ids) {
    const std::string &img_id_formatted = to_format(img_id * 10);
    const std::string &filename =
        img_path + prefix + img_id_formatted + extension;
    img_names.emplace_back(filename);
  }
  return img_names;
}

const std::vector<std::string> get_sequence(const std::string &img_path,
                                            const std::string &extension,
                                            int min_num, int max_num) {
  const std::string &prefix = "imageCompressedCam0_";
  std::vector<std::string> img_sequence;
  img_sequence.reserve(max_num - min_num + 1);

  auto to_format = [](const int number) {
    std::stringstream ss;
    ss << std::setw(7) << std::setfill('0') << number;
    return ss.str();
  };

  for (int idx = min_num; idx <= max_num; ++idx) {
    const std::string &img_id = to_format(idx * 10);
    const std::string &filename = img_path + prefix + img_id + extension;
    img_sequence.emplace_back(filename);
  }

  return img_sequence;
}

float cosine_dis(const Mat &mat1, const Mat &mat2) {

  float dot_product = mat1.dot(mat2);
  float sq_norm1 = mat1.dot(mat1);
  float sq_norm2 = mat2.dot(mat2);
  float cosine_simiarity = dot_product / std::sqrt(sq_norm1 * sq_norm2);
  return 1 - cosine_simiarity;
}

float cosine_dis(const std::vector<float> &vec1,
                 const std::vector<float> &vec2) {

  Mat mat1 = Mat(vec1, CV_32FC1);
  Mat mat2 = Mat(vec2, CV_32FC1);
  float dot_product = mat1.dot(mat2);
  float sq_norm1 = mat1.dot(mat1);
  float sq_norm2 = mat2.dot(mat2);
  float cosine_simiarity = dot_product / std::sqrt(sq_norm1 * sq_norm2);
  return 1 - cosine_simiarity;
}

float euclidean_dis_sq(const Mat &mat1, const Mat &mat2) {

  Mat mat_sub = mat1 - mat2;
  return mat_sub.dot(mat_sub);
}

float euclidean_dis_sq(const std::vector<float> &vec1,
                       const std::vector<float> &vec2) {
  Mat mat1 = Mat(vec1, CV_32FC1);
  Mat mat2 = Mat(vec2, CV_32FC1);
  Mat mat_sub = mat1 - mat2;
  return mat_sub.dot(mat_sub);
}

Mat Vec_MatToMat(const std::vector<Mat> &vec_mat) {
  std::cout << "Converting vector of Mat to Mat: ";
  int num_row = 0;
  int num_col = vec_mat[0].cols;

  for (const auto &mat_ele : vec_mat) {
    num_row += mat_ele.rows;
  }

  Mat mat = Mat::zeros(num_row, num_col, vec_mat[0].type());
  int row_id = 0;
  for (int mt_id = 0; mt_id < (int)vec_mat.size(); ++mt_id) {
    int mat_rows = vec_mat[mt_id].rows;
    (vec_mat[mt_id]).copyTo(mat.rowRange(row_id, row_id + mat_rows));
    row_id += mat_rows;
  }
  std::cout << "completed.\n";
  return mat;
}

std::vector<Mat> gen_blabels(const cv::Mat &bestLabels,
                             const std::vector<cv::Mat> &descriptors) {
  size_t num_img = descriptors.size();
  std::vector<Mat> blabels_for_each;
  blabels_for_each.reserve(num_img);
  int start_idx = 0;
  int end_idx = 0;
  for (const auto &img_descriptor : descriptors) {
    start_idx = end_idx;
    end_idx = start_idx + img_descriptor.rows;
    blabels_for_each.emplace_back(bestLabels.rowRange(start_idx, end_idx));
  }
  return blabels_for_each;
}

std::vector<bovw::Histogram> gen_vechist_from_csv(const std::string &filename,
                                                  bool reweight) {

  std::vector<bovw::Histogram> vec_hist;
  std::ifstream in(filename, std::ios_base::in);
  if (!in) {
    std::cerr << "Cannot read .csv file: " << filename << std::endl;
    return std::vector<bovw::Histogram>();
  }

  std::string line;
  while (std::getline(in, line) && !line.empty()) {
    // std::cout << line << std::endl;
    std::stringstream ss{line};
    std::string str_counts;
    if (!reweight) {
      std::vector<int> data;
      while (std::getline(ss, str_counts, ',')) {
        int counts = std::stoi(str_counts);
        data.emplace_back(counts);
        // std::cout << counts << " ";
      }
      vec_hist.emplace_back(Histogram(data));
    } else {
      std::vector<float> data;
      while (std::getline(ss, str_counts, ',')) {
        float counts = std::stof(str_counts);
        data.emplace_back(counts);
        // std::cout << counts << " ";
      }
      vec_hist.emplace_back(Histogram({}, data, 0));
    }
  }

  for (auto &hist : vec_hist) {
    hist.set_img_num(vec_hist.size());
  }

  return vec_hist;
}

void show_results(const Histogram &query_img,
                  const std::vector<Histogram> &vec_hist, int results_num,
                  const std::string html_name) {
  namespace fs = std::filesystem;
  // compute distances
  // int is the index
  std::vector<std::pair<int, float>>
      distances; // The reason not using map<float, int> is that it might be two
                 // keys with same value, e.g., 0, 0.
  distances.reserve(vec_hist.size());
  int idx = 0;
  for (const auto &img_hist : vec_hist) {
    float distance =
        cosine_dis(query_img.reweighted_data(), img_hist.reweighted_data());
    distances.emplace_back(std::make_pair(idx++, distance));
    // std::cout << distance << std::endl;
  }
  std::sort(distances.begin(), distances.end(),
            [](const std::pair<int, float> &left,
               const std::pair<int, float> &right) {
              return left.second < right.second;
            });
  // for (const auto &ele : distances) {
  //   std::cout << ele.second << " ";
  // }
  // std::cout << std::endl;
  // Compute the vector of image names and vector of cosine distances
  std::string title = "Fan's Image Browser";
  std::string stylesheet = "include/style.css";
  std::vector<std::string> img_names;
  std::vector<float> img_distances;
  img_distances.reserve(results_num);
  img_names.reserve(results_num);
  for (auto it = distances.begin(); it < distances.begin() + results_num;
       ++it) {
    img_names.emplace_back(gen_img_name("data/images/", ".png", it->first));
    img_distances.emplace_back(it->second);
  }
  // Compute the rows of html
  auto it_name = img_names.cbegin();
  auto it_distance = img_distances.cbegin();
  int row_num = results_num / 3;
  int last_row = results_num % 3;
  std::vector<image_browser::ImageRow> rows;
  for (int row_idx = 0; row_idx < row_num; ++row_idx) {
    image_browser::ImageRow img_row;
    for (int col_idx = 0; col_idx < 3; ++col_idx) {
      std::string img_path = *it_name++;
      float score = *it_distance++;
      img_row.push_back(
          image_browser::ScoredImage{img_path, 100 * (1 - score)});
    }

    rows.push_back(img_row);
  }
  if (last_row != 0) {
    image_browser::ImageRow img_row(3);
    for (int col_idx = 0; col_idx < last_row; ++col_idx) {
      std::string img_path = *it_name++;
      float score = *it_distance++;
      img_row[col_idx] =
          image_browser::ScoredImage{img_path, 100 * (1 - score)};
    }
    rows.push_back(img_row);
  }
  // create html
  std::string html_file{html_name};
  // fs::create_directories(fs::path(html_name).remove_filename());
  std::ofstream ofs(html_file, std::ios_base::app);
  image_browser::CreateImageBrowser(title, stylesheet, rows, ofs);
}

} // namespace bovw