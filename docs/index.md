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

