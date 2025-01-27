/****************************************************************
 * Licensed to the Apache Software Foundation (ASF) under one   *
 * or more contributor license agreements.  See the NOTICE file *
 * distributed with this work for additional information        *
 * regarding copyright ownership.  The ASF licenses this file   *
 * to you under the Apache License, Version 2.0 (the            *
 * "License"); you may not use this file except in compliance   *
 * with the License.  You may obtain a copy of the License at   *
 *                                                              *
 *   http://www.apache.org/licenses/LICENSE-2.0                 *
 *                                                              *
 * Unless required by applicable law or agreed to in writing,   *
 * software distributed under the License is distributed on an  *
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY       *
 * KIND, either express or implied.  See the License for the    *
 * specific language governing permissions and limitations      *
 * under the License.                                           *
 ****************************************************************/

package org.apache.james.mime4j.field;

import java.io.StringReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

//BEGIN android-changed: Stubbing out logging
import org.apache.james.mime4j.Log;
import org.apache.james.mime4j.LogFactory;
//END android-changed
import org.apache.james.mime4j.field.contenttype.parser.ContentTypeParser;
import org.apache.james.mime4j.field.contenttype.parser.ParseException;
import org.apache.james.mime4j.field.contenttype.parser.TokenMgrError;

/**
 * Represents a <code>Content-Type</code> field.
 *
 * <p>TODO: Remove dependency on Java 1.4 regexps</p>
 *
 *
 * @version $Id: //DTV/MP_BR/DTV_X_IDTV0801_002298_3_001/android/kk-4.x/packages/apps/UnifiedEmail/src/org/apache/james/mime4j/field/ContentTypeField.java#1 $
 */
public class ContentTypeField extends Field {

    /**
     * The prefix of all <code>multipart</code> MIME types.
     */
    public static final String TYPE_MULTIPART_PREFIX = "multipart/";
    /**
     * The <code>multipart/digest</code> MIME type.
     */
    public static final String TYPE_MULTIPART_DIGEST = "multipart/digest";
    /**
     * The <code>text/plain</code> MIME type.
     */
    public static final String TYPE_TEXT_PLAIN = "text/plain";
    /**
     * The <code>message/rfc822</code> MIME type.
     */
    public static final String TYPE_MESSAGE_RFC822 = "message/rfc822";
    /**
     * The name of the <code>boundary</code> parameter.
     */
    public static final String PARAM_BOUNDARY = "boundary";
    /**
     * The name of the <code>charset</code> parameter.
     */
    public static final String PARAM_CHARSET = "charset";

    private String mimeType = "";
    private Map<String, String> parameters = null;
    private ParseException parseException;

    protected ContentTypeField(String name, String body, String raw, String mimeType, Map<String, String> parameters, ParseException parseException) {
        super(name, body, raw);
        this.mimeType = mimeType;
        this.parameters = parameters;
        this.parseException = parseException;
    }

    /**
     * Gets the exception that was raised during parsing of
     * the field value, if any; otherwise, null.
     */
    public ParseException getParseException() {
        return parseException;
    }

    /**
     * Gets the MIME type defined in this Content-Type field.
     *
     * @return the MIME type or an empty string if not set.
     */
    public String getMimeType() {
        return mimeType;
    }

    /**
     * Gets the MIME type defined in the child's
     * Content-Type field or derives a MIME type from the parent
     * if child is <code>null</code> or hasn't got a MIME type value set.
     * If child's MIME type is multipart but no boundary
     * has been set the MIME type of child will be derived from
     * the parent.
     *
     * @param child the child.
     * @param parent the parent.
     * @return the MIME type.
     */
    public static String getMimeType(ContentTypeField child,
                                     ContentTypeField parent) {

        if (child == null || child.getMimeType().length() == 0
                || child.isMultipart() && child.getBoundary() == null) {

            if (parent != null && parent.isMimeType(TYPE_MULTIPART_DIGEST)) {
                return TYPE_MESSAGE_RFC822;
            } else {
                return TYPE_TEXT_PLAIN;
            }
        }

        return child.getMimeType();
    }

    /**
     * Gets the value of a parameter. Parameter names are case-insensitive.
     *
     * @param name the name of the parameter to get.
     * @return the parameter value or <code>null</code> if not set.
     */
    public String getParameter(String name) {
        return parameters != null
                    ? parameters.get(name.toLowerCase())
                    : null;
    }

    /**
     * Gets all parameters.
     *
     * @return the parameters.
     */
    public Map<String, String> getParameters() {
        if (parameters != null) {
            return Collections.unmodifiableMap(parameters);
        }
        return Collections.emptyMap();
    }

    /**
     * Gets the value of the <code>boundary</code> parameter if set.
     *
     * @return the <code>boundary</code> parameter value or <code>null</code>
     *             if not set.
     */
    public String getBoundary() {
        return getParameter(PARAM_BOUNDARY);
    }

    /**
     * Gets the value of the <code>charset</code> parameter if set.
     *
     * @return the <code>charset</code> parameter value or <code>null</code>
     *         if not set.
     */
    public String getCharset() {
        return getParameter(PARAM_CHARSET);
    }

    /**
     * Gets the value of the <code>charset</code> parameter if set for the
     * given field. Returns the default <code>us-ascii</code> if not set or if
     * <code>f</code> is <code>null</code>.
     *
     * @return the <code>charset</code> parameter value.
     */
    public static String getCharset(ContentTypeField f) {
        if (f != null) {
            if (f.getCharset() != null && f.getCharset().length() > 0) {
                return f.getCharset();
            }
        }
        return "us-ascii";
    }

    /**
     * Determines if the MIME type of this field matches the given one.
     *
     * @param mimeType the MIME type to match against.
     * @return <code>true</code> if the MIME type of this field matches,
     *         <code>false</code> otherwise.
     */
    public boolean isMimeType(String mimeType) {
        return this.mimeType.equalsIgnoreCase(mimeType);
    }

    /**
     * Determines if the MIME type of this field is <code>multipart/*</code>.
     *
     * @return <code>true</code> if this field is has a <code>multipart/*</code>
     *         MIME type, <code>false</code> otherwise.
     */
    public boolean isMultipart() {
        return mimeType.startsWith(TYPE_MULTIPART_PREFIX);
    }

    public static class Parser implements FieldParser {
        private static Log log = LogFactory.getLog(Parser.class);

        public Field parse(final String name, final String body, final String raw) {
            ParseException parseException = null;
            String mimeType = "";
            Map<String, String> parameters = null;

            ContentTypeParser parser = new ContentTypeParser(new StringReader(body));
            try {
                parser.parseAll();
            }
            catch (ParseException e) {
                if (log.isDebugEnabled()) {
                    log.debug("Parsing value '" + body + "': "+ e.getMessage());
                }
                parseException = e;
            }
            catch (TokenMgrError e) {
                if (log.isDebugEnabled()) {
                    log.debug("Parsing value '" + body + "': "+ e.getMessage());
                }
                parseException = new ParseException(e.getMessage());
            }

            try {
                final String type = parser.getType();
                final String subType = parser.getSubType();

                if (type != null && subType != null) {
                    mimeType = (type + "/" + parser.getSubType()).toLowerCase();

                    ArrayList<String> paramNames = parser.getParamNames();
                    ArrayList<String> paramValues = parser.getParamValues();

                    if (paramNames != null && paramValues != null) {
                        for (int i = 0; i < paramNames.size() && i < paramValues.size(); i++) {
                            if (parameters == null)
                                parameters = new HashMap<String, String>((int)(paramNames.size() * 1.3 + 1));
                            String paramName = paramNames.get(i).toLowerCase();
                            String paramValue = paramValues.get(i);
                            parameters.put(paramName, paramValue);
                        }
                    }
                }
            }
            catch (NullPointerException npe) {
            }
            return new ContentTypeField(name, body, raw, mimeType, parameters, parseException);
        }
    }
}
