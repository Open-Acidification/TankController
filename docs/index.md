# Tank Controller

The library for Tank Controller. 
As more features get added to the library, they should only be added when testing for that feature is included.

## Topics for documentation

### Things to document

* Discuss debug strategy and logging via serial

* Document items in the EEPROM
  * pH
    * Precision x.xxx (three decimal places)
    * Common sea water 8.2
    * Experiments target 8.1 to 7.5
    * probe accuracy - can be to a few thousand (more likely to hundreth)
    * Thousand is helpful for setting and managing tank
    * **Probably could have a section on science of pH**
    * Requires calibration (1 and 2 point calibration)
      * Precision of xx.xxx
      * Sets the compensation temperature (required temperature to accurately measure)
      * Reading must stabilize (TODO make notification when stable)
      * both temperature and pH probe are used in calibration
      * **Needs detailed calibration instructions** (both manual steps and device code process)
    * <http://www.4oakton.com/TechTips/TT_ph.pdf>
  * PID
    * Calculates the length of bubbling
    * Must be longer than a tenth of a second to open the device for bubbling
    * Magic to get desired pH
  * Temperature (Celcius)
    * 0.03 accuracy
    * Requires calibration
    * Experiments target 10 to 15
    * Both chillers and heaters
  * Tank ID
    * Tank id used in logging
    * Basically a label for tank
    * Key for Google data sheets
    * sort/filter
  * Google sheet interval
    * Period of upload
    * Still needed (alternate reporting tool)
  * Observer time
  * Seed device addresses
    * Mac, IP Address
  * Reset screen
    * Allows for resetting for failed pixels
  * See Tank ID and Log file name
    * May need to review log process
  * See PID Constants
  * Tuning
    * Remove autotune
    * Manual tune is only working process
  * Sine wave
    * Set baseline, amplitude, and interval
    * Interval minutes per cycle
  * Temperature calibration
    * Enter the temperature offset
  * Set time
  * Enable/Disable PID
  * Set chill or heat
    * Possibly call "Temperature control"
* Main loop
  * Sensor must have delays between calls (~1 second)
  * Temperature set for 30 seconds at a time
  * pH bubbler uses a 10 second at a time
  * Show set points and current values
  * Ethernet
    * Google
    * Time
    * Ethernet release
    * HTTP API
