/******************************************************************************\

          This file is part of the C! library.  A.K.A the cbang library.

                Copyright (c) 2003-2019, Cauldron Development LLC
                   Copyright (c) 2003-2017, Stanford University
                               All rights reserved.

         The C! library is free software: you can redistribute it and/or
        modify it under the terms of the GNU Lesser General Public License
       as published by the Free Software Foundation, either version 2.1 of
               the License, or (at your option) any later version.

        The C! library is distributed in the hope that it will be useful,
          but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
                 Lesser General Public License for more details.

         You should have received a copy of the GNU Lesser General Public
                 License along with the C! library.  If not, see
                         <http://www.gnu.org/licenses/>.

        In addition, BSD licensing may be granted on a case by case basis
        by written permission from at least one of the copyright holders.
           You may request written permission by emailing the authors.

                  For information regarding this software email:
                                 Joseph Coffland
                          joseph@cauldrondevelopment.com

\******************************************************************************/

#include "TarFileReader.h"

#include <cbang/os/SystemUtilities.h>
#include <cbang/os/SysError.h>
#include <cbang/log/Logger.h>
#include <cbang/iostream/BZip2Decompressor.h>
#include <cbang/iostream/LZ4Decompressor.h>

#include <boost/ref.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/gzip.hpp>
namespace io = boost::iostreams;

using namespace cb;
using namespace std;


struct TarFileReader::private_t {
  io::filtering_istream filter;
};


TarFileReader::TarFileReader(const string &path, Compression compression) :
  pri(new private_t), stream(SystemUtilities::iopen(path)),
  didReadHeader(false) {

  if (compression == COMPRESSION_AUTO) compression = CompressionFromPath(path);
  addCompression(compression);
  pri->filter.push(*this->stream);
}


TarFileReader::TarFileReader(istream &stream, Compression compression) :
  pri(new private_t), stream(SmartPointer<istream>::Phony(&stream)),
  didReadHeader(false) {

  addCompression(compression);
  pri->filter.push(*this->stream);
}


TarFileReader::~TarFileReader() {delete pri;}


bool TarFileReader::hasMore() {
  if (!didReadHeader) {
    SysError::clear();
    if (!readHeader(pri->filter))
      THROW("Tar file read failed: " << SysError());
    didReadHeader = true;
  }

  return !isEOF();
}


bool TarFileReader::next() {
  if (didReadHeader) {
    skipFile(pri->filter);
    didReadHeader = false;
  }

  return hasMore();
}


std::string TarFileReader::extract(const string &_path) {
  if (_path.empty()) THROW("path cannot be empty");
  if (!hasMore()) THROW("No more tar files");

  string path = _path;
  if (SystemUtilities::isDirectory(path)) {
    path += "/" + getFilename();

    // Check that path is under the target directory
    string a = SystemUtilities::getCanonicalPath(_path);
    string b = SystemUtilities::getCanonicalPath(path);
    if (!String::startsWith(b, a))
      THROW("Tar path points outside of the extraction directory: " << path);
  }

  LOG_DEBUG(5, "Extracting: " << path);

  switch (getType()) {
  case NORMAL_FILE: case CONTIGUOUS_FILE:
    return extract(*SystemUtilities::oopen(path));
  case DIRECTORY: SystemUtilities::ensureDirectory(path); break;
  default: THROW("Unsupported tar file type " << getType());
  }

  return getFilename();
}


string TarFileReader::extract(ostream &out) {
  if (!hasMore()) THROW("No more tar files");

  readFile(out, pri->filter);
  didReadHeader = false;

  return getFilename();
}


void TarFileReader::addCompression(Compression compression) {
  switch (compression) {
  case COMPRESSION_NONE:                                             break;
  case COMPRESSION_BZIP2: pri->filter.push(BZip2Decompressor());     break;
  case COMPRESSION_GZIP:  pri->filter.push(io::gzip_decompressor()); break;
  case COMPRESSION_ZLIB:  pri->filter.push(io::zlib_decompressor()); break;
  case COMPRESSION_LZ4:   pri->filter.push(LZ4Decompressor());       break;
  default: THROW("Invalid compression type " << compression);
  }
}
