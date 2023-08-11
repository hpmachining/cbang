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

#include <cbang/json/Value.h>
#include <cbang/json/Reader.h>
#include <cbang/json/YAMLReader.h>

#include <iostream>

using namespace std;
using namespace cb::JSON;


int main(int argc, char *argv[]) {
  try {
    ValuePtr data;

    if (argc == 2 && string(argv[1]) == "--yaml") {
      YAMLReader reader(cin);
      YAMLReader::docs_t docs;

      reader.parse(docs);
      for (unsigned i = 0; i < docs.size(); i++) {
        if (i) cout << '\n';
        cout << *docs[i];
      }

    } else {
      Reader reader(cin);
      data = reader.parse();
      if (!data.isNull()) cout << *data;
    }

    return 0;

  } CBANG_CATCH_ERROR;
  return 0;
}
