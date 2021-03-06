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

#include "PCIDevice.h"

#include <cbang/String.h>

using namespace std;
using namespace cb;


PCIDevice::PCIDevice(uint16_t vendorID, uint16_t deviceID, int16_t busID ,
                     int16_t slotID, int16_t functionID, const string &name) :
  vendor(0), id(deviceID), bus(busID), slot(slotID), function(functionID),
  name(name) {
  setVendorID(vendorID);
}


void PCIDevice::setVendorID(uint16_t vendorID) {
  if (vendorID) vendor = PCIVendor::find(vendorID);
  else vendor = 0;
}


string PCIDevice::getVendorIDStr() const {
  return String::printf("0x%04x", getVendorID());
}


string PCIDevice::getDeviceIDStr() const {
  return String::printf("0x%04x", getDeviceID());
}


string PCIDevice::toString() const {
  if (!getName().empty()) return getName();
  return getVendorIDStr() + ":" + getDeviceIDStr();
}


bool PCIDevice::operator<(const PCIDevice &d) const {
  if (bus != d.bus) return bus < d.bus;
  if (slot != d.slot) return slot < d.slot;
  if (function != d.function) return function < d.function;
  if (getVendorID() != d.getVendorID()) return getVendorID() < d.getVendorID();
  return id < d.id;
}
