// https://forum.arduino.cc/index.php/topic,44216.0.html#11
unsigned FormatUnsigned(unsigned long val, char *buf, unsigned buf_len = 0xffff, byte width = 0);
void FormatDouble(double val, byte precision, char *buf, unsigned buf_len = 0xffff);

//
// Produce a formatted string in a buffer corresponding to the value provided.
// If the 'width' parameter is non-zero, the value will be padded with leading
// zeroes to achieve the specified width.  The number of characters added to
// the buffer (not including the null termination) is returned.
//
unsigned FormatUnsigned(unsigned long val, char *buf, unsigned buf_len, byte width) {
  if (!buf || !buf_len)
    return (0);

  // produce the digit string (backwards in the digit buffer)
  char dbuf[10];
  unsigned idx = 0;
  while (idx < sizeof(dbuf)) {
    dbuf[idx++] = (val % 10) + '0';
    if ((val /= 10) == 0)
      break;
  }

  // copy the optional leading zeroes and digits to the target buffer
  unsigned len = 0;
  byte padding = (width > idx) ? width - idx : 0;
  char c = '0';
  while ((--buf_len > 0) && (idx || padding)) {
    if (padding)
      padding--;
    else
      c = dbuf[--idx];
    *buf++ = c;
    len++;
  }

  // add the null termination
  *buf = '\0';
  return (len);
}

//
// Format a floating point value with number of decimal places.
// The 'precision' parameter is a number from 0 to 6 indicating the desired decimal places.
// The 'buf' parameter points to a buffer to receive the formatted string.  This must be
// sufficiently large to contain the resulting string.  The buffer's length may be
// optionally specified.  If it is given, the maximum length of the generated string
// will be one less than the specified value.
//
// example: FormatDouble(3.1415, 2, buf); // produces 3.14 (two decimal places)
//
void FormatDouble(double val, byte precision, char *buf, unsigned buf_len) {
  if (!buf || !buf_len)
    return;

  // limit the precision to the maximum allowed value
  const byte maxPrecision = 6;
  if (precision > maxPrecision)
    precision = maxPrecision;

  if (--buf_len > 0) {
    // check for a negative value
    if (val < 0.0) {
      val = -val;
      *buf = '-';
      buf_len--;
    }

    // compute the rounding factor and fractional multiplier
    double rounding_factor = 0.5;
    unsigned long mult = 1;
    for (byte i = 0; i < precision; i++) {
      rounding_factor /= 10.0;
      mult *= 10;
    }

    if (buf_len > 0) {
      // apply the rounding factor
      val += rounding_factor;

      // add the integral portion to the buffer
      unsigned len = FormatUnsigned((unsigned long)val, buf, buf_len);
      buf += len;
      buf_len -= len;
    }

    // handle the fractional portion
    if ((precision > 0) && (buf_len > 0)) {
      *buf++ = '.';
      if (--buf_len > 0)
        buf += FormatUnsigned((unsigned long)((val - (unsigned long)val) * mult), buf, buf_len, precision);
    }
  }

  // null-terminate the string
  *buf = '\0';
}