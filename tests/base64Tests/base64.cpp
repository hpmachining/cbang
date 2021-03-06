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

#include <cbang/net/Base64.h>

#include <cbang/Catch.h>

#include <iostream>

using namespace std;
using namespace cb;


int usage(const char *name) {
  cerr << "Usage: " << name << " <-d | -e> <string>" << endl;
  return 1;
}


int main(int argc, char *argv[]) {
  try {
    if (argc != 3) return usage(argv[0]);

    if (string("-d") == argv[1]) cout << Base64().decode(argv[2]) << endl;
    else if (string("-e") == argv[1]) cout << Base64().encode(argv[2]) << endl;
    else return usage(argv[0]);

    return 0;

  } CBANG_CATCH_ERROR;
  return 1;
}
