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

#pragma once

#include "HTTPConn.h"


namespace cb {
  namespace Event {
    class Client;

    class HTTPConnOut : public HTTPConn {
    public:
      HTTPConnOut(Client &client);

      void makeRequest(const SmartPointer<Request> &req);

      // From HTTPConn
      bool isIncoming() const {return false;}
      void writeRequest(const SmartPointer<Request> &req, Buffer buffer,
                        bool hasMore, std::function<void (bool)> cb);

    protected:
      void fail(ConnectionError err, const std::string &msg);
      void readHeader(const SmartPointer<Request> &req);
      void readBody(const SmartPointer<Request> &req);
      void process(const SmartPointer<Request> &req);
      void dispatch();
    };
  }
}
