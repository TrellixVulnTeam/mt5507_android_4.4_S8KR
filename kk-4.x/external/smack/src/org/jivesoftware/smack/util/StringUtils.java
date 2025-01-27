/**
 * $RCSfile$
 * $Revision: #1 $
 * $Date: 2014/10/13 $
 *
 * Copyright 2003-2007 Jive Software.
 *
 * All rights reserved. Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.jivesoftware.smack.util;

import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.List;
import java.util.Random;
import java.util.TimeZone;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * A collection of utility methods for String objects.
 */
public class StringUtils {

	/**
     * Date format as defined in XEP-0082 - XMPP Date and Time Profiles. The time zone is set to
     * UTC.
     * <p>
     * Date formats are not synchronized. Since multiple threads access the format concurrently, it
     * must be synchronized externally or you can use the convenience methods
     * {@link #parseXEP0082Date(String)} and {@link #formatXEP0082Date(Date)}.
     * @deprecated This public version will be removed in favor of using the methods defined within this class.
     */
    public static final DateFormat XEP_0082_UTC_FORMAT = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.SSS'Z'");
    
    /*
     * private version to use internally so we don't have to be concerned with thread safety.
     */
    private static final DateFormat dateFormatter = DateFormatType.XEP_0082_DATE_PROFILE.createFormatter();
    private static final Pattern datePattern = Pattern.compile("^\\d+-\\d+-\\d+$");
    
    private static final DateFormat timeFormatter = DateFormatType.XEP_0082_TIME_MILLIS_ZONE_PROFILE.createFormatter();
    private static final Pattern timePattern = Pattern.compile("^(\\d+:){2}\\d+.\\d+(Z|([+-](\\d+:\\d+)))$");
    private static final DateFormat timeNoZoneFormatter = DateFormatType.XEP_0082_TIME_MILLIS_PROFILE.createFormatter();
    private static final Pattern timeNoZonePattern = Pattern.compile("^(\\d+:){2}\\d+.\\d+$");
    
    private static final DateFormat timeNoMillisFormatter = DateFormatType.XEP_0082_TIME_ZONE_PROFILE.createFormatter();
    private static final Pattern timeNoMillisPattern = Pattern.compile("^(\\d+:){2}\\d+(Z|([+-](\\d+:\\d+)))$");
    private static final DateFormat timeNoMillisNoZoneFormatter = DateFormatType.XEP_0082_TIME_PROFILE.createFormatter();
    private static final Pattern timeNoMillisNoZonePattern = Pattern.compile("^(\\d+:){2}\\d+$");
    
    private static final DateFormat dateTimeFormatter = DateFormatType.XEP_0082_DATETIME_MILLIS_PROFILE.createFormatter();
    private static final Pattern dateTimePattern = Pattern.compile("^\\d+(-\\d+){2}+T(\\d+:){2}\\d+.\\d+(Z|([+-](\\d+:\\d+)))?$");
    private static final DateFormat dateTimeNoMillisFormatter = DateFormatType.XEP_0082_DATETIME_PROFILE.createFormatter();
    private static final Pattern dateTimeNoMillisPattern = Pattern.compile("^\\d+(-\\d+){2}+T(\\d+:){2}\\d+(Z|([+-](\\d+:\\d+)))?$");

    private static final DateFormat xep0091Formatter = new SimpleDateFormat("yyyyMMdd'T'HH:mm:ss");
    private static final DateFormat xep0091Date6DigitFormatter = new SimpleDateFormat("yyyyMd'T'HH:mm:ss");
    private static final DateFormat xep0091Date7Digit1MonthFormatter = new SimpleDateFormat("yyyyMdd'T'HH:mm:ss");
    private static final DateFormat xep0091Date7Digit2MonthFormatter = new SimpleDateFormat("yyyyMMd'T'HH:mm:ss");
    private static final Pattern xep0091Pattern = Pattern.compile("^\\d+T\\d+:\\d+:\\d+$");
    
    private static final List<PatternCouplings> couplings = new ArrayList<PatternCouplings>();
    
    static {
    	TimeZone utc = TimeZone.getTimeZone("UTC");
        XEP_0082_UTC_FORMAT.setTimeZone(utc);
        dateFormatter.setTimeZone(utc);
        timeFormatter.setTimeZone(utc);
        timeNoZoneFormatter.setTimeZone(utc);
        timeNoMillisFormatter.setTimeZone(utc);
        timeNoMillisNoZoneFormatter.setTimeZone(utc);
        dateTimeFormatter.setTimeZone(utc);
        dateTimeNoMillisFormatter.setTimeZone(utc);
        
        xep0091Formatter.setTimeZone(utc);
        xep0091Date6DigitFormatter.setTimeZone(utc);
        xep0091Date7Digit1MonthFormatter.setTimeZone(utc);
        xep0091Date7Digit1MonthFormatter.setLenient(false);
        xep0091Date7Digit2MonthFormatter.setTimeZone(utc);
        xep0091Date7Digit2MonthFormatter.setLenient(false);
        
        couplings.add(new PatternCouplings(datePattern, dateFormatter));
        couplings.add(new PatternCouplings(dateTimePattern, dateTimeFormatter, true));
        couplings.add(new PatternCouplings(dateTimeNoMillisPattern, dateTimeNoMillisFormatter, true));
        couplings.add(new PatternCouplings(timePattern, timeFormatter, true));
        couplings.add(new PatternCouplings(timeNoZonePattern, timeNoZoneFormatter));
        couplings.add(new PatternCouplings(timeNoMillisPattern, timeNoMillisFormatter, true));
        couplings.add(new PatternCouplings(timeNoMillisNoZonePattern, timeNoMillisNoZoneFormatter));
    }

    private static final char[] QUOTE_ENCODE = "&quot;".toCharArray();
    private static final char[] APOS_ENCODE = "&apos;".toCharArray();
    private static final char[] AMP_ENCODE = "&amp;".toCharArray();
    private static final char[] LT_ENCODE = "&lt;".toCharArray();
    private static final char[] GT_ENCODE = "&gt;".toCharArray();

    /**
     * Parses the given date string in the <a href="http://xmpp.org/extensions/xep-0082.html">XEP-0082 - XMPP Date and Time Profiles</a>.
     * 
     * @param dateString the date string to parse
     * @return the parsed Date
     * @throws ParseException if the specified string cannot be parsed
     * @deprecated Use {@link #parseDate(String)} instead.
     * 
     */
    public static Date parseXEP0082Date(String dateString) throws ParseException {
    	return parseDate(dateString);
    }
    
    /**
     * Parses the given date string in either of the three profiles of <a href="http://xmpp.org/extensions/xep-0082.html">XEP-0082 - XMPP Date and Time Profiles</a>
     * or <a href="http://xmpp.org/extensions/xep-0091.html">XEP-0091 - Legacy Delayed Delivery</a> format.
     * <p>
     * This method uses internal date formatters and is thus threadsafe.
     * @param dateString the date string to parse
     * @return the parsed Date
     * @throws ParseException if the specified string cannot be parsed
     */
    public static Date parseDate(String dateString) throws ParseException {
        Matcher matcher = xep0091Pattern.matcher(dateString);
        
        /*
         * if date is in XEP-0091 format handle ambiguous dates missing the
         * leading zero in month and day
         */
        if (matcher.matches()) {
        	int length = dateString.split("T")[0].length();
        	
            if (length < 8) {
                Date date = handleDateWithMissingLeadingZeros(dateString, length);

                if (date != null)
                	return date;
            }
            else {
            	synchronized (xep0091Formatter) {
                	return xep0091Formatter.parse(dateString);
				}
            }
        }
        else {
        	for (PatternCouplings coupling : couplings) {
                matcher = coupling.pattern.matcher(dateString);
                
                if (matcher.matches())
                {
                	if (coupling.needToConvertTimeZone) {
                		dateString = coupling.convertTime(dateString);
                	}
                		
                    synchronized (coupling.formatter) {
                    	return coupling.formatter.parse(dateString);
                    }
                }
			}
        }
        
        /*
         * We assume it is the XEP-0082 DateTime profile with no milliseconds at this point.  If it isn't, is is just not parseable, then we attempt
         * to parse it regardless and let it throw the ParseException. 
         */
        synchronized (dateTimeNoMillisFormatter) {
        	return dateTimeNoMillisFormatter.parse(dateString);
        }
    }
    
    /**
     * Parses the given date string in different ways and returns the date that
     * lies in the past and/or is nearest to the current date-time.
     * 
     * @param stampString date in string representation
     * @param dateLength 
     * @param noFuture 
     * @return the parsed date
     * @throws ParseException The date string was of an unknown format
     */
    private static Date handleDateWithMissingLeadingZeros(String stampString, int dateLength) throws ParseException {
        if (dateLength == 6) {
        	synchronized (xep0091Date6DigitFormatter) {
				return xep0091Date6DigitFormatter.parse(stampString);
			}
        }
        Calendar now = Calendar.getInstance();
        
        Calendar oneDigitMonth = parseXEP91Date(stampString, xep0091Date7Digit1MonthFormatter);
        Calendar twoDigitMonth = parseXEP91Date(stampString, xep0091Date7Digit2MonthFormatter);
        
        List<Calendar> dates = filterDatesBefore(now, oneDigitMonth, twoDigitMonth);
        
        if (!dates.isEmpty()) {
            return determineNearestDate(now, dates).getTime();
        } 
        return null;
    }

    private static Calendar parseXEP91Date(String stampString, DateFormat dateFormat) {
        try {
            synchronized (dateFormat) {
                dateFormat.parse(stampString);
                return dateFormat.getCalendar();
            }
        }
        catch (ParseException e) {
            return null;
        }
    }

    private static List<Calendar> filterDatesBefore(Calendar now, Calendar... dates) {
        List<Calendar> result = new ArrayList<Calendar>();
        
        for (Calendar calendar : dates) {
            if (calendar != null && calendar.before(now)) {
                result.add(calendar);
            }
        }

        return result;
    }

    private static Calendar determineNearestDate(final Calendar now, List<Calendar> dates) {
        
        Collections.sort(dates, new Comparator<Calendar>() {

            public int compare(Calendar o1, Calendar o2) {
                Long diff1 = new Long(now.getTimeInMillis() - o1.getTimeInMillis());
                Long diff2 = new Long(now.getTimeInMillis() - o2.getTimeInMillis());
                return diff1.compareTo(diff2);
            }
            
        });
        
        return dates.get(0);
    }

    /**
     * Formats a Date into a XEP-0082 - XMPP Date and Time Profiles string.
     * 
     * @param date the time value to be formatted into a time string
     * @return the formatted time string in XEP-0082 format
     */
    public static String formatXEP0082Date(Date date) {
        synchronized (dateTimeFormatter) {
            return dateTimeFormatter.format(date);
        }
    }

    public static String formatDate(Date toFormat, DateFormatType type)
    {
    	return null;
    }
    
    /**
     * Returns the name portion of a XMPP address. For example, for the
     * address "matt@jivesoftware.com/Smack", "matt" would be returned. If no
     * username is present in the address, the empty string will be returned.
     *
     * @param XMPPAddress the XMPP address.
     * @return the name portion of the XMPP address.
     */
    public static String parseName(String XMPPAddress) {
        if (XMPPAddress == null) {
            return null;
        }
        int atIndex = XMPPAddress.lastIndexOf("@");
        if (atIndex <= 0) {
            return "";
        }
        else {
            return XMPPAddress.substring(0, atIndex);
        }
    }

    /**
     * Returns the server portion of a XMPP address. For example, for the
     * address "matt@jivesoftware.com/Smack", "jivesoftware.com" would be returned.
     * If no server is present in the address, the empty string will be returned.
     *
     * @param XMPPAddress the XMPP address.
     * @return the server portion of the XMPP address.
     */
    public static String parseServer(String XMPPAddress) {
        if (XMPPAddress == null) {
            return null;
        }
        int atIndex = XMPPAddress.lastIndexOf("@");
        // If the String ends with '@', return the empty string.
        if (atIndex + 1 > XMPPAddress.length()) {
            return "";
        }
        int slashIndex = XMPPAddress.indexOf("/");
        if (slashIndex > 0 && slashIndex > atIndex) {
            return XMPPAddress.substring(atIndex + 1, slashIndex);
        }
        else {
            return XMPPAddress.substring(atIndex + 1);
        }
    }

    /**
     * Returns the resource portion of a XMPP address. For example, for the
     * address "matt@jivesoftware.com/Smack", "Smack" would be returned. If no
     * resource is present in the address, the empty string will be returned.
     *
     * @param XMPPAddress the XMPP address.
     * @return the resource portion of the XMPP address.
     */
    public static String parseResource(String XMPPAddress) {
        if (XMPPAddress == null) {
            return null;
        }
        int slashIndex = XMPPAddress.indexOf("/");
        if (slashIndex + 1 > XMPPAddress.length() || slashIndex < 0) {
            return "";
        }
        else {
            return XMPPAddress.substring(slashIndex + 1);
        }
    }

    /**
     * Returns the XMPP address with any resource information removed. For example,
     * for the address "matt@jivesoftware.com/Smack", "matt@jivesoftware.com" would
     * be returned.
     *
     * @param XMPPAddress the XMPP address.
     * @return the bare XMPP address without resource information.
     */
    public static String parseBareAddress(String XMPPAddress) {
        if (XMPPAddress == null) {
            return null;
        }
        int slashIndex = XMPPAddress.indexOf("/");
        if (slashIndex < 0) {
            return XMPPAddress;
        }
        else if (slashIndex == 0) {
            return "";
        }
        else {
            return XMPPAddress.substring(0, slashIndex);
        }
    }

    /**
     * Returns true if jid is a full JID (i.e. a JID with resource part).
     *
     * @param jid
     * @return true if full JID, false otherwise
     */
    public static boolean isFullJID(String jid) {
        if (parseName(jid).length() <= 0 || parseServer(jid).length() <= 0
                || parseResource(jid).length() <= 0) {
            return false;
        }
        return true;
    }

    /**
     * Escapes the node portion of a JID according to "JID Escaping" (JEP-0106).
     * Escaping replaces characters prohibited by node-prep with escape sequences,
     * as follows:<p>
     *
     * <table border="1">
     * <tr><td><b>Unescaped Character</b></td><td><b>Encoded Sequence</b></td></tr>
     * <tr><td>&lt;space&gt;</td><td>\20</td></tr>
     * <tr><td>"</td><td>\22</td></tr>
     * <tr><td>&</td><td>\26</td></tr>
     * <tr><td>'</td><td>\27</td></tr>
     * <tr><td>/</td><td>\2f</td></tr>
     * <tr><td>:</td><td>\3a</td></tr>
     * <tr><td>&lt;</td><td>\3c</td></tr>
     * <tr><td>&gt;</td><td>\3e</td></tr>
     * <tr><td>@</td><td>\40</td></tr>
     * <tr><td>\</td><td>\5c</td></tr>
     * </table><p>
     *
     * This process is useful when the node comes from an external source that doesn't
     * conform to nodeprep. For example, a username in LDAP may be "Joe Smith". Because
     * the &lt;space&gt; character isn't a valid part of a node, the username should
     * be escaped to "Joe\20Smith" before being made into a JID (e.g. "joe\20smith@example.com"
     * after case-folding, etc. has been applied).<p>
     *
     * All node escaping and un-escaping must be performed manually at the appropriate
     * time; the JID class will not escape or un-escape automatically.
     *
     * @param node the node.
     * @return the escaped version of the node.
     */
    public static String escapeNode(String node) {
        if (node == null) {
            return null;
        }
        StringBuilder buf = new StringBuilder(node.length() + 8);
        for (int i=0, n=node.length(); i<n; i++) {
            char c = node.charAt(i);
            switch (c) {
                case '"': buf.append("\\22"); break;
                case '&': buf.append("\\26"); break;
                case '\'': buf.append("\\27"); break;
                case '/': buf.append("\\2f"); break;
                case ':': buf.append("\\3a"); break;
                case '<': buf.append("\\3c"); break;
                case '>': buf.append("\\3e"); break;
                case '@': buf.append("\\40"); break;
                case '\\': buf.append("\\5c"); break;
                default: {
                    if (Character.isWhitespace(c)) {
                        buf.append("\\20");
                    }
                    else {
                        buf.append(c);
                    }
                }
            }
        }
        return buf.toString();
    }

    /**
     * Un-escapes the node portion of a JID according to "JID Escaping" (JEP-0106).<p>
     * Escaping replaces characters prohibited by node-prep with escape sequences,
     * as follows:<p>
     *
     * <table border="1">
     * <tr><td><b>Unescaped Character</b></td><td><b>Encoded Sequence</b></td></tr>
     * <tr><td>&lt;space&gt;</td><td>\20</td></tr>
     * <tr><td>"</td><td>\22</td></tr>
     * <tr><td>&</td><td>\26</td></tr>
     * <tr><td>'</td><td>\27</td></tr>
     * <tr><td>/</td><td>\2f</td></tr>
     * <tr><td>:</td><td>\3a</td></tr>
     * <tr><td>&lt;</td><td>\3c</td></tr>
     * <tr><td>&gt;</td><td>\3e</td></tr>
     * <tr><td>@</td><td>\40</td></tr>
     * <tr><td>\</td><td>\5c</td></tr>
     * </table><p>
     *
     * This process is useful when the node comes from an external source that doesn't
     * conform to nodeprep. For example, a username in LDAP may be "Joe Smith". Because
     * the &lt;space&gt; character isn't a valid part of a node, the username should
     * be escaped to "Joe\20Smith" before being made into a JID (e.g. "joe\20smith@example.com"
     * after case-folding, etc. has been applied).<p>
     *
     * All node escaping and un-escaping must be performed manually at the appropriate
     * time; the JID class will not escape or un-escape automatically.
     *
     * @param node the escaped version of the node.
     * @return the un-escaped version of the node.
     */
    public static String unescapeNode(String node) {
        if (node == null) {
            return null;
        }
        char [] nodeChars = node.toCharArray();
        StringBuilder buf = new StringBuilder(nodeChars.length);
        for (int i=0, n=nodeChars.length; i<n; i++) {
            compare: {
                char c = node.charAt(i);
                if (c == '\\' && i+2<n) {
                    char c2 = nodeChars[i+1];
                    char c3 = nodeChars[i+2];
                    if (c2 == '2') {
                        switch (c3) {
                            case '0': buf.append(' '); i+=2; break compare;
                            case '2': buf.append('"'); i+=2; break compare;
                            case '6': buf.append('&'); i+=2; break compare;
                            case '7': buf.append('\''); i+=2; break compare;
                            case 'f': buf.append('/'); i+=2; break compare;
                        }
                    }
                    else if (c2 == '3') {
                        switch (c3) {
                            case 'a': buf.append(':'); i+=2; break compare;
                            case 'c': buf.append('<'); i+=2; break compare;
                            case 'e': buf.append('>'); i+=2; break compare;
                        }
                    }
                    else if (c2 == '4') {
                        if (c3 == '0') {
                            buf.append("@");
                            i+=2;
                            break compare;
                        }
                    }
                    else if (c2 == '5') {
                        if (c3 == 'c') {
                            buf.append("\\");
                            i+=2;
                            break compare;
                        }
                    }
                }
                buf.append(c);
            }
        }
        return buf.toString();
    }

    /**
     * Escapes all necessary characters in the String so that it can be used
     * in an XML doc.
     *
     * @param string the string to escape.
     * @return the string with appropriate characters escaped.
     */
    public static String escapeForXML(String string) {
        if (string == null) {
            return null;
        }
        char ch;
        int i=0;
        int last=0;
        char[] input = string.toCharArray();
        int len = input.length;
        StringBuilder out = new StringBuilder((int)(len*1.3));
        for (; i < len; i++) {
            ch = input[i];
            if (ch > '>') {
            }
            else if (ch == '<') {
                if (i > last) {
                    out.append(input, last, i - last);
                }
                last = i + 1;
                out.append(LT_ENCODE);
            }
            else if (ch == '>') {
                if (i > last) {
                    out.append(input, last, i - last);
                }
                last = i + 1;
                out.append(GT_ENCODE);
            }

            else if (ch == '&') {
                if (i > last) {
                    out.append(input, last, i - last);
                }
                // Do nothing if the string is of the form &#235; (unicode value)
                if (!(len > i + 5
                    && input[i + 1] == '#'
                    && Character.isDigit(input[i + 2])
                    && Character.isDigit(input[i + 3])
                    && Character.isDigit(input[i + 4])
                    && input[i + 5] == ';')) {
                        last = i + 1;
                        out.append(AMP_ENCODE);
                    }
            }
            else if (ch == '"') {
                if (i > last) {
                    out.append(input, last, i - last);
                }
                last = i + 1;
                out.append(QUOTE_ENCODE);
            }
            else if (ch == '\'') {
                if (i > last) {
                    out.append(input, last, i - last);
                }
                last = i + 1;
                out.append(APOS_ENCODE);
            }
        }
        if (last == 0) {
            return string;
        }
        if (i > last) {
            out.append(input, last, i - last);
        }
        return out.toString();
    }

    /**
     * Used by the hash method.
     */
    private static MessageDigest digest = null;

    /**
     * Hashes a String using the SHA-1 algorithm and returns the result as a
     * String of hexadecimal numbers. This method is synchronized to avoid
     * excessive MessageDigest object creation. If calling this method becomes
     * a bottleneck in your code, you may wish to maintain a pool of
     * MessageDigest objects instead of using this method.
     * <p>
     * A hash is a one-way function -- that is, given an
     * input, an output is easily computed. However, given the output, the
     * input is almost impossible to compute. This is useful for passwords
     * since we can store the hash and a hacker will then have a very hard time
     * determining the original password.
     *
     * @param data the String to compute the hash of.
     * @return a hashed version of the passed-in String
     */
    public synchronized static String hash(String data) {
        if (digest == null) {
            try {
                digest = MessageDigest.getInstance("SHA-1");
            }
            catch (NoSuchAlgorithmException nsae) {
                System.err.println("Failed to load the SHA-1 MessageDigest. " +
                "Jive will be unable to function normally.");
            }
        }
        // Now, compute hash.
        try {
            digest.update(data.getBytes("UTF-8"));
        }
        catch (UnsupportedEncodingException e) {
            System.err.println(e);
        }
        return encodeHex(digest.digest());
    }

    /**
     * Encodes an array of bytes as String representation of hexadecimal.
     *
     * @param bytes an array of bytes to convert to a hex string.
     * @return generated hex string.
     */
    public static String encodeHex(byte[] bytes) {
        StringBuilder hex = new StringBuilder(bytes.length * 2);

        for (byte aByte : bytes) {
            if (((int) aByte & 0xff) < 0x10) {
                hex.append("0");
            }
            hex.append(Integer.toString((int) aByte & 0xff, 16));
        }

        return hex.toString();
    }

    /**
     * Encodes a String as a base64 String.
     *
     * @param data a String to encode.
     * @return a base64 encoded String.
     */
    public static String encodeBase64(String data) {
        byte [] bytes = null;
        try {
            bytes = data.getBytes("ISO-8859-1");
        }
        catch (UnsupportedEncodingException uee) {
            uee.printStackTrace();
        }
        return encodeBase64(bytes);
    }

    /**
     * Encodes a byte array into a base64 String.
     *
     * @param data a byte array to encode.
     * @return a base64 encode String.
     */
    public static String encodeBase64(byte[] data) {
        return encodeBase64(data, false);
    }

    /**
     * Encodes a byte array into a bse64 String.
     *
     * @param data The byte arry to encode.
     * @param lineBreaks True if the encoding should contain line breaks and false if it should not.
     * @return A base64 encoded String.
     */
    public static String encodeBase64(byte[] data, boolean lineBreaks) {
        return encodeBase64(data, 0, data.length, lineBreaks);
    }

    /**
     * Encodes a byte array into a bse64 String.
     *
     * @param data The byte arry to encode.
     * @param offset the offset of the bytearray to begin encoding at.
     * @param len the length of bytes to encode.
     * @param lineBreaks True if the encoding should contain line breaks and false if it should not.
     * @return A base64 encoded String.
     */
    public static String encodeBase64(byte[] data, int offset, int len, boolean lineBreaks) {
        return Base64.encodeBytes(data, offset, len, (lineBreaks ?  Base64.NO_OPTIONS : Base64.DONT_BREAK_LINES));
    }

    /**
     * Decodes a base64 String.
     * Unlike Base64.decode() this method does not try to detect and decompress a gzip-compressed input.
     *
     * @param data a base64 encoded String to decode.
     * @return the decoded String.
     */
    public static byte[] decodeBase64(String data) {
        byte[] bytes;
        try {
            bytes = data.getBytes("UTF-8");
        } catch (java.io.UnsupportedEncodingException uee) {
            bytes = data.getBytes();
        }

        bytes = Base64.decode(bytes, 0, bytes.length, Base64.NO_OPTIONS);
        return bytes;
    }

    /**
     * Pseudo-random number generator object for use with randomString().
     * The Random class is not considered to be cryptographically secure, so
     * only use these random Strings for low to medium security applications.
     */
    private static Random randGen = new Random();

    /**
     * Array of numbers and letters of mixed case. Numbers appear in the list
     * twice so that there is a more equal chance that a number will be picked.
     * We can use the array to get a random number or letter by picking a random
     * array index.
     */
    private static char[] numbersAndLetters = ("0123456789abcdefghijklmnopqrstuvwxyz" +
                    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").toCharArray();

    /**
     * Returns a random String of numbers and letters (lower and upper case)
     * of the specified length. The method uses the Random class that is
     * built-in to Java which is suitable for low to medium grade security uses.
     * This means that the output is only pseudo random, i.e., each number is
     * mathematically generated so is not truly random.<p>
     *
     * The specified length must be at least one. If not, the method will return
     * null.
     *
     * @param length the desired length of the random String to return.
     * @return a random String of numbers and letters of the specified length.
     */
    public static String randomString(int length) {
        if (length < 1) {
            return null;
        }
        // Create a char buffer to put random letters and numbers in.
        char [] randBuffer = new char[length];
        for (int i=0; i<randBuffer.length; i++) {
            randBuffer[i] = numbersAndLetters[randGen.nextInt(71)];
        }
        return new String(randBuffer);
    }

    private StringUtils() {
        // Not instantiable.
    }
    
    private static class PatternCouplings {
    	Pattern pattern;
    	DateFormat formatter;
    	boolean needToConvertTimeZone = false;
    	
    	public PatternCouplings(Pattern datePattern, DateFormat dateFormat) {
    		pattern = datePattern;
    		formatter = dateFormat;
		}

    	public PatternCouplings(Pattern datePattern, DateFormat dateFormat, boolean shouldConvertToRFC822) {
    		pattern = datePattern;
    		formatter = dateFormat;
    		needToConvertTimeZone = shouldConvertToRFC822;
		}
    	
    	public String convertTime(String dateString) {
            if (dateString.charAt(dateString.length() - 1) == 'Z') {
                return dateString.replace("Z", "+0000");
            }
            else {
            	// If the time zone wasn't specified with 'Z', then it's in
            	// ISO8601 format (i.e. '(+|-)HH:mm')
            	// RFC822 needs a similar format just without the colon (i.e.
            	// '(+|-)HHmm)'), so remove it
                return dateString.replaceAll("([\\+\\-]\\d\\d):(\\d\\d)","$1$2");
    		}
    	}
	}

}
