// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#include "theia/util/filesystem.h"

#include <glog/logging.h>
#include <stlplus3/file_system.hpp>
#include <string>
#include <vector>

namespace theia
{

  bool GetFilepathsFromWildcard(
      const std::string &filepath_with_wildcard,
      std::vector<std::string> *filepaths)
  {
    CHECK_NOTNULL(filepaths)->clear();

    const std::string folder = stlplus::folder_part(filepath_with_wildcard);
    if (!stlplus::folder_exists(folder))
    {
      VLOG(2) << "Input folder does not exist:" << folder;
      return false;
    }

    const std::string filename_part =
        stlplus::filename_part(filepath_with_wildcard);

    const bool kReturnSubfolders = false;
    const bool kReturnFiles = true;
    std::vector<std::string> image_filenames = stlplus::folder_wildcard(
        folder, filename_part, kReturnSubfolders, kReturnFiles);

    filepaths->resize(image_filenames.size());
    for (int i = 0; i < filepaths->size(); i++)
    {
      filepaths->at(i) = stlplus::create_filespec(folder, image_filenames[i]);
    }

    if (filepaths->size() == 0)
    {
      VLOG(2) << "No files matched the input.";
    }

    return true;
  }

  bool GetFilenameFromFilepath(const std::string &filepath,
                               const bool with_extension,
                               std::string *filename)
  {
    CHECK_NOTNULL(filename)->clear();

    if (with_extension)
    {
      *filename = stlplus::filename_part(filepath);
    }
    else
    {
      *filename = stlplus::basename_part(filepath);
    }

    return filename->length() > 0;
  }

  bool FileExists(const std::string &filename)
  {
    return stlplus::file_exists(filename);
  }

  // Returns true if the directory exists, false otherwise.
  bool DirectoryExists(const std::string &directory)
  {
    return stlplus::folder_exists(directory);
  }

  // Creates the given directory.
  bool CreateDirectory(const std::string &directory)
  {
    return stlplus::folder_create(directory);
  }

} // namespace theia
