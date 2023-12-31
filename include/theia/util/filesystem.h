// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_UTIL_FILESYSTEM_H_
#define THEIA_UTIL_FILESYSTEM_H_

#include <string>
#include <vector>

namespace theia {

// Gets the filepath of all files matching the input wildcard. Returns true if
// the wildcard could be successfully evaluated and false otherwise (e.g. if the
// folder does not exist).
bool GetFilepathsFromWildcard(const std::string& filepath_with_wildcard,
                              std::vector<std::string>* filepaths);

// Extracts the filename from the filepath (i.e., removes all directory
// information). If with_extension is set to true then the extension is kept and
// output with the filename, otherwise the extension is removed.
bool GetFilenameFromFilepath(const std::string& filepath,
                             const bool with_extension,
                             std::string* filename);

// Returns true if the file exists, false otherwise.
bool FileExists(const std::string& filename);

// Returns true if the directory exists, false otherwise.
bool DirectoryExists(const std::string& directory);

// Creates the given directory.
bool CreateDirectory(const std::string& directory);

}  // namespace theia

#endif  // THEIA_UTIL_FILESYSTEM_H_
