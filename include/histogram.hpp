#ifndef HISTOGRAM_HPP_
#define HISTOGRAM_HPP_

#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

#include "bowdictionary.hpp"

namespace bovw {

class Histogram {
  friend std::ostream &operator<<(std::ostream &os, const Histogram &hist_obj);
  // friend std::ostream &operator<<(std::ostream &os,
  //                                 const std::vector<float> &reweighted_data);

private:
  std::vector<int> data_{};
  std::vector<float> reweighted_data_{};
  int img_num_{};

public:
  // Constructors
  Histogram() = default;
  explicit Histogram(const std::vector<int> &data) : data_(data){};
  // explicit Histogram(const std::vector<int> &data, int img_num)
  //     : data_(data), img_num_(img_num){};
  explicit Histogram(const std::vector<int> &data,
                     const std::vector<float> &reweighted_data, int img_num)
      : data_(data), reweighted_data_(reweighted_data), img_num_(img_num){};
  Histogram(cv::Mat descriptors, const BowDictionary &dictionary);
  explicit Histogram(const cv::Mat_<int> &bestLabel, int dictionary_size);
  // Getter functions
  const std::vector<int> &data() const { return data_; };
  const std::vector<float> &reweighted_data() const {
    return reweighted_data_;
  };
  size_t size() const { return data_.size(); };
  bool empty() const { return data_.empty(); };
  int img_num() const { return img_num_; };
  // Setter
  void set_img_num(int img_num) { img_num_ = img_num; };
  std::vector<float> &reweighted_data() { return reweighted_data_; };
  // Operators
  int &operator[](size_t idx) { return data_[idx]; };
  const int &operator[](size_t idx) const { return data_[idx]; };
  // I/O
  void ReadFromCSV(const std::string &filename, bool reweighted = false);
  void WriteToCSV(const std::string &filename, bool reweighted = false) const;

  // Iterators
  std::vector<int>::iterator begin() { return data_.begin(); };
  std::vector<int>::const_iterator begin() const { return data_.begin(); };
  std::vector<int>::const_iterator cbegin() const { return data_.cbegin(); };
  std::vector<int>::iterator end() { return data_.end(); };
  std::vector<int>::const_iterator end() const { return data_.end(); };
  std::vector<int>::const_iterator cend() const { return data_.cend(); };
  // Member function
  void reweight_hist(const BowDictionary &dictionary);
};

} // namespace bovw

#endif