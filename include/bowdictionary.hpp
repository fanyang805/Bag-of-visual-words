#ifndef BOWDICTIONARY_
#define BOWDICTIONARY_

#include <vector>

#include <opencv2/core.hpp>

namespace bovw {

class Histogram;

class BowDictionary {
private:
  cv::Mat vocabulary_;
  // number of images that contain word i
  std::vector<int> words_occurrence_;
  cv::Mat bestLabels_;
  BowDictionary() = default;
  ~BowDictionary() = default;
  BowDictionary(const int &max_iterations, const int &dict_size,
                const std::vector<cv::Mat> &descriptors) {
    build(max_iterations, dict_size, descriptors);
  }

public:
  BowDictionary(const BowDictionary &) = delete;
  void operator=(const BowDictionary &) = delete;
  static BowDictionary &GetInstance() {
    static BowDictionary instance;
    return instance;
  };

  // Getter funcs
  const int &size() const { return vocabulary_.rows; };
  const cv::Mat vocabulary(const int &ind) const {
    return vocabulary_.row(ind);
  };
  const cv::Mat vocabulary() const { return vocabulary_; };
  bool empty() const { return (size() == 0); }
  const cv::Mat &bestLabels() const { return bestLabels_; };
  const std::vector<int> &words_occurrence() const {
    return words_occurrence_;
  };

  // Setter funcs
  void build(const int &max_iterations, const int &dict_size,
             const std::vector<cv::Mat> &descriptors);
  void set_vocabulary(const cv::Mat &vocabulary) { vocabulary_ = vocabulary; }
  void set_words_ocurrence(const std::vector<int> &words_occurrence) {
    words_occurrence_ = words_occurrence;
  }
  void set_words_ocurrence(const std::vector<Histogram> &vec_hist);
  // Utility funcs
  void serialize_dic(const std::string &filename) const;
  void deserialize_dic(const std::string &filename);
};

} // namespace bovw

#endif