/*
 *  Licensed to the Apache Software Foundation (ASF) under one or more
 *  contributor license agreements.  See the NOTICE file distributed with
 *  this work for additional information regarding copyright ownership.
 *  The ASF licenses this file to You under the Apache License, Version 2.0
 *  (the "License"); you may not use this file except in compliance with
 *  the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
* @author Vera Y. Petrashkova
* @version $Revision: #1 $
*/

package tests.api.javax.security.cert;

import junit.framework.TestCase;

import javax.security.cert.CertificateException;


/**
 * Tests for <code>DigestException</code> class constructors and methods.
 *
 */
public class CertificateExceptionTest extends TestCase {

    static String[] msgs = {
            "",
            "Check new message",
            "Check new message Check new message Check new message Check new message Check new message" };

    static Throwable tCause = new Throwable("Throwable for exception");

    /**
     * Test for <code>CertificateException()</code> constructor Assertion:
     * constructs CertificateException with no detail message
     */
    public void testCertificateException01() {
        CertificateException tE = new CertificateException();
        assertNull("getMessage() must return null.", tE.getMessage());
        assertNull("getCause() must return null", tE.getCause());
    }

    /**
     * Test for <code>CertificateException(String)</code> constructor
     * Assertion: constructs CertificateException with detail message msg.
     * Parameter <code>msg</code> is not null.
     */
    public void testCertificateException02() {
        CertificateException tE;
        for (int i = 0; i < msgs.length; i++) {
            tE = new CertificateException(msgs[i]);
            assertEquals("getMessage() must return: ".concat(msgs[i]), tE
                    .getMessage(), msgs[i]);
            assertNull("getCause() must return null", tE.getCause());
        }
    }

    /**
     * Test for <code>CertificateException(String)</code> constructor
     * Assertion: constructs CertificateException when <code>msg</code> is
     * null
     */
    public void testCertificateException03() {
        String msg = null;
        CertificateException tE = new CertificateException(msg);
        assertNull("getMessage() must return null.", tE.getMessage());
        assertNull("getCause() must return null", tE.getCause());
    }
}
