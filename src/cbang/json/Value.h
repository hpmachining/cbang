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

#include "ValueType.h"
#include "Factory.h"
#include "Writer.h"

#include <ostream>


namespace cb {
  namespace JSON {
    class List;
    class Dict;
    class Null;
    class Value;
    class Sink;

    class Value : public Factory, public ValueType::Enum {
    public:
      virtual ~Value() {}

      virtual ValueType getType() const = 0;
      virtual ValuePtr copy(bool deep = false) const = 0;
      virtual bool canWrite(Sink &sink) const {return true;}

      virtual bool isSimple() const {return true;}
      bool isUndefined() const {return getType() == JSON_UNDEFINED;}
      bool isNull() const {return getType() == JSON_NULL;}
      bool isBoolean() const {return getType() == JSON_BOOLEAN;}
      bool isNumber() const {return getType() == JSON_NUMBER;}
      bool isString() const {return getType() == JSON_STRING;}
      bool isList() const {return getType() == JSON_LIST;}
      bool isDict() const {return getType() == JSON_DICT;}

      template <typename T> bool is() {return dynamic_cast<T *>(this);}

      template <typename T> T &cast() {
        T *ptr = dynamic_cast<T *>(this);
        if (!ptr) CBANG_THROW("Invalid cast");
        return *ptr;
      }

      template <typename T> const T &cast() const {
        const T *ptr = dynamic_cast<T *>(this);
        if (!ptr) CBANG_THROW("Invalid cast");
        return *ptr;
      }

      virtual bool getBoolean() const {CBANG_THROW("Value is not a Boolean");}
      virtual double getNumber() const {CBANG_THROW("Value is not a Number");}
      virtual int8_t getS8() const {CBANG_THROW("Value is not a Number");}
      virtual uint8_t getU8() const {CBANG_THROW("Value is not a Number");}
      virtual int16_t getS16() const {CBANG_THROW("Value is not a Number");}
      virtual uint16_t getU16() const {CBANG_THROW("Value is not a Number");}
      virtual int32_t getS32() const {CBANG_THROW("Value is not a Number");}
      virtual uint32_t getU32() const {CBANG_THROW("Value is not a Number");}
      virtual int64_t getS64() const {CBANG_THROW("Value is not a Number");}
      virtual uint64_t getU64() const {CBANG_THROW("Value is not a Number");}
      virtual std::string &getString() {CBANG_THROW("Value is not a String");}
      virtual const std::string &getString() const
      {CBANG_THROW("Value is not a String");}
      virtual List &getList() {CBANG_THROW("Value is not a List");}
      virtual const List &getList() const {CBANG_THROW("Value is not a List");}
      virtual Dict &getDict() {CBANG_THROW("Value is not a Dict");}
      virtual const Dict &getDict() const {CBANG_THROW("Value is not a Dict");}

      virtual void setBoolean(bool value)
      {CBANG_THROW("Value is not a Boolean");}
      virtual void set(double value) {CBANG_THROW("Value is not a Number");}
      virtual void set(float value) {set((double)value);}
      virtual void set(int8_t value) {set((int16_t)value);}
      virtual void set(uint8_t value) {set((uint16_t)value);}
      virtual void set(int16_t value) {set((int32_t)value);}
      virtual void set(uint16_t value) {set((uint32_t)value);}
      virtual void set(int32_t value) {set((int64_t)value);}
      virtual void set(uint32_t value) {set((uint64_t)value);}
      virtual void set(int64_t value) {set((double)value);}
      virtual void set(uint64_t value) {set((double)value);}
      virtual void set(const std::string &value) {CBANG_THROW("Not a String");}

      virtual unsigned size() const {CBANG_THROW("Not a List or Dict");}

      // List functions
      virtual const ValuePtr &get(unsigned i) const
      {CBANG_THROW("Not a List or Dict");}
      void appendDict();
      void appendList();
      void appendUndefined();
      void appendNull();
      void appendBoolean(bool value);
      void append(double value);
      void append(float value) {append((double)value);}
      void append(int8_t value) {append((int16_t)value);}
      void append(uint8_t value) {append((uint16_t)value);}
      void append(int16_t value) {append((int32_t)value);}
      void append(uint16_t value) {append((uint32_t)value);}
      void append(int32_t value) {append((int64_t)value);}
      void append(uint32_t value) {append((uint64_t)value);}
      void append(int64_t value);
      void append(uint64_t value);
      void append(const std::string &value);
      virtual void append(const ValuePtr &value) {CBANG_THROW("Not a List");}

      // List accessors
      bool getBoolean(unsigned i) const {return get(i)->getBoolean();}
      double getNumber(unsigned i) const {return get(i)->getNumber();}
      int8_t getS8(unsigned i) const {return get(i)->getS8();}
      uint8_t getU8(unsigned i) const {return get(i)->getU8();}
      int16_t getS16(unsigned i) const {return get(i)->getS16();}
      uint16_t getU16(unsigned i) const {return get(i)->getU16();}
      int32_t getS32(unsigned i) const {return get(i)->getS32();}
      uint32_t getU32(unsigned i) const {return get(i)->getU32();}
      int64_t getS64(unsigned i) const {return get(i)->getS64();}
      uint64_t getU64(unsigned i) const {return get(i)->getU64();}
      std::string &getString(unsigned i) {return get(i)->getString();}
      const std::string &getString(unsigned i) const
      {return get(i)->getString();}
      std::string getAsString(unsigned i) const {return get(i)->asString();}
      List &getList(unsigned i) {return get(i)->getList();}
      const List &getList(unsigned i) const {return get(i)->getList();}
      Dict &getDict(unsigned i) {return get(i)->getDict();}
      const Dict &getDict(unsigned i) const {return get(i)->getDict();}

      // List setters
      virtual void set(unsigned i, const ValuePtr &value)
      {CBANG_THROW("Not a List");}
      void setDict(unsigned i);
      void setList(unsigned i);
      void setUndefined(unsigned i);
      void setNull(unsigned i);
      void setBoolean(unsigned i, bool value);
      void set(unsigned i, double value);
      void set(unsigned i, float value) {set(i, (double)value);}
      void set(unsigned i, int8_t value) {set(i, (int16_t)value);}
      void set(unsigned i, uint8_t value) {set(i, (uint16_t)value);}
      void set(unsigned i, int16_t value) {set(i, (int32_t)value);}
      void set(unsigned i, uint16_t value) {set(i, (uint32_t)value);}
      void set(unsigned i, int32_t value) {set(i, (int64_t)value);}
      void set(unsigned i, uint32_t value) {set(i, (uint64_t)value);}
      void set(unsigned i, int64_t value);
      void set(unsigned i, uint64_t value);
      void set(unsigned i, const std::string &value);

      // Dict functions
      virtual const std::string &keyAt(unsigned i) const
      {CBANG_THROW("Not a Dict");}
      virtual int indexOf(const std::string &key) const
      {CBANG_THROW("Not a Dict");}
      bool has(const std::string &key) const {return indexOf(key) != -1;}
      bool hasNull(const std::string &key) const {
        int index = indexOf(key);
        return index == -1 ? false : get(index)->isNull();
      }
      bool hasBoolean(const std::string &key) const {
        int index = indexOf(key);
        return index == -1 ? false : get(index)->isBoolean();
      }
      bool hasNumber(const std::string &key) const {
        int index = indexOf(key);
        return index == -1 ? false : get(index)->isNumber();
      }
      bool hasString(const std::string &key) const {
        int index = indexOf(key);
        return index == -1 ? false : get(index)->isString();
      }
      bool hasList(const std::string &key) const {
        int index = indexOf(key);
        return index == -1 ? false : get(index)->isList();
      }
      bool hasDict(const std::string &key) const {
        int index = indexOf(key);
        return index == -1 ? false : get(index)->isDict();
      }
      virtual const ValuePtr &get(const std::string &key) const
      {CBANG_THROW("Not a Dict");}

      virtual void insert(const std::string &key, const ValuePtr &value)
      {CBANG_THROW("Not a Dict");}
      void insertDict(const std::string &key);
      void insertList(const std::string &key);
      void insertUndefined(const std::string &key);
      void insertNull(const std::string &key);
      void insertBoolean(const std::string &key, bool value);
      void insert(const std::string &key, double value);
      void insert(const std::string &key, float value)
      {insert(key, (double)value);}
      void insert(const std::string &key, uint8_t value)
      {insert(key, (uint16_t)value);}
      void insert(const std::string &key, int8_t value)
      {insert(key, (int16_t)value);}
      void insert(const std::string &key, uint16_t value)
      {insert(key, (uint32_t)value);}
      void insert(const std::string &key, int16_t value)
      {insert(key, (int32_t)value);}
      void insert(const std::string &key, uint32_t value)
      {insert(key, (uint64_t)value);}
      void insert(const std::string &key, int32_t value)
      {insert(key, (int64_t)value);}
      void insert(const std::string &key, uint64_t value);
      void insert(const std::string &key, int64_t value);
      void insert(const std::string &key, const std::string &value);
      void merge(const Value &value);

      // Dict accessors
      bool getBoolean(const std::string &key) const
      {return get(key)->getBoolean();}
      double getNumber(const std::string &key) const
      {return get(key)->getNumber();}
      int8_t getS8(const std::string &key) const
      {return get(key)->getS8();}
      uint8_t getU8(const std::string &key) const
      {return get(key)->getU8();}
      int16_t getS16(const std::string &key) const
      {return get(key)->getS16();}
      uint16_t getU16(const std::string &key) const
      {return get(key)->getU16();}
      int32_t getS32(const std::string &key) const
      {return get(key)->getS32();}
      uint32_t getU32(const std::string &key) const
      {return get(key)->getU32();}
      int64_t getS64(const std::string &key) const
      {return get(key)->getS64();}
      uint64_t getU64(const std::string &key) const
      {return get(key)->getU64();}
      std::string &getString(const std::string &key)
      {return get(key)->getString();}
      const std::string &getString(const std::string &key) const
      {return get(key)->getString();}
      std::string getAsString(const std::string &key) const
      {return get(key)->asString();}
      List &getList(const std::string &key) {return get(key)->getList();}
      const List &getList(const std::string &key) const
      {return get(key)->getList();}
      Dict &getDict(const std::string &key) {return get(key)->getDict();}
      const Dict &getDict(const std::string &key) const
      {return get(key)->getDict();}

      // Dict accessors with defaults
#define CBANG_JSON_GET_DEFAULT(NAME, TYPE)                              \
      TYPE get##NAME(const std::string &key,                            \
                     const TYPE &defaultValue) const {                  \
        try {                                                           \
          int index = indexOf(key);                                     \
          return index == -1 ? defaultValue : get(index)->get##NAME();  \
        } catch (const Exception &e) {}                                 \
        return defaultValue;                                            \
      }

      CBANG_JSON_GET_DEFAULT(Boolean, bool);
      CBANG_JSON_GET_DEFAULT(Number, double);
      CBANG_JSON_GET_DEFAULT(S8, int8_t);
      CBANG_JSON_GET_DEFAULT(U8, uint8_t);
      CBANG_JSON_GET_DEFAULT(S16, int16_t);
      CBANG_JSON_GET_DEFAULT(U16, uint16_t);
      CBANG_JSON_GET_DEFAULT(S32, int32_t);
      CBANG_JSON_GET_DEFAULT(U32, uint32_t);
      CBANG_JSON_GET_DEFAULT(S64, int64_t);
      CBANG_JSON_GET_DEFAULT(U64, uint64_t);
      CBANG_JSON_GET_DEFAULT(String, std::string);

#undef CBANG_JSON_GET_DEFAULT

      const ValuePtr &get(const std::string &key,
                          const ValuePtr &defaultValue) const {
        int index = indexOf(key);
        return index == -1 ? defaultValue : get(index);
      }

      std::string getAsString(const std::string &key,
                              const std::string &defaultValue) const {
        int index = indexOf(key);
        return index == -1 ? defaultValue : get(index)->asString();
      }

      std::string format(char type) const;
      std::string format(char type, int index, const std::string &name,
                         const String::FormatCB &cb) const;
      std::string format(const std::string &s,
                         const String::FormatCB &cb) const;
      std::string format(const std::string &s,
                         const std::string &defaultValue = "") const;

      // Operators
      const Value &operator[](unsigned i) const {return *get(i);}
      const Value &operator[](const std::string &key) const {return *get(key);}

      virtual void write(Sink &sink) const = 0;
      std::string toString(unsigned indent = 0, bool compact = false) const;
      std::string asString() const;
    };

    static inline
    std::ostream &operator<<(std::ostream &stream, const Value &value) {
      Writer writer(stream);
      value.write(writer);
      return stream;
    }
  }
}
