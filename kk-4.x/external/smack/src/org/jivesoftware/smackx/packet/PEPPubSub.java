/**
 * $RCSfile: PEPPubSub.java,v $
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

package org.jivesoftware.smackx.packet;

import org.jivesoftware.smack.packet.IQ;

/**
 * Represents XMPP PEP/XEP-163 pubsub packets.<p>
 * 
 * The 'http://jabber.org/protocol/pubsub' namespace  is used to publish personal events items from one client 
 * to subscribed clients (See XEP-163).
 *
 * @author Jeff Williams
 */
public class PEPPubSub extends IQ {
    
    PEPItem item;

    /**
    * Creates a new PubSub.
    *
    */
    public PEPPubSub(PEPItem item) {
        super();
        
        this.item = item;
    }

    /**
    * Returns the XML element name of the extension sub-packet root element.
    * Always returns "x"
    *
    * @return the XML element name of the packet extension.
    */
    public String getElementName() {
        return "pubsub";
    }

    /** 
     * Returns the XML namespace of the extension sub-packet root element.
     * According the specification the namespace is always "jabber:x:roster"
     * (which is not to be confused with the 'jabber:iq:roster' namespace
     *
     * @return the XML namespace of the packet extension.
     */
    public String getNamespace() {
        return "http://jabber.org/protocol/pubsub";
    }

    /**
     * Returns the XML representation of a Personal Event Publish according the specification.
     * 
     * Usually the XML representation will be inside of a Message XML representation like
     * in the following example:
     * <pre>
     * &lt;message id="MlIpV-4" to="gato1@gato.home" from="gato3@gato.home/Smack"&gt;
     *     &lt;subject&gt;Any subject you want&lt;/subject&gt;
     *     &lt;body&gt;This message contains roster items.&lt;/body&gt;
     *     &lt;x xmlns="jabber:x:roster"&gt;
     *         &lt;item jid="gato1@gato.home"/&gt;
     *         &lt;item jid="gato2@gato.home"/&gt;
     *     &lt;/x&gt;
     * &lt;/message&gt;
     * </pre>
     * 
     */
    public String getChildElementXML() {
        StringBuilder buf = new StringBuilder();
        buf.append("<").append(getElementName()).append(" xmlns=\"").append(getNamespace()).append("\">");
        buf.append("<publish node=\"").append(item.getNode()).append("\">");
        buf.append(item.toXML());
        buf.append("</publish>");
        buf.append("</").append(getElementName()).append(">");
        return buf.toString();
    }

}
