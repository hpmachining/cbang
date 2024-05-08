/******************************************************************************\

          This file is part of the C! library.  A.K.A the cbang library.

                Copyright (c) 2021-2024, Cauldron Development  Oy
                Copyright (c) 2003-2021, Cauldron Development LLC
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

#pragma once

#include "Query.h"

#include <cbang/http/Request.h>


namespace cb {
  namespace API {
    class API;

    class Login : public Query {
      API &api;
      std::string provider;
      std::string redirectURI;
      unsigned resultCount = 0;

    public:
      Login(API &api, const SmartPointer<HTTP::Request> &req,
            const std::string &sql, const std::string &provider,
            const std::string &redirectURI);

      void loginComplete();
      void login(callback_t cb);
      void processProfile(const JSON::ValuePtr &profile);

      // From Query
      void callback(state_t state) override;
    };
  }
}
