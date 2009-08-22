// -*- Mode: C++; tab-width:2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi:tw=80:et:ts=2:sts=2
//
// -----------------------------------------------------------------------
//
// This file is part of RLVM, a RealLive virtual machine clone.
//
// -----------------------------------------------------------------------
//
// Copyright (C) 2006, 2007 Elliot Glaysher
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// -----------------------------------------------------------------------

#include "Systems/Base/Colour.hpp"
#include "Systems/Base/GraphicsSystem.hpp"
#include "Systems/Base/GraphicsObject.hpp"

#include "TestSystem/TestGraphicsSystem.hpp"
#include "TestSystem/TestSurface.hpp"

#include "Utilities/Exception.hpp"

#include <sstream>

using namespace std;

// -----------------------------------------------------------------------

TestGraphicsSystem::TestGraphicsSystem(System& system, Gameexe& gexe)
  : GraphicsSystem(system, gexe) {
  for (int i = 0; i < 16; ++i) {
    ostringstream oss;
    oss << "DC #" << i;
    display_contexts_[i].reset(new TestSurface(oss.str()));
  }

  display_contexts_[0]->allocate(screenSize());
  display_contexts_[1]->allocate(screenSize());
}

// -----------------------------------------------------------------------

void TestGraphicsSystem::allocateDC(int dc, Size size) {
  if(dc >= 16)
    throw rlvm::Exception("Invalid DC number in TestGrpahicsSystem::allocate_dc");

  // We can't reallocate the screen!
  if(dc == 0)
    throw rlvm::Exception("Attempting to reallocate DC 0!");

  // DC 1 is a special case and must always be at least the size of
  // the screen.
  if(dc == 1) {
    boost::shared_ptr<TestSurface> dc0 = display_contexts_[0];
    if(size.width() < dc0->size().width())
      size.setWidth(dc0->size().width());
    if(size.height() < dc0->size().height())
      size.setHeight(dc0->size().height());
  }

  // Allocate a new obj.
  display_contexts_[dc]->allocate(size);
}

// -----------------------------------------------------------------------

void TestGraphicsSystem::freeDC(int dc) {
  if(dc == 0)
    throw rlvm::Exception("Attempt to deallocate DC[0]");
  else if(dc == 1) {
    // DC[1] never gets freed; it only gets blanked
    display_contexts_[1]->fill(RGBAColour::Black());
  } else
    display_contexts_[dc]->deallocate();
}

// -----------------------------------------------------------------------

void TestGraphicsSystem::clearAndPromoteObjects() {
}

// -----------------------------------------------------------------------

GraphicsObject& TestGraphicsSystem::getObject(int layer, int obj_number) {
  static GraphicsObject x;
  return x;
}

// -----------------------------------------------------------------------

boost::shared_ptr<Surface> TestGraphicsSystem::loadNonCGSurfaceFromFile(
    const std::string& short_filename) {
  // Make this a real surface so we can track what's done with it
  return boost::shared_ptr<Surface>(
    new TestSurface(short_filename, Size(50, 50)));
}

// -----------------------------------------------------------------------

boost::shared_ptr<Surface> TestGraphicsSystem::getDC(int dc) {
  return display_contexts_[dc];
}

// -----------------------------------------------------------------------

boost::shared_ptr<Surface> TestGraphicsSystem::buildSurface(const Size& s) {
  static int surface_num = 0;
  ostringstream oss;
  oss << "Built Surface #" << surface_num++;

  return boost::shared_ptr<Surface>(new TestSurface(oss.str(), s));
}

// -----------------------------------------------------------------------

void TestGraphicsSystem::blitSurfaceToDC(
  Surface& source_obj, int target_dc,
  int srcX, int srcY, int src_width, int src_height,
  int destX, int destY, int dest_width, int dest_height,
  int alpha) {
  // TODO
}