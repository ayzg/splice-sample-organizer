// -----------------------------------------------------------------------------
// Splice File Organizer
// Author: Anton Yashchenko 
// Website: bigteeny.com
// Date: 2023/11/19
// -----------------------------------------------------------------------------
// Description:
// This program organizes audio files from a Splice Samples folder into a
// specified destination folder based on their content. The folder structure
// and criteria for file organization are as follows:
//
// - Drums:
//   - 808: Files containing "808" in the filename
//   - Snare: Files containing "snare," "_snr," or "snr_" in the filename
//   - Kick: Files containing "kick," "_kck," or "kck_" in the filename
//   - Clap: Files containing "clap," "_clp," or "clp_" in the filename
//   - Hat: Files containing "hat," "ht_," or "_ht" in the filename
//   - Other: Files containing "drum," "_drm," or "drm_" in the filename
//
// - Other:
//   - Loop: Files containing "loop" in the filename
//   - Other: Files that don't match any of the above criteria
//
// The program prompts the user for the source and destination folder names and
// organizes the files accordingly. It also provides an option to print detailed
// information about each file move.
//
// Additional Requirements:
// - Set the environment variable "MAX_PATH" to a greater value (e.g., 32767) to
//   be able handle long file paths.
// -----------------------------------------------------------------------------
// License:
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#pragma once
#ifndef GUARD_SPLICE_FILE_ORGANIZER_HPP
#define GUARD_SPLICE_FILE_ORGANIZER_HPP

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <set>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// Set to store processed file names
std::set<std::string> processed_files;

// Convert a string to lowercase
std::string to_lower(const std::string& str) {
  std::string lower_str = str;
  std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(),
                 ::tolower);
  return lower_str;
}

// Check if a file is an audio file (wav or mp3)
bool is_audio_file(const fs::path& file_path) {
  std::string extension = to_lower(file_path.extension().string());
  return extension == ".wav" || extension == ".mp3";
}

// Check if a file has already been processed
bool file_already_processed(const fs::path& file_path) {
  std::string filename = to_lower(file_path.filename().string());
  return processed_files.find(filename) != processed_files.end();
}

// Append an index to a filename
std::string append_index_to_filename(const fs::path& filename, int index) {
  std::string base_name = filename.stem().string();
  std::string extension = filename.extension().string();
  return base_name + "_" + std::to_string(index) + extension;
}

// Create parent directories for a file
void create_parent_directories(const fs::path& file_path) {
  fs::create_directories(file_path.parent_path());
}

// Print information about a file move
void print_file_move_info(const fs::path& source, const fs::path& destination) {
  std::cout << "Moved: \n";
  std::cout << "  From: " << source << "\n";
  std::cout << "  To:   " << destination << "\n";
}

// Organize a file based on its content
void organize_file(const fs::path& source, const fs::path& destination,
                   bool print_info) {
  if (!is_audio_file(source)) {
    return;  // Ignore non-audio files
  }

  std::string filename = to_lower(source.filename().string());
  fs::path dest_path = destination;

  // Organize files based on content
  if (filename.find("808") != std::string::npos) {
    dest_path /= "Drums";
    dest_path /= "808";
    dest_path /= source.filename().c_str();
  } else if (filename.find("snare") != std::string::npos ||
             filename.find("_snr") != std::string::npos ||
             filename.find("snr_") != std::string::npos) {
    dest_path /= "Drums";
    dest_path /= "Snare";
    dest_path /= source.filename().c_str();
  } else if (filename.find("kick") != std::string::npos ||
             filename.find("_kck") != std::string::npos ||
             filename.find("kck_") != std::string::npos) {
    dest_path /= "Drums";
    dest_path /= "Kick";
    dest_path /= source.filename().c_str();
  } else if (filename.find("clap") != std::string::npos ||
             filename.find("_clp") != std::string::npos ||
             filename.find("clp_") != std::string::npos) {
    dest_path /= "Drums";
    dest_path /= "Clap";
    dest_path /= source.filename().c_str();
  } else if (filename.find("hat") != std::string::npos ||
             filename.find("ht_") != std::string::npos ||
             filename.find("_ht") != std::string::npos) {
    dest_path /= "Drums";
    dest_path /= "Hat";
    dest_path /= source.filename().c_str();
  } else if (filename.find("drum") != std::string::npos ||
             filename.find("_drm") != std::string::npos ||
             filename.find("drm_") != std::string::npos) {
    dest_path /= "Drums";
    dest_path /= "Other";
    dest_path /= source.filename().c_str();
  } else if (filename.find("loop") != std::string::npos) {
    dest_path /= "Other";
    dest_path /= "Loop";
    dest_path /= source.filename().c_str();
  } else {
    dest_path /= "Other";
    dest_path /= "Other";
    dest_path /= source.filename().c_str();
  }

  try {
    create_parent_directories(dest_path);

    if (print_info) {
      std::cout << "Request: \n";
      std::cout << "  Source:      " << source << "\n";
      std::cout << "  Destination: " << dest_path << "\n";
    }

    // Perform the file move
    if (!fs::exists(dest_path)) {
      fs::copy_file(source, dest_path);
      if (print_info) print_file_move_info(source, dest_path);
    } else {
      if (!file_already_processed(dest_path)) {
        fs::remove(dest_path);
        fs::copy_file(source, dest_path);
        if (print_info) print_file_move_info(source, dest_path);
      } else {
        for (int i = 0; i < 999999; ++i) {
          fs::path indexed_dest_path =
              destination / append_index_to_filename(source.filename(), i);
          if (!fs::exists(indexed_dest_path)) {
            fs::copy_file(source, indexed_dest_path);
            if (print_info) print_file_move_info(source, indexed_dest_path);
            break;
          }
        }
      }
    }

    // Mark the file as processed
    processed_files.insert(to_lower(source.filename().string()));
  } catch (const fs::filesystem_error& e) {
    std::cerr << "\nError copying file: " << e.what() << "\n";
  }
}

// Process a directory and organize its files
void process_directory(const fs::path& source, const fs::path& destination,
                       bool print_info) {
  try {
    for (const auto& entry : fs::directory_iterator(source)) {
      if (entry.is_directory()) {
        process_directory(entry.path(), destination, print_info);
      } else {
        organize_file(entry.path(), destination, print_info);
      }
    }
  } catch (const fs::filesystem_error& e) {
    std::cerr << "\nError processing file system: \n" << e.what() << "\n";
  }
}

int main() {
  std::cout << "---------------------------------------------------------------"
               "-------------- "
            << std::endl;
  std::cout << "Splice File Organizer " << std::endl;
  std::cout << "Author: Anton Yashchenko (BigTeeny) " << std::endl;
  std::cout << "Contact: bigteenymusic@gmail.com " << std::endl;
  std::cout << "Website: bigteenymusic.com \n" << std::endl;

  std::cout << "Enter Splice Samples folder name: ";
  std::string source_folder;
  std::cin >> source_folder;

  // Verify if the source folder exists
  if (!fs::exists(source_folder)) {
    std::cout << "Source folder does not exist. Exiting.\n";
    return 1;
  }

  std::cout << "Enter destination folder name: ";
  std::string destination_folder;
  std::cin >> destination_folder;

  // Ask the user if they want to print detailed information about file moves
  std::cout
      << "Print the source and destination info: Enter 1 for YES, 0 for NO: ";
  bool print_info;
  std::cin >> print_info;

  // Set the source and destination folders
  fs::path source_path = source_folder;
  fs::path destination_path = destination_folder;

  // Create destination folders if they don't exist
  fs::create_directories(destination_path / "Drums" / "808");
  fs::create_directories(destination_path / "Drums" / "Snare");
  fs::create_directories(destination_path / "Drums" / "Kick");
  fs::create_directories(destination_path / "Drums" / "Clap");
  fs::create_directories(destination_path / "Drums" / "Hat");
  fs::create_directories(destination_path / "Drums" / "Other");
  fs::create_directories(destination_path / "Other" / "Loop");
  fs::create_directories(destination_path / "Other" / "Other");

  // Process the source directory
  process_directory(source_path, destination_path, print_info);

  std::cout << "Splice Files organized successfully.\n";

  std::cout << "Press Enter to exit...";
  std::cin.ignore();  // Ignore the previous newline character
  std::cin.get();     // Wait for the user to press Enter

  return 0;
}

#endif  // !GUARD_SPLICE_FILE_ORGANIZER_HPP