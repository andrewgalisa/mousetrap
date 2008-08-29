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
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with mouseTrap.  If not, see <http://www.gnu.org/licenses/>.

"""The Screen Mode script."""

__id__        = "$Id$"
__version__   = "$Revision$"
__date__      = "$Date$"
__copyright__ = "Copyright (c) 2008 Flavio Percoco Premoli"
__license__   = "GPLv2"

import gtk
import mouseTrap.events as events
import mouseTrap.environment as env
import mouseTrap.mouseTrap as mouseTrap

from mouseTrap.mTi18n import _

# The name given for the config file
setName = "screen"

## Internal Modes
modes = { "screen|none"  :  _( "Mouse Screen Mode" ) }


class Profile:

    def __init__( self, gui ):
        """
        The ScreenProfile Class Constructor.
        
        Initialize the ScreenProfile class and sets the needed attributes.
        
        Arguments:
        - self: The main object pointer.
        - mouseTrap: The mouseTrap object pointer.
        - cAm: The camera object pointer.
        """

        ## Preference Vars and WidgetsTypes
        self.pref = { "vScreen" : "label" }
        
        self.mTp      = mouseTrap
        self.settings = mouseTrap.settings
        self.gui = gui

        self.mpClick = [ 0, 0, 0, 0, 0, 0 ]
        
        self._loadSettings()
            
        self._registerMapperEvents()

    def _loadSettings( self ):
        """
        This load the settings of the screen mode.
        
        Arguments:
        - self: The main object pointer.
        """
        
        try:
            self.vScreen = eval(self.settings.vScreen)
        except:
            self.vScreen = {'startX': 60, 'startY': 50, 'endY': 110, 'endX': 140, 'height': 60.0, 'width': 80.0}
            
    def _checkPref( self ):
        """
        Checks any variation in the mouseTrap settings variables related to this mode.
        
        Arguments:
        - self: The main object pointer.
        """
        
        try:
            
            if self.vScreen != eval(self.settings.vScreen):
                self._loadSettings()
                events.registerMapperEvent( "screeMode", [ self.vScreen["startX"], self.vScreen["startY"] ],
                    [ self.vScreen["endX"], self.vScreen["endY"] ], True, ["moveMode:screen"], self._moveScreenMode, 0)
        except:
            return True
            
    def _registerMapperEvents( self ):
        """
        Register the mapper events needed.
        
        Arguments:
        - self: The main object pointer.
        """


        self.gui.mapper.connect("button_press_event", self.mouseClick, self.gui.mapper )
        self.gui.mapper.connect("button_release_event", self.mouseClick, self.gui.mapper )
        self.gui.mapper.connect("motion_notify_event", self.mouseMotion, self.gui.mapper )
        
        events.registerMapperEvent( "screeMode", [ self.vScreen["startX"], self.vScreen["startY"] ],
                [ self.vScreen["endX"], self.vScreen["endY"] ], True, ["moveMode:screen"], self._moveScreenMode, 0)

    def mouseClick( self, widget, event, mapper ):
        """
        This is the callback function for the mouse clicks in the
        mapper.

        Arguments:
        - self: The main object pointer.
        - widget: The Drawing area.
        - event: The event information.
        """

        if event.button == 1 and event.type.value_name == "GDK_BUTTON_PRESS":
            self.mpClick[0], self.mpClick[1] = [ int(event.x), int(event.y)]
        elif event.button == 1 and event.type.value_name == "GDK_BUTTON_RELEASE":
            self.settings.vScreen = "%s" % {'startX': self.mpClick[0], 
                                            'startY': self.mpClick[1], 
                                            'endX': self.mpClick[2], 
                                            'endY': self.mpClick[3], 
                                            'width': self.mpClick[4],
                                            'height': self.mpClick[5] }
            self.mpClick = [ 0, 0, 0, 0, 0, 0 ]

    def mouseMotion( self, widget, event, mapper ):
        """
        This is the calback function for the button1 motion events
        in the mapper
        
        Arguments:
        - self: The main object pointer.
        - widget: The Drawing area.
        - event: The event information.
        """
        self.mpClick[2] = int( event.x )
        self.mpClick[3] = int( event.y )
        self.mpClick[4] = abs( int(event.x) - self.mpClick[0])
        self.mpClick[5] = abs( int(event.y) - self.mpClick[1] )
        self.gui.mapper.updateView()
    
    def drawMotionRectangle( self, context ):
        """
        This function draws a rectangle during the mouse motion event on the 
        mapper when the button 1 is pressed.
        
        Arguments:
        - self: The main object pointer
        - context: Cairo context to draw the rectangle.
        """
        
        if self.mpClick[0] < self.mpClick[2]:
            self.gui.mapper.drawRectangle( context, self.mpClick[0], self.mpClick[1], self.mpClick[4], self.mpClick[5], (10, 0.8, 0.1))
        else:
            self.gui.mapper.drawRectangle( context, self.mpClick[2], self.mpClick[3], self.mpClick[4], self.mpClick[5], (10, 0.8, 0.1))
            
    def _moveScreenMode( self, *args ):
        """
        Perform the movements of the pointer using the 'REAL MOUSE' mode.
        
        Arguments:
        - self: The main object pointer.
        """

        if not mouseTrap.getModVar( "cam", "forehead" ):
            return
       
        curX, curY = mouseTrap.mice( "position" )
        pointer = mouseTrap.getModVar( "cam", "mpPointer" )
        
        par = ["width", "height"]
        vScreenCenter = [ self.vScreen["startX"] + (self.vScreen["width"]/2),
                          self.vScreen["startY"] + (self.vScreen["height"]/2) ]
        
        newX, newY = [ (float(poss)/self.vScreen[par[i]])*env.screen[par[i]] 
                            for i,poss in enumerate([ (self.vScreen["width"]/2) - ( vScreenCenter[0] - pointer.x),
                                                      (self.vScreen["height"]/2) - ( vScreenCenter[1] - pointer.y ) ])]
       
        if not curX == newX or not curY == newY:
            mouseTrap.mice( "move", newX, newY )

    def prefTab( self, prefGui ):
        """
        This is the preferences tab function for the Screen Mode Profile.

        Arguments:
        - self: The main object pointer.
        - prefGui: The preferences gui pointer.
        """
        
        Frame = gtk.Frame()
        
        mainBox = gtk.VBox( spacing = 6)
        
        cdsHbox = gtk.HBox( spacing = 6 )
        
        vScreenlb = gtk.Label( ("%s" % self.vScreen ) )
        prefGui.preffWidgets['vScreen'] = vScreenlb
        
        self.spins = []
        
        startX = self.addSpin( prefGui, _("Start X: "), self.vScreen["startX"], max = 200)
        cdsHbox.pack_start( startX, False, False )
        
        startY = self.addSpin( prefGui, _("Start Y: "), self.vScreen["startY"], max = 160 )
        cdsHbox.pack_start( startY, False, False )
        
        width = self.addSpin( prefGui, _("Width: "), self.vScreen["width"], max = 200)
        cdsHbox.pack_start( width, False, False )
        
        height = self.addSpin( prefGui, _("Height: "), self.vScreen["height"], max = 160)
        cdsHbox.pack_start( height, False, False )
        
        mainBox.pack_start( cdsHbox, False, False )
        
        cdsHbox.show_all()
        mainBox.show_all()
        
        Frame.add( mainBox )
        Frame.show()
        
        prefGui.NoteBook.insert_page(Frame, gtk.Label( _("Screen Mode") ) )
        
    def spinChanged( self, widget, prefGui ):
        """
        This is the callback function for the spin change event.
        
        Arguments:
        - self: The main object pointer.
        - prefgui: the preferences gui pointer.
        """
        
        tmp = [ spin.get_value_as_int() for spin in self.spins  ]
        
        self.settings.vScreen = "%s" % {'startX': tmp[0], 'startY': tmp[1], 'endY': tmp[1] + tmp[3],
                                        'endX': tmp[0] + tmp[2], 'height': tmp[3], 'width': tmp[2]}
                                        
        prefGui.preffWidgets["vScreen"].set_text( self.settings.vScreen ) 

        
    def addSpin( self, prefGui, label, startValue, min = 1, max = 15):
        """
        Creates a new spin button inside a HBox and return it.
        
        Arguments:
        - self: The main object pointer.
        - label: The spin button label.
        - var: The prefferences dict variable.
        - startValue: The start value.
        """
        
        spinHbox = gtk.HBox( spacing = 4 )
        spinHbox.show_all()
        
        spinLbl = gtk.Label( label )
        spinLbl.set_alignment( 0.0, 0.5 )
        spinLbl.show()
        spinHbox.pack_start( spinLbl, False, False )
        
        adj = gtk.Adjustment( startValue, min, max, 1, 1, 0)
        spinButton = gtk.SpinButton( adj, 0.0, 0 )
        adj.connect("value_changed", self.spinChanged, prefGui )
        self.spins.append( spinButton )
        spinButton.set_wrap( True )
        spinHbox.pack_start( spinButton, False, False )
        
        spinLbl.set_mnemonic_widget( spinButton )
        
        return spinHbox

    def drawMapper( self, context ):
        """
        Calls the drawing function needed

        Arguments:
        - self: The main object pointer.
        - context: The Drawing area context to paint.
        """
        
        self._checkPref()
        
        if not 0 in self.mpClick:
            self.drawMotionRectangle( context )
            
        if self.vScreen["startX"] < self.vScreen["endX"]:
            self.gui.mapper.drawRectangle(context,  self.vScreen["startX"], self.vScreen["startY"], 
                                                        self.vScreen["width"], self.vScreen["height"], (255, 255, 255))
        else:
            self.gui.mapper.drawRectangle(context,  self.vScreen["endX"], self.vScreen["endY"], 
                                                        self.vScreen["width"], self.vScreen["height"], (255, 255, 255))