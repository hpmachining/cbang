/******************************************************************************\

          This file is part of the C! library.  A.K.A the cbang library.

                Copyright (c) 2003-2024, Cauldron Development LLC
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

#include "GoogleProvider.h"

#include <cbang/net/URI.h>
#include <cbang/config/Options.h>
#include <cbang/json/JSON.h>

using namespace std;
using namespace cb;
using namespace cb::OAuth2;


GoogleProvider::GoogleProvider() :
  Provider("https://accounts.google.com/o/oauth2/v2/auth",
           "https://www.googleapis.com/oauth2/v4/token",
           "https://www.googleapis.com/oauth2/v3/userinfo",
           "openid email profile") {}


SmartPointer<JSON::Value>
GoogleProvider::processProfile(const SmartPointer<JSON::Value> &profile) const {
  SmartPointer<JSON::Value> p = new JSON::Dict;

  p->insert("provider",        "google");
  p->insert("id",              profile->getString("sub"));
  p->insert("name",            profile->getString("name"));
  p->insert("email",           profile->getString("email"));
  p->insert("avatar",          profile->getString("picture"));
  p->insertBoolean("verified", profile->getBoolean("email_verified"));
  p->insert("raw", profile);

  return p;
}