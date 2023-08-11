/******************************************************************************\

          This file is part of the C! library.  A.K.A the cbang library.

                Copyright (c) 2003-2023, Cauldron Development LLC
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

#include <cbang/Catch.h>
#include <cbang/gpu/CUDALibrary.h>
#include <cbang/gpu/OpenCLLibrary.h>

#include <iostream>

using namespace std;
using namespace cb;


template <typename LIB>
void printCD(const char *name) try {
  cout << name << ":\n";

  auto &lib = LIB::instance();
  for (auto &cd: lib)
    cout << "  " << cd << '\n';
} CATCH_ERROR;


int main(int argc, char *argv[]) {
  printCD<CUDALibrary>("CUDA");
  printCD<OpenCLLibrary>("OpenCL");
  return 0;
}
