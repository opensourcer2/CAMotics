/******************************************************************************\

    CAMotics is an Open-Source simulation and CAM software.
    Copyright (C) 2011-2017 Joseph Coffland <joseph@cauldrondevelopment.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

\******************************************************************************/

#pragma once

#include <gcode/MoveStream.h>
#include <gcode/ToolTable.h>

#include <cbang/json/Serializable.h>
#include <cbang/geom/Rectangle.h>

#include <vector>
#include <ostream>


namespace cb {namespace JSON {class Sink;}}

namespace GCode {
  class STL;

  class ToolPath :
    protected std::vector<GCode::Move>, public cb::Rectangle3D,
    public GCode::MoveStream, public cb::JSON::Serializable {
    GCode::ToolTable tools;

    double time;
    double distance;

  public:
    ToolPath(const GCode::ToolTable &tools) :
      tools(tools), time(0), distance(0) {}
    ~ToolPath();

    const cb::Rectangle3D &getBounds() const {return *this;}
    const GCode::ToolTable &getTools() const {return tools;}
    GCode::ToolTable &getTools() {return tools;}
    double getTime() const {return time;}
    double getDistance() const {return distance;}

    int find(double time, unsigned first, unsigned last) const;
    int find(double time) const;

    void print() const {}

    // From std::vector<GCode::Move>
    typedef std::vector<GCode::Move> path_t;
    using path_t::size;
    using path_t::empty;
    using path_t::begin;
    using path_t::end;
    using path_t::const_iterator;
    const Move &operator[](int i) const
    {return (*static_cast<const path_t *>(this))[i];}
    const Move &at(int i) const
    {return static_cast<const path_t *>(this)->at(i);}

    // From cb::JSON::Serializable
    using cb::JSON::Serializable::read;
    using cb::JSON::Serializable::write;
    void read(const cb::JSON::Value &value);
    void write(cb::JSON::Sink &sink) const;

    // From GCode::MoveStream
    void move(GCode::Move &move);
  };
}
