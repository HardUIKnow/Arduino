                          Sleep Demo Serial
     * -----------------
     * Example code to demonstrate the sleep functions in an Arduino.
     *
     * use a resistor between RX and pin2. By default RX is pulled up to 5V
     * therefore, we can use a sequence of Serial data forcing RX to 0, what
     * will make pin2 go LOW activating INT0 external interrupt, bringing
     * the MCU back to life
     *
     * there is also a time counter that will put the MCU to sleep after 10 secs
     *
     * NOTE: when coming back from POWER-DOWN mode, it takes a bit
     *       until the system is functional at 100%!! (typically <1sec)
     *
     * Copyright (C) 2006 MacSimski 2006-12-30
     * Copyright (C) 2007 D. Cuartielles 2007-07-08 - Mexico DF
     *
     *  This program is free software: you can redistribute it and/or modify
     *  it under the terms of the GNU General Public License as published by
     *  the Free Software Foundation, either version 3 of the License, or
     *  (at your option) any later version.
     *
     *  This program is distributed in the hope that it will be useful,
     *  but WITHOUT ANY WARRANTY; without even the implied warranty of
     *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     *  GNU General Public License for more details.
     *
     *  You should have received a copy of the GNU General Public License
     *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
     *