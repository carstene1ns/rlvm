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

#ifndef SRC_MACHINEBASE_LONGOPERATION_HPP_
#define SRC_MACHINEBASE_LONGOPERATION_HPP_

#include <boost/scoped_ptr.hpp>

#include "Systems/Base/EventListener.hpp"

class RLMachine;

// A LongOperation is a non-trivial command that requires multiple
// passes through the game loop to complete.
//
// For example, pause(). The pause() RLOperation sets the pause()
// LongOperation, which is executed instead of normal
// interpretation. The pause() LongOperation checks for any input from
// the user (ctrl or mouse click), returning true when it detects it,
// telling the RLMachine to delete the current LongOperation and
// resume normal operations.
//
// Since we are taking over normal event flow, we implement
// EventListener. LongOperations on the top of the callstack receive key/mouse
// events.
class LongOperation : public EventListener {
 public:
  LongOperation();
  virtual ~LongOperation();

  // Executes the current LongOperation. Returns true if the command has
  // completed, and normal interpretation should be resumed, false otherwise.
  virtual bool operator()(RLMachine& machine) = 0;

  // How long this operation should sleep between invocations so that we don't
  // busyloop too much. Defaults to 10ms.
  virtual int sleepTime();
};

// LongOperator decorator that simply invokes the included
// LongOperation and when that LongOperation finishes, performs an
// arbitrary action.
class PerformAfterLongOperationDecorator : public LongOperation {
 public:
  explicit PerformAfterLongOperationDecorator(LongOperation* in_op);
  ~PerformAfterLongOperationDecorator();

  // Overridden from EventListener:
  // Forward all messages to our held operation
  virtual void mouseMotion(const Point& new_location);
  virtual bool mouseButtonStateChanged(MouseButton mouse_button, bool pressed);
  virtual bool keyStateChanged(KeyCode key_code, bool pressed);

  // Overridden from LongOperation:
  virtual bool operator()(RLMachine& machine);

 private:
  boost::scoped_ptr<LongOperation> operation_;

  // Payload of decorator implemented by subclasses
  virtual void performAfterLongOperation(RLMachine& machine) = 0;
};

#endif  // SRC_MACHINEBASE_LONGOPERATION_HPP_
