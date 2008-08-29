# -*- coding: utf-8 -*-

# mouseTrap
#
# Copyright 2008 Flavio Percoco Premoli
#
# This file is part of mouseTrap.
#
# mouseTrap is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# mouseTrap is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE\  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with mouseTrap.  If not, see <http://www.gnu.org/licenses/>.

"""The mouse events handler."""

__id__        = "$Id$"
__version__   = "$Revision$"
__date__      = "$Date$"
__copyright__ = "Copyright (c) 2008 Flavio Percoco Premoli"
__license__   = "GPLv2"

import gtk
import pyatspi
import environment as env
import Xlib.ext.xtest as xtest

from Xlib import X, display
    
clickVal  = { X.ButtonPress   : 0, 
              X.ButtonRelease : 5}
              
clickType = { 'p' : [ X.ButtonPress ],
              'r' : [ X.ButtonRelease ],
              'c' : [ X.ButtonPress, X.ButtonRelease],
              'd' : [ X.ButtonPress, X.ButtonRelease,
                      X.ButtonPress, X.ButtonRelease ] }
                      
                      
## GTK Display for any user
gtkDisplay = gtk.gdk.Display( "" )

## X Display for non gnome users
xDisplay   = display.Display()

isGnome = False
if env.desktop == "gnome":
    isGnome = True
    
    ## pyatspi registry for gnome users
    reg = pyatspi.Registry
    
## Is the D&D click being used ?
dragging = False 
        
def handler( func ):
    def wrapper( *arg, **kw ):
        return dsp[arg[0]]( *arg[1:], **kw )
    return wrapper
    
def position( *arg ):
    """
    Get the absolute position of the mouse pointer
    
    Returns A list with the X and Y coordinates.
    """
    return list(gtkDisplay.get_pointer()[1:3])
    
def click( x = None, y = None, button = "bc1" ):
    
    global isGnome
    global dragging

    if not x or not y:
        x, y = position()
        
    if dragging:
        button = button[:2] + 'r'
        dragging = False
    elif button[2] == 'p':
        dragging = True
        
    if isGnome:
        try:
            reg.generateMouseEvent( x, y, button )
        except:
            isGnome = False
    else:
        for click in clickType[button[2]]:
            xDisplay.xtest_fake_input(click, int(button[1]), clickVal[click])
        xDisplay.flush()
        
    return True
    
def move( x, y ):
    """
    Changes the mouse position to the specified coords.
    
    Arguments:
    - self: The main object pointer.
    - x: The x position.
    - y: the y position.
    """
    global isGnome
        
    if isGnome:
        try:
            reg.generateMouseEvent( x, y, 'abs' )
        except:
            isGnome = False
    else:
        xtest.fake_input( xDisplay, X.MotionNotify, x = x, y = y)
        #display.sync()
        xDisplay.flush()
        
    return True
        
# Dictionary Dispatcher
dsp = { "move"      : move,
        "click"     : click,
        "position" : position }