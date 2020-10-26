#include "image_browser.hpp"
#include "html_writer.hpp"
#include <fstream>
#include <iostream>

namespace bovw::image_browser {

void AddFullRow(const ImageRow &row, std::ofstream &ofs, bool first_row) {
  ofs << "<div class=\"row\">\n";

  for (const ScoredImage &image : row) {
    // auto [img_path, score] = image;
    if (first_row) {
      AddImage(std::get<0>(image), std::get<1>(image), ofs, first_row);
      first_row = false;
    } else {
      AddImage(std::get<0>(image), std::get<1>(image), ofs);
    }
  }
  ofs << "</div>\n";
}

void CreateImageBrowser(const std::string &title, const std::string &stylesheet,
                        const std::vector<ImageRow> &rows, std::ofstream &ofs) {
  ofs << "<!DOCTYPE html>\n"
      << "<html>\n";
  AddTitle(title, ofs);
  AddCSSStyle(stylesheet, ofs);
  OpenBody(ofs);

  bool first_row = true;
  for (const ImageRow &row : rows) {
    if (first_row) {
      AddFullRow(row, ofs, first_row);
      first_row = false;
    } else {
      AddFullRow(row, ofs);
    }
  }

  CloseBody(ofs);

  ofs << "</html>\n";
}

} // namespace bovw::image_browser